#include <fstream>
#include "Compiler.h"

int main(int argc, char *argv[]) {
	// check for file argument
	if (argc > 2) {
		cerr << "Too many arguments. One argument (filename) is required." << endl;
		return 0;
	} else if (argc < 2) {
		cerr << "Q input filename is missing." << endl;
		return 0;
	}
	
	// check that file exists
	fstream file(argv[1], fstream::in);
	if (!file.is_open()) {
		cerr << "Given Q input file does not exist." << endl;
		return 0;
	}
	
	// start the parsing process!
	Tokenizer t(file, cout);
	Parser p;
	
	if (!p.parse(&t)) {
		cerr << "Parsing failed." << endl;
		return 0;
	}
	cout << "Parsing succeeded." << endl;
	Module * m = p.getModule();
	
	// Execute type checking
	TypeChecker tc;
	cout << m->apply(tc);
	
	// Dump the AST
	Dump d;
	cout << m->apply(d);
	
	//LOG << endl << endl;
	//t.printSymbols();
	
	// close our input file
	file.close();
}
