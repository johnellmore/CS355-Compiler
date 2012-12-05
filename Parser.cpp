#include "Compiler.h"

bool Parser::parse(Tokenizer *t) {
	tokenizer = t;
	unique_ptr<Module> thisModule(new Module(tokenizer->currentPosition()));
	module = move(parseModule(move(thisModule)));
	if (module == NULL) return false;
	return true;
}

void Parser::printError(string msg) {
	cerr << "Error @ " << tokenizer->currentPosition().toString() << ": " << msg << endl;
}

unique_ptr<Module> Parser::parseModule(unique_ptr<Module> module) {
	LOG << "parsing module" << endl;
	if (tokenizer->check(TT_END)) {
		if (tokenizer->peek(TT_EOF)) return module;
		else {
			printError("eof expected");
			return NULL;
		}
	}
	
	// parse module vars
	if (tokenizer->check(TT_VARS)) {
		// build declaration list
		unique_ptr<DeclList> dl(&module->vars);
		dl = parseDeclarationList(move(dl));
		if (dl == NULL) return NULL;
		dl.release();
		
		if (tokenizer->check(TT_END))
			return parseModule(move(module));
		else {
			printError("expected end of module variable declarations");
			return NULL;
		}
	}
	
	// parse module functions
	if (tokenizer->check(TT_FUNCTION)) {
		auto parsedFuncU = parseFunctionSignature();
		Func* moduleFunc = parsedFuncU.release();
		LOG << "  Finished function signature parsing" << endl;
		if (moduleFunc == NULL) return NULL;
		
		bool isDeclared = false;
		LOG << "  Checking for function existence... ";
		// see if we can find this function in the list already
		for (auto f = module->funcs.begin(); f != module->funcs.end(); f++) {
			if (&**f == moduleFunc) { // compare the AST* of each module function to the current function
				LOG << "found a matching function call!" << endl;
				isDeclared = true;
				break;
			}
		}
		// if it's a new function (not declared before), add it to the module
		if (!isDeclared) {
			unique_ptr<Func> moduleFuncP(moduleFunc);
			module->funcs.push_back(move(moduleFuncP));
		}
		
		unique_ptr<Func> moduleFuncP(moduleFunc);
		moduleFuncP = parseFunctionBody(move(moduleFuncP));
		if (moduleFuncP == NULL) {
			return NULL;
		}
		moduleFuncP.release();
		
		return parseModule(move(module));
	}
	
	printError("module expected");
	return NULL;
}

unique_ptr<Func> Parser::parseFunctionSignature() {
	LOG << "parsing function signature" << endl;
	
	if (tokenizer->check(TT_IDENTIFIER)) {
		auto lastToken = tokenizer->lastToken();
		if (lastToken->hasAttribute(TF_VARIABLE)) {
			printError("identifier \"" + lastToken->getText() + "\" already declared as variable");
			return NULL;
		}
		bool isDefined = lastToken->hasAttribute(TF_FUNCTION);
		
		// set the token as a function identifier
		lastToken->setAsFunction();
		
		if (tokenizer->check(TT_COLON)) {
			unique_ptr<Func> func = NULL;
			auto type = parseScalarType();
			if (type == ST_INVALID) return NULL;
			
			unique_ptr<DeclList> dl(new DeclList());
			if (tokenizer->peek(TT_IDENTIFIER)) {
				// a declaration list of params is coming - this is optional
				dl = parseDeclarationList(move(dl));
				if (dl == NULL) return NULL;
			}
			
			if (!isDefined) {
				func.reset(new Func(tokenizer->currentPosition(), lastToken, type, move(dl)));
				lastToken->setDeclaration(&*func);
			} else {
				func.reset((Func*)lastToken->getDeclaration());
			}
			return func;
		}
		printError("expected colon after identifier in function signature");
		return NULL;
	}
	
	printError("function signature did not start with an identifier");
	return NULL;
}

unique_ptr<Func> Parser::parseFunctionBody(unique_ptr<Func> f) {
	LOG << "parsing function body" << endl;
	
	unique_ptr<DeclList> vars(new DeclList());
	unique_ptr<StmtList> stmts(new StmtList());
	bool hasLocalVars = false;
	
	// end
	if (tokenizer->check(TT_END)) return f;
	
	// vars DL ...
	if (tokenizer->check(TT_VARS)) {
		hasLocalVars = true;
		vars = parseDeclarationList(move(vars));
		if (vars == NULL) {
			f.release();
			return NULL;
		}
	}
	
	// do SL ...
	if (tokenizer->check(TT_DO)) {
		stmts = parseStatementList(move(stmts));
		if (stmts == NULL) {
			f.release();
			return NULL;
		}
	} else if (hasLocalVars) {
		printError("cannot have local function vars with no function statements");
		f.release();
		return NULL;
	}
	
	// return E
	if (tokenizer->check(TT_RETURN)) {
		unique_ptr<Expr> ret(parseExpression());
		if (ret == NULL) {
			f.release();
			return NULL;
		}
		
		// define the function
		f->define(move(vars), move(stmts), move(ret));
		return f;
	}
	
	printError("function vars, do statement list, return, or end expected");
	f.release();
	return NULL;
}

