#!/bin/sh
find . -name "*.gcno" -delete
find . -name "*.gcda" -delete
g++ --coverage -Wall -std=c++0x Token.cpp Tokenizer.cpp AST.cpp Parser.cpp Dump.cpp AstTest.cpp
#g++ -Wall -DVERBOSE -std=c++0x Token.cpp Tokenizer.cpp AST.cpp Parser.cpp Dump.cpp tests/AstTest.cpp
