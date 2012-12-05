// Value-semantic object specifying a location in a source file.
struct Position {
	Position() : lineNumber(0), character(0) {}
	Position(int l, int c) : lineNumber(l), character(c) {}
	Position(const Position &p) : lineNumber(p.lineNumber), character(p.character) {}
	string toString() const;
	int lineNumber;
	int character;
};

typedef list<Token> TokenList;

// Process to convert text input into tokens. 
class Tokenizer {
public:
	
	Tokenizer(istream &ninput);
	
	// move to next token
	void			nextToken();
	
	// look at the current token
	bool			peek(TokenType expected);
	bool			peek(TokenFlag expected);
	
	// look at the prev token and skip if it matches
	bool			check(TokenType expected);
	bool			check(TokenFlag expected);
	
	// look at the current token
	bool			test(TokenType expected);
	
	// get the last (previous) token
	Token *	lastToken();
	
	void setLastAsFunction();
	bool isExistingFunction();
	bool isExistingSymbol();
	
	// get the location of the current token
	const Position &currentPosition()   const;
	
	void dumpSymbols() {
		cout << "DUMPING SYMBOLS" << endl;
		for (TokenList::iterator it = symbols.begin(); it != symbols.end(); it++)
			cout << "\t" << it->getText().substr(0, 7) << "\t" << it->getType() << "\t" << it->getFlags() << endl;
		cout << "END DUMP" << endl;
	}
	
private:
	istream * input;
	Position nextPos;
	Position curPos;
	
	TokenList symbols;
	
	static bool isKeyword(const string test);
	static bool isWhitespace(const char test);
	static bool isIdentifierChar(const char test, const int pos);
	static bool isConstantChar(const char test);
	
	void pushToken(const string &text, TokenType type, TokenFlag flags=TF_NONE);
	
	static bool Tokenizer::areTokensSameText(const Token &test1, const Token &test2);
	static bool Tokenizer::areTokensExactSame(const Token &test1, const Token &test2);
	
	// disallow these
	Tokenizer(const Tokenizer &);
	void operator=(const Tokenizer &);
};
