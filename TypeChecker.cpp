#include "Compiler.h"

static string oper[] = {"+","-","*","/","%","<","<=",">",">=","=","<>","and","or","not","neg","++","--"};
static string t[] = {"INVALID","bool","byte","short","long"};

void TypeChecker::printError(AST * node, string msg) {
	cerr << "Type Error @ " << node->location().toString() << ": " << msg << endl;
}

string TypeChecker::debugStr(AST * node) {
	return "  Type Debug @ " + node->location().toString() + ": ";
}

string TypeChecker::applyType(Type *type) {
	return ""; // don't need no stinkin type checking
}

string TypeChecker::applyModule(Module *module,const StringList &vars,const StringList &funcs) {
	return concatenate(vars)+concatenate(funcs); // don't need no stinkin type checking
}

string TypeChecker::applyVarDecl(VarDecl *vd,string type) {
	return type; // don't need no stinkin type checking
}

string TypeChecker::applyFunc(Func *func,const StringList &params,const StringList &vars,const StringList &stmts,string expr) {
	if ((concatenate(params)+concatenate(vars)+concatenate(stmts)+expr).size()) return "error";
	
	// check that the function has been defined (has a body)
	
	
	// check that the parameters and local vars have numeric array sizes
	
	// check that the return value is the correct type
	auto funcType = func->type;
	auto returnType = func->returnExpr->getType();
	LOG << debugStr(func) << "function type " << t[funcType] << " has return type " << t[returnType] << endl;
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
	printError(func, "return expression type doesn't match function signature");
	return "error";
}

string TypeChecker::applyExprStmt(ExprStmt *es,string expr) {
	return expr; // don't need no stinkin type checking
}

string TypeChecker::applyAssignmentStmt(AssignmentStmt *as,string lvalue,string expr) {
	if ((lvalue+expr).size()) return "error";
	
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
		printError(ue, " unary neg must operate on numeric types");
		return "error";
	} else if (ue->oper == TT_NOT && exprType != ST_BOOL) {
		printError(ue, " unary not must operate on booleans");
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
	printError(be, "invalid operand in expression");
	return "error";
}

string TypeChecker::applyConstant(Constant *c) {
	return ""; // don't need no stinkin type checking
}

string TypeChecker::applyLValue(LValue *lv,string index) {
	if (index.size()) return "error";
	
	// check that index is numeric
	auto indexType = lv->index->getType();
	LOG << debugStr(lv) << "lvalue has index type " << t[indexType] << endl;
	if (indexType <= ST_BOOL) {
		printError(lv, "lvalue index must be a numeric type");
		return "error";
	}
	
	return "";
}

string TypeChecker::applyIncDecExpr(IncDecExpr *id,string expr) {
	// check that index is not a bool
	auto lvType = id->lvalue->getType();
	LOG << debugStr(id) << "incdec operating on " << t[lvType] << endl;
	if (lvType <= ST_BOOL) {
		printError(id, "incdec must operate on a numeric type");
		return "error";
	}
	
	return "";
}

string TypeChecker::applyFuncCall(FuncCall *fc,const StringList &params) {
	// check that the function call parametes match the types of the actual function
	
	
	return "";
}

string TypeChecker::applyTypeConversion(TypeConversion *tc, string expr) {
	cerr << "TypeChecker visiting a TypeConversion object!" << endl;
	return "";
}
