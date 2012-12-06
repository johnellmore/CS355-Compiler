// Dwayne Towell -- CS 355 Compiler Construction

#include "Compiler.h"

string Visitor::concatenate(const StringList &strings) {
	string result = "";
	for (auto i=strings.begin(); i!=strings.end(); ++i)
		result += *i;
	return result;
}


// AST ----------------------------
AST::~AST() {
}

// Module ---------------------------
string Module::apply(Visitor &v) {
	list<string> varResults;
	list<string> funcResults;
	applyList(v,vars,varResults);
	applyList(v,funcs,funcResults);
	return v.applyModule(this,varResults,funcResults);
}

// VarDecl ---------------------------
VarDecl::VarDecl(const Position &p,Token *_name,unique_ptr<Type> _type) 
:   AST(p)
,   name(_name)
,   type(move(_type)) 
{
	name->setDeclaration(this);
}

string VarDecl::apply(Visitor &v) {
	string typeResult = type->apply(v);
	return v.applyVarDecl(this,typeResult);
}

// Func ----------------------------
Func::Func(const Position &p,Token *i,ScalarType st,unique_ptr<DeclList> dl)
:   AST(p)
,   name(i)
,   type(st)
{
	params.swap(*dl);
	name->setDeclaration(this);
}

void Func::define(unique_ptr<DeclList> v, unique_ptr<StmtList> s, unique_ptr<Expr> e) {
	vars.swap(*v);
	stmts.swap(*s);
	returnExpr = move(e);
}

string Func::apply(Visitor &v) {
	list<string> paramResults;
	list<string> varResults;
	list<string> stmtResults;
	applyList(v,params,paramResults);
	applyList(v,vars,varResults);
	applyList(v,stmts,stmtResults);
	string exprResult = returnExpr ? returnExpr->apply(v) : "";
	return v.applyFunc(this,paramResults,varResults,stmtResults,exprResult); 
}

// AssignmentStmt --------------------------------
string ExprStmt::apply(Visitor &v) {
	return v.applyExprStmt(this,expr->apply(v));
}

// AssignmentStmt --------------------------------
string AssignmentStmt::apply(Visitor &v) {
	return v.applyAssignmentStmt(this,lvalue->apply(v),expr->apply(v));
}

// LoopStmt --------------------------------

string LoopStmt::apply(Visitor &v) {
	list<string> stmtResults;
	applyList(v,stmts,stmtResults);
	return v.applyLoopStmt(this,stmtResults,expr->apply(v));
}

// ConditionalStmt --------------------------------
string ConditionalStmt::apply(Visitor &v) {
	list<string> thenStmtResults;
	list<string> elseStmtResults;
	applyList(v,thenStmts,thenStmtResults);
	applyList(v,elseStmts,elseStmtResults);
	return v.applyConditionalStmt(this,thenStmtResults,elseStmtResults,conditionalExpr->apply(v));
}

bool Expr::convertibleTo(ScalarType t) {
	ScalarType myType = getType();
	assert(myType>=ST_BOOL && myType<=ST_LONG);
	if (t==ST_BOOL)
		return myType==ST_BOOL;
	return myType>=ST_BYTE && myType<=t; 
}    


// UnaryExpr --------------------------------
string UnaryExpr::apply(Visitor &v) {
	return v.applyUnaryExpr(this,expr->apply(v));
}

ScalarType UnaryExpr::getType() const {
	return expr->getType();
}    

// BinaryExpr --------------------------------
string BinaryExpr::apply(Visitor &v) {
	return v.applyBinaryExpr(this,leftExpr->apply(v),rightExpr->apply(v));
}

ScalarType BinaryExpr::getType() const {
	if (oper>=TT_AND)
		return ST_BOOL;
	return (ScalarType)max(leftExpr->getType(),rightExpr->getType());
}    

// Constant --------------------------------
string Constant::apply(Visitor & v) {
	return v.applyConstant(this);
}

ScalarType Constant::getType() const {
	return type;
}    

// LValue --------------------------------
string LValue::apply(Visitor & v) {
	return v.applyLValue(this,index ? index->apply(v) : "");
}

ScalarType LValue::getType() const {
	return name->getScalarType();
}    

// IncDecExpr --------------------------------
string IncDecExpr::apply(Visitor & v) {
	return v.applyIncDecExpr(this,lvalue->apply(v));
}

ScalarType IncDecExpr::getType() const {
	return lvalue->getType();
}    

// FuncCall --------------------------------
FuncCall::FuncCall(const Position &p,Token *i,unique_ptr<ExprList> pl) 
:   Expr(p)
,   name(i)
{
	params.swap(*pl);
}

string FuncCall::apply(Visitor & v) {
	list<string> paramResults;
	applyList(v,params,paramResults);
	return v.applyFuncCall(this,paramResults);
}

ScalarType FuncCall::getType() const {
	return name->getScalarType();
}

// TypeConversion --------------------------------
string TypeConversion::apply(Visitor &v) {
	return v.applyTypeConversion(this, expr->apply(v));
}
