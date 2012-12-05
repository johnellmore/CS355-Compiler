@echo off
g++ -Wall Token.cpp Tokenizer.cpp Parser.cpp tests/ParserTest.cpp

:: These files work
echo.
echo These files should be successfully parsed:
a q/0-empty.q
a q/1-increment.q
a q/2-arraysearch.q

:: These files don't work
echo.
echo These files should fail parsing:
a q/e-0-deadsimpleerror.q
a q/e-1-functiondec.q
a q/e-2-badarraysearch.q
a q/e-3-missingexpressionargument.q
a q/e-4-invalidarrayindex.q
a q/e-5-functionvarmixup.q
