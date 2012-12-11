// Dwayne Towell -- CS 355 Compiler Construction

struct Position {
    Position() : lineNumber(0), character(0) {}
	Position(int l, int c) : lineNumber(l), character(c) {}
	Position(const Position &p) : lineNumber(p.lineNumber), character(p.character) {}
	string toString() const;
	int lineNumber;
	int character;
};

typedef list<unique_ptr<Token>> TokenList;

class Tokenizer {
public:
	Tokenizer(istream &ninput,ostream &errors);
	
	void			nextToken();
	
	// look at the current token only
	bool			peek(TokenType expected);
	bool			peek(TokenFlag expected);
	
	// look at the current token and skip if it matches
	bool			check(TokenType expected);
	bool			check(TokenFlag expected);
	
	// scoping 
	unsigned int	getScope() const { return curScope; };
	unsigned int	focusScope() { curScope = nextScope++; return curScope; };
	void			blurScope() { curScope = 0; }
	
	Token *			lastToken()         const { assert(previousToken); return previousToken; }
	
	const Position &currentPosition()   const { return position; }
	void			debugSymbols();
	
private:
	istream &		input;
	ostream &		errors;
	TokenList		symbols;
	
	static bool		isKeyword(const string test);
	static bool		isWhitespace(const char test);
	static bool		isIdentifierChar(const char test, const int pos);
	static bool 	isConstantChar(const char test);
	
	void 			pushToken(const string &text, TokenType type, TokenFlag flags=TF_NONE);
	void 			pushSymbol(string name);
	Token *			findSymbol(string name);
	
	Position		position;
	string			lineBuffer;
	unsigned int	curScope;
	unsigned int	nextScope;
	
	Token *			currentToken;
	Token *			previousToken;
};
