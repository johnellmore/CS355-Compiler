#include "Compiler.h"

Token::Token(const string &ntext, TokenType ntype, TokenFlag nflags) :
	text(ntext), type(ntype), flags(nflags) {}

int Token::getValue() const {
	if (type == TT_CONSTANT) {
		stringstream t(text);
		int value;
		t >> value;
		return value;
	} else if (TT_TRUE)
		return 1;
	return 0;
}

ScalarType Token::getScalarType() const {
	if (hasAttribute(TF_VARIABLE))
		return dynamic_cast<VarDecl*>(declaration)->type->type;
	else if (hasAttribute(TF_FUNCTION))
		return dynamic_cast<Func*>(declaration)->type;
	else if (type == TT_CONSTANT)
		return ST_LONG;
	else if (type == TT_TRUE || type == TT_FALSE)
		return ST_BOOL;
	return ST_INVALID;
}

bool Token::getBoolValue() const {
	if (type == TT_TRUE || type == TT_FALSE)
		return (bool)(((Constant*)declaration)->value);
	return false;
}

int Token::getArraySize() const {
	if (hasAttribute(TF_VARIABLE) && hasAttribute(TF_ARRAY))
		return dynamic_cast<VarDecl*>(declaration)->type->size;
	return 0;
}

void Token::setAsFunction() {
	LOG << "    function set on: " << text << endl;
	flags = (TokenFlag)(TF_FUNCTION | flags);
}

void Token::setAsVariable(int s) {
	LOG << "    variable set on: " << text << endl;
	flags = (TokenFlag)(TF_VARIABLE | flags);
	// if size is 0, it's not an array
	if (s > 0) flags = (TokenFlag)(TF_ARRAY | flags);
}