class CodeGen : public Visitor {
public:
	CodeGen() : numLabels(0) {};
	string applyType(Type *type);
	string applyModule(Module *module,const StringList &vars,const StringList &funcs);
	string applyVarDecl(VarDecl *vd,string type);
	string applyFunc(Func *func,const StringList &params,const StringList &vars,const StringList &stmts,string returnExpr);
	string applyExprStmt(ExprStmt *es,string expr);
	string applyAssignmentStmt(AssignmentStmt *as,string lvalue,string expr);
	string applyLoopStmt(LoopStmt *ls,const StringList &stmts,string expr);
	string applyConditionalStmt(ConditionalStmt *cs,const StringList &thens,const StringList &elses,string expr);
	string applyUnaryExpr(UnaryExpr *ue,string expr);
	string applyBinaryExpr(BinaryExpr *be,string left,string right);
	string applyConstant(Constant *c);
	string applyLValue(LValue *lv,string expr);
	string applyIncDecExpr(IncDecExpr *id,string lvalue);
	string applyFuncCall(FuncCall *fc,const StringList &params);
	string applyTypeConversion(TypeConversion *tc, string expr);
protected:
	static string concatenateLines(const StringList &strings);
	string getAddr(const string reg, const int offset);
	string getArrayAddr(const string baseReg, const int offset, const string indexReg);
	string newLabel();
private:
	int numLabels;
};
