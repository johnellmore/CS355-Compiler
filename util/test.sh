#!/bin/sh

echo "\nTESTING SUCCESSES (should be no output)"

echo "  Testing Success 0: Empty file"
./q qtests/c-0-empty.q qtests/compiled-0-empty.asm > qtests/output-0-empty.txt

echo "  Testing Success 1: Full test"
./q qtests/c-1-fulltest.q qtests/compiled-1-fulltest.asm > qtests/output-1-fulltest.txt



echo "\nTESTING ERROR MESSAGES"

echo "  Testing Error 2: Invalid token"
./q qtests/c-2-invalidtoken.q qtests/bogus.asm > qtests/output-2-invalidtoken.txt

echo "  Testing Error 3: EOF expected but not found"
./q qtests/c-3-eof-expected.q qtests/bogus.asm > qtests/output-3-eof-expected.txt

echo "  Testing Error 4: Modules vars unended"
./q qtests/c-4-module-vars-unended.q qtests/bogus.asm > qtests/output-4-module-vars-unended.txt

echo "  Testing Error 5: Module expected"
./q qtests/c-5-module-expected.q qtests/bogus.asm > qtests/output-5-module-expected.txt

echo "  Testing Error 6: Identifier clash"
./q qtests/c-6-identifier-clash.q qtests/bogus.asm > qtests/output-6-identifier-clash.txt

echo "  Testing Error 7: Invalid function name"
./q qtests/c-7-invalid-function-name.q qtests/bogus.asm > qtests/output-7-invalid-function-name.txt

echo "  Testing Error 8: Function vars, no do"
./q qtests/c-8-function-vars-no-do.q qtests/bogus.asm > qtests/output-8-function-vars-no-do.txt

echo "  Testing Error 9: Function missing flesh"
./q qtests/c-9-function-missing-flesh.q qtests/bogus.asm > qtests/output-9-function-missing-flesh.txt

echo "  Testing Error 10: VarDecl missing identifier"
./q qtests/c-10-vardecl-missing-ident.q qtests/bogus.asm > qtests/output-10-vardecl-missing-ident.txt

echo "  Testing Error 11: VarDecl identifier is function"
./q qtests/c-11-vardecl-ident-function.q qtests/bogus.asm > qtests/output-11-vardecl-ident-function.txt

echo "  Testing Error 12: VarDecl missing colon"
./q qtests/c-12-vardecl-missing-colon.q qtests/bogus.asm > qtests/output-12-vardecl-missing-colon.txt

echo "  Testing Error 13: Invalid array length"
./q qtests/c-13-invalid-array-length.q qtests/bogus.asm > qtests/output-13-invalid-array-length.txt

echo "  Testing Error 14: Array too small"
./q qtests/c-14-array-too-small.q qtests/bogus.asm > qtests/output-14-array-too-small.txt

echo "  Testing Error 15: Missing array close bracket"
./q qtests/c-15-missing-array-close-bracket.q qtests/bogus.asm > qtests/output-15-missing-array-close-bracket.txt

echo "  Testing Error 16: Invalid scalar type"
./q qtests/c-16-invalid-scalar-type.q qtests/bogus.asm > qtests/output-16-invalid-scalar-type.txt

echo "  Testing Error 17: Expression expected"
./q qtests/c-17-expression-expected.q qtests/bogus.asm > qtests/output-17-expression-expected.txt

echo "  Testing Error 18: LValue expected var ident"
./q qtests/c-18-lvalue-expected-var-ident.q qtests/bogus.asm > qtests/output-18-lvalue-expected-var-ident.txt

echo "  Testing Error 19: LValue expected array close"
./q qtests/c-19-lvalue-expected-array-close.q qtests/bogus.asm > qtests/output-19-lvalue-expected-array-close.txt

echo "  Testing Error 20: LValue expected ident"
./q qtests/c-20-lvalue-expected-ident.q qtests/bogus.asm > qtests/output-20-lvalue-expected-ident.txt

echo "  Testing Error 21: If close bracket missing"
./q qtests/c-21-if-close-bracket-missing.q qtests/bogus.asm > qtests/output-21-if-close-bracket-missing.txt

echo "  Testing Error 22: Else open bracket missing"
./q qtests/c-22-else-open-bracket-missing.q qtests/bogus.asm > qtests/output-22-else-open-bracket-missing.txt

echo "  Testing Error 23: Else close bracket missing"
./q qtests/c-23-else-close-bracket-missing.q qtests/bogus.asm > qtests/output-23-else-close-bracket-missing.txt

echo "  Testing Error 24: Expected loop or if"
./q qtests/c-24-expected-loop-or-if.q qtests/bogus.asm > qtests/output-24-expected-loop-or-if.txt



echo "\nTESTING EXTRA CODE"

echo "  Testing Error 25: Fail return expression parse"
./q qtests/c-25-fail-return-exp-parse.q qtests/bogus.asm > qtests/output-25-fail-return-exp-parse.txt

