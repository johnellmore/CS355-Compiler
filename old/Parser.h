
class Parser {
public:
	bool parse(Tokenizer *t);
	
private:
	bool printError(string msg); // always return false
	
	bool parseModule();
	bool parseFunctionSignature();
	bool parseFunctionBody();
	bool parseDeclarationList();
	bool parseVariableDeclaration();
	bool parseType();
	bool parseScalarType();
	bool parseExpression();
	bool parseLValue();
	bool parseParameterList();
	bool parseStatementList();
	bool parseStatement();
	
	Tokenizer *tokenizer;
};