unique_ptr<DeclList> Parser::parseDeclarationList(unique_ptr<DeclList> dl) {
	LOG << "parsing declaration list" << endl;
	
	auto varDecl = parseVariableDeclaration();
	if (varDecl == NULL) {
		dl.release();
		return NULL;
	}
	dl->push_back(move(varDecl));
	
	if (tokenizer->check(TT_SEMICOLON)) {
		dl = parseDeclarationList(move(dl));
	}
	return dl;
}

unique_ptr<VarDecl> Parser::parseVariableDeclaration() {
	LOG << "parsing variable declaration" << endl;
	
	if (!tokenizer->check(TT_IDENTIFIER)) {
		printError("variable declaration missing starting identifier");
		return NULL;
	}
	auto thisVar = tokenizer->lastToken();
	if (thisVar->hasAttribute(TF_FUNCTION)) {
		printError("variable previously declared as a function");
		return NULL;
	}
	
	if (!tokenizer->check(TT_COLON)) {
		printError("variable declaration missing colon");
		return NULL;
	}
	auto type = parseType();
	if (type == NULL) return NULL;
	
	thisVar->setAsVariable(type->size);
	unique_ptr<VarDecl> var(new VarDecl(tokenizer->currentPosition(), thisVar, move(type)));
	thisVar->setDeclaration(&*var);
	return var;
}

unique_ptr<Type> Parser::parseType() {
	LOG << "parsing type" << endl;
	
	auto scalarType = parseScalarType();
	if (scalarType == ST_INVALID) return NULL;
	int size = 0;
	
	// check if this is an array declaration
	if (tokenizer->check(TT_OPEN_ARRAY)) {
		if (!tokenizer->check(TT_CONSTANT)) {
			printError("expected constant for array length");
			return NULL;
		}
		
		// get size number
		auto constantToken = tokenizer->lastToken();
		stringstream sizeStr(constantToken->getText());
		sizeStr >> size;
		if (size < 1) {
			printError("invalid size for array");
			return NULL;
		}
		
		// make sure there are closing parentheses
		if (!tokenizer->check(TT_CLOSE_ARRAY)) {
			printError("missing array length closing bracket");
			return NULL;
		}
	}
	
	unique_ptr<Type> type(new Type(tokenizer->currentPosition(), scalarType, size));
	return type;
}

ScalarType Parser::parseScalarType() {
	LOG << "parsing scalar type" << endl;
	
	if (tokenizer->check(TT_BOOL)) return ST_BOOL;
	if (tokenizer->check(TT_BYTE)) return ST_BYTE;
	if (tokenizer->check(TT_SHORT)) return ST_SHORT;
	if (tokenizer->check(TT_LONG)) return ST_LONG;
	
	printError("invalid scalar type");
	return ST_INVALID;
}

unique_ptr<Expr> Parser::parseExpression() {
	LOG << "parsing expression" << endl;
	
	// all binary operators
	if (tokenizer->check(TF_BINARY_OPERATOR)) {
		Token * last = tokenizer->lastToken();
		
		unique_ptr<Expr> l(parseExpression());
		if (l == NULL) return NULL;
		
		unique_ptr<Expr> r(parseExpression());
		if (r == NULL) return NULL;
		
		unique_ptr<Expr> expr(new BinaryExpr(tokenizer->currentPosition(), last->getType(), move(l), move(r)));
		return expr;
	}
	
	// all unary operators
	if (tokenizer->check(TF_UNARY_OPERATOR)) {
		Token * oper = tokenizer->lastToken();
		
		unique_ptr<Expr> v(parseExpression());
		if (v == NULL) return NULL;
		
		unique_ptr<Expr> e(new UnaryExpr(tokenizer->currentPosition(), oper->getType(), move(v)));
		return e;
	}
	
	// increment and decrement
	if (tokenizer->check(TF_INCDEC_OPERATOR)) {
		Token * oper = tokenizer->lastToken();
		
		unique_ptr<LValue> lv(parseLValue());
		if (lv == NULL) return NULL;
		
		unique_ptr<Expr> e(new IncDecExpr(tokenizer->currentPosition(), oper->getType(), move(lv)));
		return e;
	}
	
	// single things (constants, bools, l-values)
	if (tokenizer->check(TF_BOOL_CONSTANT)) {
		Token * last = tokenizer->lastToken();
		unique_ptr<Expr> c(new Constant(tokenizer->currentPosition(), last->getValue(), last->getScalarType()));
		last->setDeclaration(&*c);
		return c;
	}
	
	// l-values and function calls
	if (tokenizer->peek(TT_IDENTIFIER)) {
		if (tokenizer->check(TF_FUNCTION)) {
			// a function call
			Token * name = tokenizer->lastToken();
			
			unique_ptr<ExprList> pl(new ExprList());
			pl = parseParameterList(move(pl));
			if (pl == NULL) return NULL;
			
			unique_ptr<Expr> f(new FuncCall(tokenizer->currentPosition(), name, move(pl)));
			return f;
		} else {
			// an l-value
			return parseLValue();
		}
	}
	
	printError("expression expected");
	return NULL;
}

