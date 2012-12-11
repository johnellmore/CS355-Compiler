#include "Compiler.h"

static string oper[] = {"+","-","*","/","%","<","<=",">",">=","=","<>","and","or","not","neg","++","--"};
static string t[] = {"INVALID","bool","byte","short","long"};

void TypeChecker::printError(AST * node, string msg) {
	cerr << "Type Error @ " << node->location().toString() << ": " << msg << endl;
}

string TypeChecker::debugStr(AST * node) {
	return "  Type Debug @ " + node->location().toString() + ": ";
}

string TypeChecker::applyType(Type *ty) {
	LOG << debugStr(ty) << "type " << t[ty->type] << " of size " << ty->size << endl;
	assert(ty->type != ST_INVALID);
	
	return "";
}

string TypeChecker::applyModule(Module *module,const StringList &vars,const StringList &funcs) {
	return concatenate(vars)+concatenate(funcs); // don't need no stinkin type checking
}

string TypeChecker::applyVarDecl(VarDecl *vd,string type) {
	return type; // don't need no stinkin type checking
}

string TypeChecker::applyFunc(Func *func,const StringList &params,const StringList &vars,const StringList &stmts,string expr) {
	if ((concatenate(params)+concatenate(vars)+concatenate(stmts)+expr).size()) return "error";
	
	auto funcType = func->type;
	auto funcName = func->name->getText();
	LOG << debugStr(func) << "function " << funcName << " has type " << t[funcType] << endl;
	
	// check that the function has been defined (has a body)
	if (func->returnExpr == NULL) {
		printError(func, "function " + func->name->getText() + " was never defined");
		return "error";
	}
	
	// check that the return value is the correct type
	auto returnType = func->returnExpr->getType();
	if (funcType == returnType) {
		// already correct
		return "";
	} else if (func->returnExpr->convertibleTo(funcType)) {
		// return type is wrong, but is convertible
		LOG << "    Converting types on function return expression" << endl;
		unique_ptr<Expr> tc(new TypeConversion(funcType, move(func->returnExpr)));
		func->returnExpr.swap(tc);
		tc.release();
		return "";
	}
	
	// incompatible types
	printError(func, "return expression type of " + funcName + " doesn't match function signature");
	return "error";
}

string TypeChecker::applyExprStmt(ExprStmt *es,string expr) {
	return expr; // don't need no stinkin type checking
}

string TypeChecker::applyAssignmentStmt(AssignmentStmt *as,string lvalue,string expr) {
	if ((lvalue+expr).size()) return "error";
	
	// ensure that neither operand is an entire array
	if (as->lvalue->name->hasAttribute(TF_ARRAY) && as->lvalue->index == NULL) {
		printError(as, "assignment statement cannot have full array as lvalue");
		return "error";
	} else if (checkIsFullArray(&*as->expr)) {
		printError(as, "assignment statement cannot have full array as rvalue");
		return "error";
	}
	
	// check that expr and lvalue are compatible
	auto lvType = as->lvalue->getType();
	auto exprType = as->expr->getType();
	LOG << debugStr(as) << "assigning " << t[exprType] << " into " << t[lvType] << endl;
	if (as->expr->getType() == lvType) {
		// already correct
		return "";
	} else if (as->expr->convertibleTo(lvType)) {
		// wrong type, but is convertible
		LOG << "    Converting types on assignment statement" << endl;
		unique_ptr<Expr> tc(new TypeConversion(lvType, move(as->expr)));
		as->expr.swap(tc);
		tc.release();
		return "";
	}
	
	// incompatible types
	printError(as, "invalid type on assignment statement");
	return "error";
}

string TypeChecker::applyLoopStmt(LoopStmt *ls,const StringList &stmts,string expr) {
	return concatenate(stmts)+expr; // don't need no stinkin type checking
}

