# CS355-Compiler

A compiler for the **q** sample language that produces assembly. Written in C++11. This project was built for my CS355 Compiler Construction course.

## Usage

First, compile the project:

```
util/compile.sh
```

Then run the compiler on some input **q** code:

```
q input.q output.asm
```

You can find some pre-written **q** code in the `fulltests` folder.

## Tests

The test harness runs about 30 tests. Every line of code is checked and working using the lcov coverage tool (excepting the pre-supplied code from my course professor). My code coverage analysis is in the "coverage" directory in HTML format.

In order to re-execute the test harness, run the following three scripts:
```
util/test.sh      # runs 30-some odd tests and outputs results to stdout
util/coverage.sh  # deletes old coverage and regenerates (requires lcov)
```
