#include "Compiler.h"

// common vars
const char wsChars[] = {0x9, 0xA, 0xD, 0x20}; // tab, newline, carriage return, space
const vector <char> whitespace(wsChars, wsChars + sizeof(wsChars) / sizeof(char));
map <string, TokenType> keywords; // populated by initKeywordList();

string Position::toString() const {
	stringstream output;
	output << "L" << lineNumber << "C" << character;
	return output.str();
}

void initKeywordList() {
	// this is wrapped in a function because I can't populate a map in global scope
	// (as far as I know)
	keywords["bool"] = TT_BOOL;
	keywords["byte"] = TT_BYTE; 
	keywords["short"] = TT_SHORT;
	keywords["long"] = TT_LONG;
	
	keywords["+"] = TT_PLUS;
	keywords["-"] = TT_MINUS;
	keywords["*"] = TT_MULTIPLY;
	keywords["/"] = TT_DIVIDE;
	keywords["%"] = TT_MOD;
	keywords["<"] = TT_LESS_THAN;
	keywords["<="] = TT_LESS_THAN_OR_EQUAL;
	keywords[">"] = TT_GREATER_THAN;
	keywords[">="] = TT_GREATER_THAN_OR_EQUAL;
	keywords["="] = TT_EQUAL;
	keywords["<>"] = TT_NOT_EQUAL;
	keywords["and"] = TT_AND;
	keywords["or"] = TT_OR;
	
	keywords["not"] = TT_NOT;
	keywords["neg"] = TT_NEG;
	keywords["++"] = TT_INC;
	keywords["--"] = TT_DEC;
	
	keywords["true"] = TT_TRUE;
	keywords["false"] = TT_FALSE;
	
	keywords["function"] = TT_FUNCTION;
	keywords["vars"] = TT_VARS;
	keywords["do"] = TT_DO;
	keywords["return"] = TT_RETURN;
	keywords["end"] = TT_END;
	keywords["set"] = TT_SET;
	keywords["if"] = TT_IF;
	keywords["else"] = TT_ELSE;
	keywords["{"] = TT_OPEN_COMPOUND;
	keywords["}"] = TT_CLOSE_COMPOUND;
	keywords["["] = TT_OPEN_ARRAY;
	keywords["]"] = TT_CLOSE_ARRAY;
	keywords[":"] = TT_COLON;
	keywords[";"] = TT_SEMICOLON;
	keywords["^"] = TT_CARET;
	keywords["!"] = TT_BANG;
	
	keywords["=="] = TT_INVALID; // a comment - doesn't need a real type, as it won't get pushed as a token
}

// checks if a token is a keyword
bool Tokenizer::isKeyword(const string test) {
	return keywords.find(test) != keywords.end();
}

// checks if a character is whitespace
bool Tokenizer::isWhitespace(const char test) {
	return binary_search(whitespace.begin(), whitespace.end(), test);
}

// checks if a character can be used in an identifier
bool Tokenizer::isIdentifierChar(const char test, const int pos) {
	if (test >= 65 && test <= 90) return true; // uppercase letters
	if (test >= 97 && test <= 122) return true; // lowercase letters
	if (pos && test >= 48 && test <= 57) return true; // numbers (only if not first character)
	return false;
}

bool Tokenizer::isConstantChar(const char test) {
	return (test >= 48 && test <= 57);
}

// construct the Tokenizer object
Tokenizer::Tokenizer(istream &ninput, ostream &nerrors) :
	input(ninput), errors(nerrors) {
	initKeywordList();
	position.lineNumber = 1;
	position.character = 0;
	previousToken = NULL;
	currentToken = NULL;
	nextToken();
}