string TypeChecker::applyConditionalStmt(ConditionalStmt *cs,const StringList &thens,const StringList &elses,string expr) {
	if ((concatenate(thens)+concatenate(elses)+expr).size()) return "error";
	
	// check that the condition is of type bool
	auto condType = cs->conditionalExpr->getType();
	LOG << debugStr(cs) << "if statement condition of type " << t[condType] << endl;
	if (condType != ST_BOOL) {
		printError(cs, "if statement condition must be a boolean value");
		return "error";
	}
	return "";
}

string TypeChecker::applyUnaryExpr(UnaryExpr *ue,string expr) {
	if (expr.size()) return "error";
	
	// check that not and neg operate on appropriate expressions
	auto exprType = ue->expr->getType();
	LOG << debugStr(ue) << "unary " << oper[ue->oper-TT_PLUS] << " operating on " << t[exprType] << endl;
	
	if (ue->oper == TT_NEG && exprType <= ST_BOOL) {
		printError(ue, "unary neg must operate on numeric types");
		return "error";
	} else if (ue->oper == TT_NOT && exprType != ST_BOOL) {
		printError(ue, "unary not must operate on booleans");
		return "error";
	}
	
	// ensure that operand is not entire array
	if (checkIsFullArray(&*ue->expr)) {
		printError(ue, "unary " + string(oper[ue->oper-TT_PLUS]) + " operand cannot be entire array");
		return "error";
	}
	
	// everything else is okay
	return "";
}

string TypeChecker::applyBinaryExpr(BinaryExpr *be,string leftExpr,string rightExpr) {
	if ((leftExpr+rightExpr).size()) return "error";
	
	// check that left and right operands are compatible types
	auto leftType = be->leftExpr->getType();
	auto rightType = be->rightExpr->getType();
	
	LOG << debugStr(be) << "binary " << oper[be->oper-TT_PLUS] << " " << t[leftType] << " and " << t[rightType] << endl;
	
	// ensure that neither operand is an entire array
	if (checkIsFullArray(&*be->leftExpr)) {
		printError(be, "binary " + string(oper[be->oper-TT_PLUS]) + " left operand cannot be entire array");
		return "error";
	} else if (checkIsFullArray(&*be->rightExpr)) {
		printError(be, "binary " + string(oper[be->oper-TT_PLUS]) + " right operand cannot be entire array");
		return "error";
	}
	
	if (leftType == rightType) {
		// already correct
		return "";
	} else if (be->leftExpr->convertibleTo(rightType)) {
		// left needs to be converted
		LOG << "    Converting left operand in binary expression" << endl;
		unique_ptr<Expr> tc(new TypeConversion(rightType, move(be->leftExpr)));
		be->leftExpr.swap(tc);
		tc.release();
		return "";
	} else if (be->rightExpr->convertibleTo(leftType)) {
		// right needs to be converted
		LOG << "    Converting right operand in binary expression" << endl;
		unique_ptr<Expr> tc(new TypeConversion(leftType, move(be->rightExpr)));
		be->rightExpr.swap(tc);
		tc.release();
		return "";
	}
	
	// incompatible types
	printError(be, "type mismatch in binary expression");
	return "error";
}

string TypeChecker::applyConstant(Constant *c) {
	return ""; // don't need no stinkin type checking
}

string TypeChecker::applyLValue(LValue *lv,string index) {
	if (index.size()) return "error";
	
	// ensure that the lvalue is not a function
	assert(lv->name->hasAttribute(TF_VARIABLE));
	
	// if there is an index, check that it's numeric
	if (lv->index == NULL) {
		LOG << debugStr(lv) << "lvalue " << lv->name->getText() << " with no index" << endl;
	} else {
		auto indexType = lv->index->getType();
		LOG << debugStr(lv) << "lvalue " << lv->name->getText() << " has index type " << t[indexType] << endl;
		if (indexType <= ST_BOOL) {
			printError(lv, "lvalue index must be a numeric type");
			return "error";
		}
	}
	
	return "";
}

