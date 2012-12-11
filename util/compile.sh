#!/bin/sh
find . -name "*.gcno" -delete
find . -name "*.gcda" -delete
#g++ --coverage -Wall -std=c++0x -o q Token.cpp Tokenizer.cpp AST.cpp Parser.cpp Dump.cpp TypeChecker.cpp CodeGen.cpp Compiler.cpp
g++ -Wall -DVERBOSE -std=c++0x -o q Token.cpp Tokenizer.cpp AST.cpp Parser.cpp Dump.cpp TypeChecker.cpp CodeGen.cpp Compiler.cpp
