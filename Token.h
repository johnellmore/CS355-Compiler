// Dwayne Towell -- CS 355 Compiler Construction

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

enum TokenFlag {
	TF_NONE             = 0,
	TF_SCALAR_TYPE      = 1,
	TF_BINARY_OPERATOR  = 2,
	TF_UNARY_OPERATOR   = 4,
	TF_INCDEC_OPERATOR  = 8,
	TF_BOOL_CONSTANT    = 16,
	
	TF_VARIABLE         = 32,
	TF_ARRAY            = 64,
	TF_LOCAL            = 128,
	TF_FUNCTION         = 256,
	// ...
};

enum ScalarType { 
	ST_INVALID,
	ST_BOOL, 
	ST_BYTE, 
	ST_SHORT, 
	ST_LONG
};

// forward
class AST;

class Token {
public:
	Token(const string &text,TokenType type);
	Token(const string &text,TokenType type,TokenFlag flags);
	
	TokenType			getType()		const { return type; }
	const string &		getText()		const { return text; }
	
	bool				hasAttribute(TokenFlag bit) const { return flags & bit; }
	
	int					getValue()		const;
	ScalarType			getScalarType()	const;
	bool				getBoolValue()	const;
	int					getArraySize()	const;
	
	void				setAsFunction();
	void				setAsVariable(int size);
	
	void				setDeclaration(AST * decl) { declaration = decl; };
	AST *				getDeclaration() { return declaration; };
	
private:
	string				text;
	TokenType			type;
	TokenFlag			flags;
	AST *				declaration;
};

#ifdef VERBOSE
	#define LOG cout
#else
	#include <iostream>
	// create a handy "stream" that does nothing, to consume logging expressions
	struct nullstream : ostream { nullstream() : ios(0), ostream(0) {} };
	static nullstream logstream;
	#define LOG if(0) logstream
#endif