// move to next token
void Tokenizer::nextToken() {
	string curToken; // the current token string
	bool isConstant = false;
	bool maybeIdentifier = true;
	
	while (!input.eof()) {
		// read in one more character
		char thisChar = input.get();
		
		// update position
		if (thisChar == 0xA) { // if char is a newline
			position.lineNumber++;
			position.character = 0;
		} else if (thisChar == 0x9) {
			position.character += 4;
		} else {
			position.character++;
		}
		
		// if new char is whitespace, reset curToken
		if (isWhitespace(thisChar)) {
			if (curToken.size()) {
				// all important tokens will have been caught. This token must be invalid.
				return pushToken(curToken, TT_INVALID);
			} else {
				curToken = "";
				continue;
			}
		}
		curToken += thisChar;
		
		// could this be an identifier so far?
		if (!isIdentifierChar(thisChar, curToken.size()-1)) {
			maybeIdentifier = false;
		}
		
		// see if the current token string matches a reserved word
		if (isKeyword(curToken)) {
			// skip the line if it's a comment
			if (curToken.compare("==") == 0) {
				string trash;
				getline(input, trash);
				position.lineNumber++;
				position.character = 0;
				return nextToken();
			}
			
			// check if concatenation of next char is a keyword
			string nextTest(curToken);
			char nextChar(input.peek());
			nextTest += nextChar;
			if (isKeyword(nextTest)) {
				continue; // let next iteration catch keyword
			} else if (!maybeIdentifier || !isIdentifierChar(nextChar, curToken.size()-1)) {
				// we got ourselves a keyword to tokenize
				return pushToken(curToken, keywords[curToken]);
			}
		}
		
		// check if this is a constant
		if ((isConstant || curToken.size() == 1) && isConstantChar(thisChar)) {
			isConstant = true;
			if (isConstantChar(input.peek())) {
				continue; // keep processing 
			} else {
				// we've got ourselves a constant
				return pushToken(curToken, TT_CONSTANT);
			}
		}
		
		// check if this is an identifier
		if (maybeIdentifier) {
			if (isIdentifierChar(input.peek(), 1)) {
				continue; // keep processing 
			} else {
				// we've got ourselves a constant
				return pushToken(curToken, TT_IDENTIFIER);
			}
		}
		
	}
	return pushToken(curToken, TT_EOF);
}

// look at the current token
bool Tokenizer::peek(TokenType expected) {
	return currentToken->getType() == expected;
}

// look at the current token
bool Tokenizer::peek(TokenFlag expected) {
	return currentToken->hasAttribute(expected);
}

// look at the current token and skip if it matches
bool Tokenizer::check(TokenType expected) {
	if (currentToken->getType() == expected) {
		nextToken();
		return true;
	}
	return false;
}

// look at the current token and skip if it matches
bool Tokenizer::check(TokenFlag expected) {
	if (currentToken->hasAttribute(expected)) {
		nextToken();
		return true;
	}
	return false;
}

void Tokenizer::pushToken(const string &text, TokenType type, TokenFlag flags) {
	previousToken = currentToken;
	
	// if the token has already been referred to, just return away
	if (type == TT_IDENTIFIER) {
		// check if the symbol was found
		currentToken = findSymbol(text);
		if (currentToken != NULL) return;
	}
	
	// it wasn't found, so add it to the table
	if (type == TT_BOOL || 
		type == TT_BYTE || 
		type == TT_SHORT || 
		type == TT_LONG) flags = (TokenFlag)(TF_SCALAR_TYPE | flags);
	else if (type == TT_PLUS || 
		type == TT_MINUS || 
		type == TT_MULTIPLY || 
		type == TT_DIVIDE || 
		type == TT_MOD || 
		type == TT_LESS_THAN || 
		type == TT_GREATER_THAN || 
		type == TT_LESS_THAN_OR_EQUAL || 
		type == TT_GREATER_THAN_OR_EQUAL || 
		type == TT_EQUAL || 
		type == TT_NOT_EQUAL || 
		type == TT_AND || 
		type == TT_OR) flags = (TokenFlag)(TF_BINARY_OPERATOR | flags);
	else if (type == TT_NEG || 
		type == TT_NOT) flags = (TokenFlag)(TF_UNARY_OPERATOR | flags);
	else if (type == TT_INC || 
		type == TT_DEC) flags = (TokenFlag)(TF_INCDEC_OPERATOR | flags);
	else if (type == TT_CONSTANT || 
		type == TT_TRUE || 
		type == TT_FALSE) flags = (TokenFlag)(TF_BOOL_CONSTANT | flags);
	
	// add the new token to the symbol table
	unique_ptr<Token> thisToken(new Token(text, type, flags));
	symbols.push_front(move(thisToken));
	currentToken = &(**symbols.begin());
	
	LOG << "  Token: " << "\"" << text << "\"" << " of type:" << type << " flag:" << flags << " @ " << position.toString() << endl;
}

Token * Tokenizer::findSymbol(string name) {
	for (auto it = symbols.begin(); it != symbols.end(); it++) {
		if ((*it)->getType() == TT_IDENTIFIER && (*it)->getText() == name) return &(**it);
	}
	return NULL;
}
