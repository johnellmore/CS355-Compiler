#include "Compiler.h"

string TypeChecker::applyType(Type *t) { 
	stringstream os;
	os << typeName[t->type];
	if (t->size>0)
		os << "[" << t->size << "]";
	return os.str();
}

string TypeChecker::applyModule(Module *,const StringList &vars,const StringList &funcs) {
	return "Module variables:\n"
		+  indent(concatenate(vars))
		+  "Module functions:\n"
		+  indent(concatenate(funcs));
}

string TypeChecker::applyVarDecl(VarDecl *vd,string type) {
	return vd->name->getText() + " : " + type + "\n";
}

string TypeChecker::applyFunc(Func *f,const StringList &params,const StringList &vars,const StringList &stmts,string expr) {
	stringstream os;
	os << f->name->getText() << " : " << typeName[f->type];
	if (f->returnExpr)
		os << " definition\n";
	else
		os << " declaration\n";
	if (f->params.size())
		os << indent("Parameters:\n") << indent(indent(concatenate(params)));
	if (f->vars.size())
		os << indent("Local variables:\n") << indent(indent(concatenate(vars)));
	if (f->stmts.size())
		os << indent("Statements:\n") << indent(indent(concatenate(stmts)));
	if (f->returnExpr)
		os << indent("Return expression:\n") << indent(indent(expr));
	return os.str();
}

string TypeChecker::applyExprStmt(ExprStmt *,string expr) {
	return "Expression statement:\n" + indent(expr);
}

string TypeChecker::applyAssignmentStmt(AssignmentStmt *,string lvalue,string expr) {
	return "Assignment statement:\n" 
		+  indent(lvalue)
		+  indent("Expression:\n")
		+  indent(indent(expr));
}

string TypeChecker::applyLoopStmt(LoopStmt *,const StringList &stmts,string expr) {
	return "Loop statement:\n"
		+  indent("Expression:\n")
		+  indent(indent(expr))
		+  indent("Statements:\n")
		+  indent(indent(concatenate(stmts)));
}

string TypeChecker::applyConditionalStmt(ConditionalStmt *,const StringList &thens,const StringList &elses,string expr) {
	return "Conditional statement:\n"
		+  indent("Expression:\n")
		+  indent(indent(expr))
		+  indent("Then statements:\n")
		+  indent(indent(concatenate(thens)))
		+  indent("Else statements:\n")
		+  indent(indent(concatenate(elses)));
}

string TypeChecker::applyUnaryExpr(UnaryExpr *ue,string expr) {
	string op[] = {"not","neg"};
	return "Unary " + op[ue->oper-TT_NOT] + " expression:\n" 
		+  indent(expr);
}

string TypeChecker::applyBinaryExpr(BinaryExpr *be,string leftExpr,string rightExpr) {
	string op[] = {"+","-","*","/","%","<","<=",">",">=","=","<>","and","or"};
	return "Binary " + op[be->oper-TT_PLUS] + " expression:\n"
		+ indent("Left expression:\n")
		+ indent(indent(leftExpr))
		+ indent("Right expression:\n")
		+ indent(indent(rightExpr));
}

string TypeChecker::applyConstant(Constant *c) {
	stringstream os;
	os << "Constant: " << c->value << " : " << typeName[c->type] << "\n";
	return os.str();
}

string TypeChecker::applyLValue(LValue *lvalue,string index) {
	string result = "L-Value: " + lvalue->name->getText() + "\n";
	if (lvalue->index)
		result += indent("Index:\n")
				+ indent(indent(index)); 
	return result;
}

string TypeChecker::applyIncDecExpr(IncDecExpr *ie,string expr) {
	string op[] = {"Increment","Decrement"};
	return op[ie->oper-TT_INC] + " expression:\n"
		+  indent(expr);
}

string TypeChecker::applyFuncCall(FuncCall *call,const StringList &params) {
	string result = "Function call to " + call->name->getText() + ":\n";
	if (call->params.size()>0)
		result += indent("Parameters:\n")
				+ indent(indent(concatenate(params)));
	return result;
}