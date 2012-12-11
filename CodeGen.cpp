#include "Compiler.h"

string CodeGen::applyType(Type *ty) {
	return "UBVSUKDFB";
}

string CodeGen::applyModule(Module *module,const StringList &vars,const StringList &funcs) {
	stringstream as;
	// generate global statements for NASM to export
	for (auto f = module->funcs.begin(); f != module->funcs.end(); f++) {
		Func * fn = &**f;
		as << "global " << fn->name->getText() << endl;
	}
	
	// define our code section
	as << endl << "section .text" << endl << endl;
	
	// output the functions
	as << concatenateLines(funcs);
	
	// define our global variables section
	as << "section .data" << endl << endl;
	
	// output the global variable declarations
	as << concatenateLines(vars);
	return as.str();
}

string CodeGen::applyVarDecl(VarDecl *vd,string type) {
	return "IUSBDFSG";
}

string CodeGen::applyFunc(Func *func,const StringList &params,const StringList &vars,const StringList &stmts,string expr) {
	stringstream as;
	
	// print label
	as << func->name->getText() << ":" << endl;
	
	// setup for the function
	as << "push ebp" << endl;
	as << "mov ebp, esp" << endl;
	
	// setup local variable space
	if (func->paramSpace) as << "sub esp, " << func->paramSpace << endl;
	
	// output function code
	as << endl << concatenateLines(stmts) << endl;
	
	// output return expression
	as << expr << endl << endl;
	
	// function conclusion (clean up local vars and restore stack
	if (func->paramSpace) as << "add esp, " << func->paramSpace << endl;
	
	as << "pop ebp" << endl;
	as << "ret" << endl;
	
	return as.str();
}

string CodeGen::applyExprStmt(ExprStmt *es,string expr) {
	// pretty simple
	return expr;
}

string CodeGen::applyAssignmentStmt(AssignmentStmt *as,string lvalue,string expr) {
	stringstream a;
	a << expr << endl;
	if (!as->lvalue->index) {
		if (as->expr->getType() == ST_LONG) a << "mov " << as->lvalue->name->getAsmAddr() << ", eax" << endl;
		else if (as->expr->getType() == ST_SHORT) a << "mov " << as->lvalue->name->getAsmAddr() << ", ax" << endl;
		else if (as->expr->getType() == ST_BYTE) a << "mov " << as->lvalue->name->getAsmAddr() << ", al" << endl;
	} else {
		a << "ILUDUFBFDS";
	}
	return a.str();
}

string CodeGen::applyLoopStmt(LoopStmt *ls,const StringList &stmts,string expr) {
	// not yet done
	return "UVSDFSGF";
}

string CodeGen::applyConditionalStmt(ConditionalStmt *cs,const StringList &thens,const StringList &elses,string expr) {
	stringstream as;
	auto elseLabel = newLabel();
	auto endLabel = newLabel();
	as << expr << endl;
	
	// binary expr
	as << "cmp eax, 1" << endl;
	as << "jne " << elseLabel << endl;
	as << concatenateLines(thens);
	as << "jmp " << endLabel << endl;
	as << elseLabel << ": " << concatenateLines(elses);
	as << endLabel << ": ";
	
	return as.str();
}

string CodeGen::applyUnaryExpr(UnaryExpr *ue,string expr) {
	// not yet done
	return "IBWEFD";
}