string TypeChecker::applyIncDecExpr(IncDecExpr *id,string expr) {
	if (expr.size()) return "error";
	
	// check that index is not a bool
	auto lvType = id->lvalue->getType();
	LOG << debugStr(id) << "incdec operating on " << t[lvType] << endl;
	if (lvType <= ST_BOOL) {
		printError(id, "incdec must operate on a numeric type");
		return "error";
	}
	
	// ensure that operand is not entire array
	if (id->lvalue->name->hasAttribute(TF_ARRAY) && id->lvalue->index == NULL) {
		printError(id, "incdec operand cannot be entire array");
		return "error";
	}
	
	return "";
}

string TypeChecker::applyFuncCall(FuncCall *fc,const StringList &params) {
	if ((concatenate(params)).size()) return "error";
	
	// setup for future checks
	assert(fc->name->hasAttribute(TF_FUNCTION));
	auto funcName = fc->name->getText();
	LOG << debugStr(fc) << "funccall " << funcName << " with " << fc->params.size() << " params" << endl;
	Func *f = dynamic_cast<Func *>(fc->name->getDeclaration());
	assert(f != NULL);
	
	// check the num of params in the FuncCall vs. actual Function
	int fNumParams = f->params.size();
	int fcNumParams = fc->params.size();
	if (fcNumParams > fNumParams) {
		printError(fc, "function call " + funcName + " has too many parameters");
		return "error";
	} else if (fcNumParams < fNumParams) {
		printError(fc, "function call " + funcName + " has too few parameters");
		return "error";
	}
	
	// check that the function call parameters match the types of the actual function
	int p = 1;
	list<unique_ptr<VarDecl>>::iterator fp = f->params.begin();
	auto fcp = fc->params.begin();
	while (fcp != fc->params.end()) {
		LOG << "    checking param #" << p << " of " << funcName << endl;
		
		// check param type
		auto funcParamType = (*fp)->type->type;
		auto callParamType = (*fcp)->getType();
		bool isFuncParamArray = ((*fp)->type->size > 0);
		bool isCallParamArray = checkIsFullArray(&**fcp);
		if (funcParamType != callParamType) {
			// we have a type mismatch
			if ((*fcp)->convertibleTo(funcParamType)) {
				// callParam is convertible
				// ensure callParam is not an array
				if (isCallParamArray) {
					printError(fc, funcName + " param #" + string(1, p+48) + " needs type conversion, but cannot be converted because it is an array");
					return "error";
				}
				
				// passed in param needs to be converted
				LOG << "      converting " << funcName << " param #" << p << endl;
				fcp->reset(new TypeConversion(funcParamType, move(*fcp)));
			} else {
				// the two params cannot be reconciled
				printError(fc, "cannot convert " + funcName + " param #" + string(1, p+48) + " from " + string(t[callParamType]) + " to " + string(t[funcParamType]));
				return "error";
			}
		}
		// the types of the params are now correct
		
		// make sure either both are arrays or both are not
		if (isFuncParamArray && !isCallParamArray) {
			// call param needs to be array
			printError(fc, funcName + " param #" + string(1, p+48) + " must be an array");
			return "error";
		} else if (!isFuncParamArray && isCallParamArray) {
			// call param should not be an array
			printError(fc, "cannot pass an array as parameter to non-array type in " + funcName + " param #" + string(1, p+48));
			return "error";
		} else if (isFuncParamArray && isCallParamArray) {
			// both are arrays (correct)
			// check if param size is correct
			if ((*fp)->type->size != dynamic_cast<LValue*>(&**fcp)->name->getArraySize()) {
				// array sizes do not match
				printError(fc, funcName + " param #" + string(1, p+48) + " is the wrong size array");
				return "error";
			}
		}
		// final case is already okay, no else block needed
		
		// increment other counters
		p++;
		fp++;
		fcp++;
	}
	
	return "";
}

string TypeChecker::applyTypeConversion(TypeConversion *tc, string expr) {
	// never executed - TypeConversion objects are only created after a node's been visited
	return "";
}

bool TypeChecker::checkIsFullArray(Expr *e) {
	auto lve = dynamic_cast<LValue*>(e);
	return (lve != NULL && lve->name->hasAttribute(TF_ARRAY) && lve->index == NULL);
}
