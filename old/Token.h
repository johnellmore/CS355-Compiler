// Dwayne Towell -- CS 355 Compiler Construction

// types of tokens in Q
enum TokenType { 
	TT_INVALID,
	TT_EOF,

	TT_BOOL, 
	TT_BYTE, 
	TT_SHORT, 
	TT_LONG,

	TT_PLUS,
	TT_MINUS,
	TT_MULTIPLY,
	TT_DIVIDE,
	TT_MOD,
	TT_LESS_THAN,
	TT_LESS_THAN_OR_EQUAL,
	TT_GREATER_THAN,
	TT_GREATER_THAN_OR_EQUAL,
	TT_EQUAL,
	TT_NOT_EQUAL,
	TT_AND,
	TT_OR,

	TT_NOT,
	TT_NEG,
	TT_INC,
	TT_DEC,

	TT_TRUE,
	TT_FALSE,

	TT_FUNCTION,
	TT_VARS,
	TT_DO,
	TT_RETURN,
	TT_END,
	TT_SET,
	TT_IF,
	TT_ELSE,
	TT_OPEN_COMPOUND,
	TT_CLOSE_COMPOUND,
	TT_OPEN_ARRAY,
	TT_CLOSE_ARRAY,
	TT_COLON,
	TT_SEMICOLON,
	TT_CARET,
	TT_BANG,

	TT_CONSTANT,
	TT_IDENTIFIER
};

// possible attributes of tokens in Q
enum TokenFlag {
	TF_NONE             = 0,
	TF_SCALAR_TYPE      = 1,
	TF_BINARY_OPERATOR  = 2,
	TF_UNARY_OPERATOR   = 4,
	TF_INCDEC_OPERATOR  = 8,
	TF_BOOL_CONSTANT    = 16,
	TF_FUNCTION         = 32
	// ...
};

// possible data types in Q
enum ScalarType { 
	ST_INVALID,
	ST_BOOL, 
	ST_BYTE, 
	ST_SHORT, 
	ST_LONG
};

// Value-semantic object representing a token in Q.
// Exactly one of these is created for each unique token,
// even if the token appears multiple times in a source file.
class Token {
public:
	Token() : text(""), type(TT_INVALID), flags(TF_NONE) {};
	Token(const string &ntext, TokenType ntype, TokenFlag nflags=TF_NONE);
	void operator=(const Token &n);
	
	TokenType			getType()       const { return type; }
	const string &		getText()       const { return text; }
	int					getFlags()      const { return (int)flags; }
	
	bool				hasAttribute(TokenFlag bit) const { return flags & bit; }
	
	void				setAsFunction() { flags = (TokenFlag)(flags | TF_FUNCTION); }
	
	int					getValue()      const;
	ScalarType			getScalarType() const;
	bool				getBoolValue()  const;

private:
	string				text;
	TokenType			type;
	TokenFlag			flags;
};

/* 
 * The following allow tracing your program using stream output syntax.
 * For example,
 * 
 * LOG << "token: " << token->type << "," << token->flags << endl;
 * 
 * When VERBOSE is defined (g++ -DVERBOSE=1 ...) application will log 
 * trace output to standard out.
 * 
 * When VERBOSE is NOT defined, not output will be generated or evaluated.      
 */
#ifdef VERBOSE
    #define LOG cout
#else
    #include <iostream>
    // create a handy "stream" that does nothing, to consume logging expressions
    struct nullstream : ostream { nullstream() : ios(0), ostream(0) {} };
    static nullstream logstream;
    #define LOG if(0) logstream
#endif