string CodeGen::applyBinaryExpr(BinaryExpr *be,string leftExpr,string rightExpr) {
	stringstream as;
	
	auto oper = be->oper;
	auto type = be->getType();
	auto leftType = be->leftExpr->getType();
	
	as << rightExpr << endl;
	as << "push eax" << endl;
	as << leftExpr << endl;
	as << "pop ecx" << endl;
	// left (eax) and right (ecx) params
	
	if (oper == TT_PLUS) {
		if (type == ST_LONG) as << "add eax, ecx";
		else if (type == ST_SHORT) as << "add ax, cx";
		else if (type == ST_BYTE) as << "add al, cl";
	} else if (oper == TT_MINUS) {
		if (type == ST_LONG) as << "sub eax, ecx";
		else if (type == ST_SHORT) as << "sub ax, cx";
		else if (type == ST_BYTE) as << "sub al, cl";
	} else if (oper == TT_MULTIPLY) {
		if (type == ST_LONG) as << "imul eax, ecx";
		else if (type == ST_SHORT) as << "imul ax, cx";
		else if (type == ST_BYTE) as << "imul al, cl";
	} else if (oper == TT_DIVIDE) {
		as << "xor edx, edx" << endl;
		if (type == ST_LONG) as << "idiv ecx";
		else if (type == ST_SHORT) as << "idiv cx";
		else if (type == ST_BYTE) as << "idiv cl";
	} else if (oper == TT_MOD) {
		as << "xor edx, edx" << endl;
		if (type == ST_LONG) as << "idiv ecx" << endl << "mov eax, edx";
		else if (type == ST_SHORT) as << "idiv cx" << endl << "mov ax, dx";
		else if (type == ST_BYTE) as << "idiv cl" << endl << "mov al, ah";
	} else if (TT_LESS_THAN <= oper && oper <= TT_NOT_EQUAL) {
		auto trueLabel = newLabel();
		auto doneLabel = newLabel();
		
		// compare operation
		if (leftType == ST_LONG) as << "cmp eax, ecx" << endl;
		else if (leftType == ST_SHORT) as << "cmp ax, cx" << endl;
		else if (leftType == ST_BYTE) as << "cmp al, cl" << endl;
		
		// operator change
		if (oper == TT_LESS_THAN) as << "jl " << trueLabel << endl;
		else if (oper == TT_LESS_THAN_OR_EQUAL) as << "jle " << trueLabel << endl;
		else if (oper == TT_GREATER_THAN) as << "jg " << trueLabel << endl;
		else if (oper == TT_GREATER_THAN_OR_EQUAL) as << "jge " << trueLabel << endl;
		else if (oper == TT_EQUAL) as << "je " << trueLabel << endl;
		else if (oper == TT_NOT_EQUAL) as << "jne " << trueLabel << endl;
		
		// logic
		as << "mov eax, 0" << endl;
		as << "jmp " << doneLabel << endl;
		as << trueLabel << ": mov eax, 1" << endl;
		as << doneLabel << ": ";
	} else if (oper == TT_AND) {
		as << "and eax, ecx" << endl;
	} else if (oper == TT_OR) {
		as << "or eax, ecx" << endl;
	} else {
		as << "ERROR";
	}
	
	return as.str();
}

string CodeGen::applyConstant(Constant *c) {
	stringstream as;
	if (c->getType() == ST_LONG) as << "mov eax, " << c->value;
	else if (c->getType() == ST_SHORT) as << "mov ax, " << c->value;
	else if (c->getType() == ST_BYTE) as << "mov al, " << c->value;
	return as.str();
}

string CodeGen::applyLValue(LValue *lv,string index) {
	stringstream as;
	if (lv->index == NULL) {
		// not accessing an array
		auto addr = lv->name->getAsmAddr();
		if (lv->getType() == ST_LONG) as << "mov eax, " << addr;
		else if (lv->getType() == ST_SHORT) as << "mov ax, " << addr;
		else if (lv->getType() == ST_BYTE) as << "mov al, " << addr;
	} else {
		// accessing an array
		as << "LOIUHDFERE";
	}
	return as.str();
}

string CodeGen::applyIncDecExpr(IncDecExpr *id,string expr) {
	return "DFGRGEGF";
}

string CodeGen::applyFuncCall(FuncCall *fc,const StringList &params) {
	stringstream as;
	
	// push parameters on in reverse order
	int stackSpace = 0;
	for (auto p = params.rbegin(); p != params.rend(); ++p) {
		as << *p << endl;
		as << "push eax" << endl;
		stackSpace += 4;
	}
	as << "call " << fc->name->getText() << endl;
	as << "add esp, " << stackSpace;
	
	return as.str();
}

string CodeGen::applyTypeConversion(TypeConversion *tc, string expr) {
	stringstream as;
	auto tcType = tc->getType();
	auto childType = tc->expr->getType();
	assert(ST_BYTE < tcType && tcType <= ST_LONG);
	assert(ST_BOOL < childType && childType <= ST_SHORT);
	assert(tcType > childType);
	
	as << expr << endl;
	
	if (childType == ST_BYTE) {
		as << "cbw" << endl;
	}
	if (tcType == ST_LONG) {
		as << "cwde" << endl;
	}
	return as.str();
}

string CodeGen::concatenateLines(const StringList &strings) {
	string result = "";
	for (auto i=strings.begin(); i!=strings.end(); ++i) {
		result += *i;
		result += "\n";
	}
	return result;
}

string CodeGen::newLabel() {
	stringstream label;
	label << "label" << numLabels++;
	return label.str();
}

string CodeGen::getAddr(const string reg, const int offset) {
	stringstream addr;
	addr << "[" << reg;
	if (offset > 0) addr << "+";
	addr << offset << "]";
	return addr.str();
}

string CodeGen::getArrayAddr(const string baseReg, const int offset, const string indexReg) {
	stringstream addr;
	addr << "[" << baseReg;
	if (offset >= 0) addr << "+";
	addr << offset << "+";
	addr << indexReg << "*4]";
	return addr.str();
}
