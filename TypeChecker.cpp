#include "Compiler.h"

void TypeChecker::printError(Position pos, string msg) {
	cerr << "Type Error @ " << pos.toString() << ": " << msg << endl;
}

string TypeChecker::applyType(Type *type) { 
	return "";
}

string TypeChecker::applyModule(Module *module,const StringList &vars,const StringList &funcs) {
	return "";
}

string TypeChecker::applyVarDecl(VarDecl *vd,string type) {
	return "";
}

string TypeChecker::applyFunc(Func *func,const StringList &params,const StringList &vars,const StringList &stmts,string expr) {
	cerr << "Being applied to a function! Its name is " << func->name->getText() << endl;
	return "";
}

string TypeChecker::applyExprStmt(ExprStmt *es,string expr) {
	return "";
}

string TypeChecker::applyAssignmentStmt(AssignmentStmt *as,string lvalue,string expr) {
	return "";
}

string TypeChecker::applyLoopStmt(LoopStmt *ls,const StringList &stmts,string expr) {
	return "";
}

string TypeChecker::applyConditionalStmt(ConditionalStmt *cs,const StringList &thens,const StringList &elses,string expr) {
	return "";
}

string TypeChecker::applyUnaryExpr(UnaryExpr *ue,string expr) {
	return "";
}

string TypeChecker::applyBinaryExpr(BinaryExpr *be,string leftExpr,string rightExpr) {
	return "";
}

string TypeChecker::applyConstant(Constant *c) {
	return "";
}

string TypeChecker::applyLValue(LValue *lv,string index) {
	return "";
}

string TypeChecker::applyIncDecExpr(IncDecExpr *id,string expr) {
	return "";
}

string TypeChecker::applyFuncCall(FuncCall *fc,const StringList &params) {
	return "";
}

string TypeChecker::applyTypeConversion(TypeConversion *tc, string expr) {
	cerr << "TypeChecker visiting a TypeConversion object!" << endl;
	return "";
}
