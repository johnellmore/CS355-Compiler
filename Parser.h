// Dwayne Towell -- CS 355 Compiler Construction

class Parser {
public:
	bool					parse(Tokenizer *tokenizer);
	Module * 				getModule() { return module.get(); }
	
private:
	unique_ptr<Module>		parseModule(unique_ptr<Module> module);
	unique_ptr<Func>		parseFunctionSignature();
	unique_ptr<Func>		parseFunctionBody(unique_ptr<Func> f);
	unique_ptr<DeclList>	parseDeclarationList(unique_ptr<DeclList> dl);
	unique_ptr<VarDecl>		parseVariableDeclaration();
	unique_ptr<Type>		parseType();
	ScalarType				parseScalarType();
	unique_ptr<Expr>		parseExpression();
	unique_ptr<LValue>		parseLValue();
	unique_ptr<ExprList>	parseParameterList(unique_ptr<ExprList> pl);
	unique_ptr<StmtList>	parseStatementList(unique_ptr<StmtList> sl);
	unique_ptr<Stmt>		parseStatement();
	
	void					printError(string msg);
	
	Tokenizer *				tokenizer;
	unique_ptr<Module>		module;
};
