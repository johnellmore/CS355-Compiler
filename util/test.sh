#!/bin/sh



echo "\nTESTING SUCCESSES (should be no output)"

echo "  Testing Success 0: Empty file"
./a.out q/c-0-empty.q > q/output-0-empty.txt

echo "  Testing Success 1: Full test"
./a.out q/c-1-fulltest.q > q/output-1-fulltest.txt



echo "\nTESTING ERROR MESSAGES"

echo "  Testing Error 2: Invalid token"
./a.out q/c-2-invalidtoken.q > q/output-2-invalidtoken.txt

echo "  Testing Error 3: EOF expected but not found"
./a.out q/c-3-eof-expected.q > q/output-3-eof-expected.txt

echo "  Testing Error 4: Modules vars unended"
./a.out q/c-4-module-vars-unended.q > q/output-4-module-vars-unended.txt

echo "  Testing Error 5: Module expected"
./a.out q/c-5-module-expected.q > q/output-5-module-expected.txt

echo "  Testing Error 6: Identifier clash"
./a.out q/c-6-identifier-clash.q > q/output-6-identifier-clash.txt

echo "  Testing Error 7: Invalid function name"
./a.out q/c-7-invalid-function-name.q > q/output-7-invalid-function-name.txt

echo "  Testing Error 8: Function vars, no do"
./a.out q/c-8-function-vars-no-do.q > q/output-8-function-vars-no-do.txt

echo "  Testing Error 9: Function missing flesh"
./a.out q/c-9-function-missing-flesh.q > q/output-9-function-missing-flesh.txt

echo "  Testing Error 10: VarDecl missing identifier"
./a.out q/c-10-vardecl-missing-ident.q > q/output-10-vardecl-missing-ident.txt

echo "  Testing Error 11: VarDecl identifier is function"
./a.out q/c-11-vardecl-ident-function.q > q/output-11-vardecl-ident-function.txt

echo "  Testing Error 12: VarDecl missing colon"
./a.out q/c-12-vardecl-missing-colon.q > q/output-12-vardecl-missing-colon.txt

echo "  Testing Error 13: Invalid array length"
./a.out q/c-13-invalid-array-length.q > q/output-13-invalid-array-length.txt

echo "  Testing Error 14: Array too small"
./a.out q/c-14-array-too-small.q > q/output-14-array-too-small.txt

echo "  Testing Error 15: Missing array close bracket"
./a.out q/c-15-missing-array-close-bracket.q > q/output-15-missing-array-close-bracket.txt

echo "  Testing Error 16: Invalid scalar type"
./a.out q/c-16-invalid-scalar-type.q > q/output-16-invalid-scalar-type.txt

echo "  Testing Error 17: Expression expected"
./a.out q/c-17-expression-expected.q > q/output-17-expression-expected.txt

echo "  Testing Error 18: LValue expected var ident"
./a.out q/c-18-lvalue-expected-var-ident.q > q/output-18-lvalue-expected-var-ident.txt

echo "  Testing Error 19: LValue expected array close"
./a.out q/c-19-lvalue-expected-array-close.q > q/output-19-lvalue-expected-array-close.txt

echo "  Testing Error 20: LValue expected ident"
./a.out q/c-20-lvalue-expected-ident.q > q/output-20-lvalue-expected-ident.txt

echo "  Testing Error 21: If close bracket missing"
./a.out q/c-21-if-close-bracket-missing.q > q/output-21-if-close-bracket-missing.txt

echo "  Testing Error 22: Else open bracket missing"
./a.out q/c-22-else-open-bracket-missing.q > q/output-22-else-open-bracket-missing.txt

echo "  Testing Error 23: Else close bracket missing"
./a.out q/c-23-else-close-bracket-missing.q > q/output-23-else-close-bracket-missing.txt

echo "  Testing Error 24: Expected loop or if"
./a.out q/c-24-expected-loop-or-if.q > q/output-24-expected-loop-or-if.txt



echo "\nTESTING EXTRA CODE"

echo "  Testing Error 25: Fail return expression parse"
./a.out q/c-25-fail-return-exp-parse.q > q/output-25-fail-return-exp-parse.txt

echo "  Testing Error 26: Fail parameter list parse"
./a.out q/c-26-fail-param-list-parse.q > q/output-26-fail-param-list-parse.txt



echo "\nTESTING FRAMEWORK"

echo "  Testing Error 101: Framework argument count exceeded"
./a.out q/c-0-empty.q extra here > q/output-101-argumenterrorbig.txt

echo "  Testing Error 102: Framework argument count unreached"
./a.out > q/output-102-argumenterrorsmall.txt

echo "  Testing Error 103: Framework input file missing"
./a.out not-really-here.q > q/output-103-filemissing.txt