unique_ptr<LValue> Parser::parseLValue() {
	LOG << "parsing l-value" << endl;
	
	if (tokenizer->peek(TT_IDENTIFIER)) {
		if (!tokenizer->check(TF_VARIABLE)) {
			printError("expected variable identifier");
			return NULL;
		}
		
		Token * ident = tokenizer->lastToken();
		unique_ptr<Expr> e;
		if (tokenizer->check(TT_OPEN_ARRAY)) {
			// there's an array index here
			e = parseExpression();
			if (e == NULL) return NULL;
			if (!tokenizer->check(TT_CLOSE_ARRAY)) {
				printError("expected array close bracket");
				return NULL;
			}
		} else {
			e.reset(new Constant(tokenizer->currentPosition(), 0, ST_LONG));
		}
		unique_ptr<LValue> l(new LValue(tokenizer->currentPosition(), ident, move(e)));
		return l;
	}
	printError("expected identifier");
	return NULL;
}

unique_ptr<ExprList> Parser::parseParameterList(unique_ptr<ExprList> pl) {
	LOG << "parsing parameter list" << endl;
	
	if (tokenizer->check(TT_BANG)) return pl;
	
	// parse the current expression
	unique_ptr<Expr> e = parseExpression();
	if (e == NULL) {
		pl.release();
		return NULL;
	}
	pl->push_back(move(e));
	
	// parse more items
	pl = parseParameterList(move(pl));
	if (pl == NULL) {
		pl.release();
		return NULL;
	}
	
	return pl;
}

unique_ptr<StmtList> Parser::parseStatementList(unique_ptr<StmtList> sl) {
	LOG << "parsing statement list" << endl;
	
	unique_ptr<Stmt> s(parseStatement());
	if (s == NULL) return NULL;
	
	// add statement to statement list
	sl->push_back(move(s));
	
	if (tokenizer->check(TT_SEMICOLON)) {
		sl = parseStatementList(move(sl));
		if (sl == NULL) return NULL;
		return sl;
	}
	return sl;
}

unique_ptr<Stmt> Parser::parseStatement() {
	LOG << "parsing statement" << endl;
	
	// set LV E
	if (tokenizer->check(TT_SET)) {
		// get the l-value
		unique_ptr<LValue> lv(parseLValue());
		if (lv == NULL) return NULL;
		
		// get the expression
		unique_ptr<Expr> e(parseExpression());
		if (e == NULL) return NULL;
		
		unique_ptr<Stmt> s(new AssignmentStmt(tokenizer->currentPosition(), move(lv), move(e)));
		return s;
	}
	
	// { SL } ...
	if (tokenizer->check(TT_OPEN_COMPOUND)) {
		unique_ptr<StmtList> sl(new StmtList());
		sl = parseStatementList(move(sl));
		if (sl == NULL) return NULL;
		
		if (!tokenizer->check(TT_CLOSE_COMPOUND)) {
			printError("missing if statement close compound bracket");
			return NULL;
		}
		
		// { SL } ^ E
		if (tokenizer->check(TT_CARET)) {
			// a loop statement
			unique_ptr<Expr> condition(parseExpression());
			if (condition == NULL) return NULL;
			
			unique_ptr<Stmt> loop(new LoopStmt(tokenizer->currentPosition(), move(condition), move(sl)));
			return loop;
			
		} else if (tokenizer->check(TT_IF)) {
			// { SL } if E [...]
			unique_ptr<Expr> condition(parseExpression());
			if (condition == NULL) return NULL;
			
			// { SL } if E else { SL }
			unique_ptr<StmtList> es(new StmtList());
			if (tokenizer->check(TT_ELSE)) {
				if (!tokenizer->check(TT_OPEN_COMPOUND)) {
					printError("missing else clause open compound bracket");
					return NULL;
				}
				
				es.reset(new StmtList());
				es = parseStatementList(move(es));
				if (es == NULL) return NULL;
				
				if (!tokenizer->check(TT_CLOSE_COMPOUND)) {
					printError("missing else clause closing compound bracket");
					return NULL;
				}
			}
			
			unique_ptr<Stmt> s(new ConditionalStmt(tokenizer->currentPosition(), move(condition), move(sl), move(es)));
			return s;
		} else {
			printError("expected loop or if statement");
			return NULL;
		}
	}
	
	// E
	unique_ptr<Expr> expr(parseExpression());
	if (expr == NULL) return NULL;
	unique_ptr<Stmt> exprSt(new ExprStmt(tokenizer->currentPosition(), move(expr)));
	return exprSt;
}
