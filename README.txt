Hey Dr. Towell,

Here's my completed parser assignment. I've throughly vetted it for bugs.
I built a test harness which runs about 30 tests on my parser. Every single 
line of code is checked and working (excepting the pre-supplied code that's
only relevant to the future Type Checker and Code Gen assignments). My code 
coverage analysis is in the "coverage" directory in HTML format (from lcov).

In order to re-execute the test harness, run the following three scripts 
under bash:
util/compile.sh		removes pre-existing code coverage & compiles parser.
util/test.sh		runs 30-some odd tests and outputs results to stdout.
util/coverage.sh	deletes old coverage and re-generates. Requires lcov.

Thanks for accepting this so late! I hope to be done with Type Checker and at
least most of Code Gen before the semester's over.

John