echo "  Testing Error 26: Fail parameter list parse"
./q qtests/c-26-fail-param-list-parse.q qtests/bogus.asm > qtests/output-26-fail-param-list-parse.txt



echo "\nTESTING TYPE CHECKER"

echo "  Testing TC Error 1: Function not defined"
./q qtests/tc-1-function-not-defined.q qtests/bogus.asm > qtests/output-tc-1-function-not-defined.txt

echo "  Testing TC Error 2: Function return type mismatch"
./q qtests/tc-2-function-return-type-mismatch.q qtests/bogus.asm > qtests/output-tc-2-function-return-type-mismatch.txt

echo "  Testing TC Error 3: Array as assignment lvalue"
./q qtests/tc-3-array-as-assignment-lvalue.q qtests/bogus.asm > qtests/output-tc-3-array-as-assignment-lvalue.txt

echo "  Testing TC Error 4: Array as assignment rvalue"
./q qtests/tc-4-array-as-assignment-rvalue.q qtests/bogus.asm > qtests/output-tc-4-array-as-assignment-rvalue.txt

echo "  Testing TC Error 5: Type mismatch on assignment"
./q qtests/tc-5-type-mismatch-on-assignment.q qtests/bogus.asm > qtests/output-tc-5-type-mismatch-on-assignment.txt

echo "  Testing TC Error 6: If condition mismatch"
./q qtests/tc-6-if-condition-mismatch.q qtests/bogus.asm > qtests/output-tc-6-if-condition-mismatch.txt

echo "  Testing TC Error 7: Unary operator type mismatch (two errors)"
./q qtests/tc-7-unary-operator-type-mismatch.q qtests/bogus.asm > qtests/output-tc-7-unary-operator-type-mismatch.txt

echo "  Testing TC Error 8: Unary operand is array"
./q qtests/tc-8-unary-operand-is-array.q qtests/bogus.asm > qtests/output-tc-8-unary-operand-is-array.txt

echo "  Testing TC Error 9: Binary operand is array"
./q qtests/tc-9-binary-operand-is-array.q qtests/bogus.asm > qtests/output-tc-9-binary-operand-is-array.txt

echo "  Testing TC Error 10: Binary expression type mismatch"
./q qtests/tc-10-binary-expr-type-mismatch.q qtests/bogus.asm > qtests/output-tc-10-binary-expr-type-mismatch.txt

echo "  Testing TC Error 11: LValue index not numeric"
./q qtests/tc-11-lvalue-index-not-numeric.q qtests/bogus.asm > qtests/output-tc-11-lvalue-index-not-numeric.txt

echo "  Testing TC Error 12: IncDec invalid operand (two errors)"
./q qtests/tc-12-incdec-invalid-operand.q qtests/bogus.asm > qtests/output-tc-12-incdec-invalid-operand.txt

echo "  Testing TC Error 13: Function call has too many/few params (two errors)"
./q qtests/tc-13-funccall-param-count-off.q qtests/bogus.asm > qtests/output-tc-13-funccall-param-count-off.txt

echo "  Testing TC Error 14: Function call cannot convert array"
./q qtests/tc-14-funccall-cannot-convert-array.q qtests/bogus.asm > qtests/output-tc-14-funccall-cannot-convert-array.txt

echo "  Testing TC Error 15: Function call parameter type mismatch"
./q qtests/tc-15-funccall-param-type-mismatch.q qtests/bogus.asm > qtests/output-tc-15-funccall-param-type-mismatch.txt

echo "  Testing TC Error 16: Function call parameter requires array, single given"
./q qtests/tc-16-funccall-func-requires-array.q qtests/bogus.asm > qtests/output-tc-16-funccall-func-requires-array.txt

echo "  Testing TC Error 17: Function call parameter requires single, array given"
./q qtests/tc-17-funccall-array-param-given.q qtests/bogus.asm > qtests/output-tc-17-funccall-array-param-given.txt

echo "  Testing TC Error 18: Function call array size mismatch"
./q qtests/tc-18-funccall-param-array-size.q qtests/bogus.asm > qtests/output-tc-18-funccall-param-array-size.txt

echo "  Testing TC Error 19: Function variable redeclaration (param vs. local)"
./q qtests/tc-19-function-var-redeclaration.q qtests/bogus.asm > qtests/output-tc-19-function-var-redeclaration.txt



echo "\nTESTING FRAMEWORK"

echo "  Testing Error 101: Framework argument count exceeded"
./q qtests/c-0-empty.q qtests/bogus.asm extra here > qtests/output-101-argumenterrorbig.txt

echo "  Testing Error 102: Framework argument count unreached"
./q > qtests/output-102-argumenterrorsmall.txt

echo "  Testing Error 103: Framework input file missing"
./q not-really-here.q qtests/bogus.asm > qtests/output-103-filemissing.txt