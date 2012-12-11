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
	
	// set up everything
	cout << "Setup..." << endl;
	Tokenizer t(file, cout);
	Parser p;
	cout << "Setup complete." << endl;
	
	cout << "Parsing... " << endl;
	if (!p.parse(&t)) {
		cerr << "Parsing failed." << endl;
		return 0;
	}
	file.close(); // close our input file
	cout << "Parsing succeeded." << endl;
	Module * m = p.getModule();
	
	// run type checking
	TypeChecker tc;
	cout << "Running type checker... " << endl;
	auto tcReturn = m->apply(tc);
	if (tcReturn.size()) { // a non-empty return string means an error occurred
		cerr << "Type checking failed." << endl;
		return 0;
	}
	cout << "Type checker succeeded." << endl;
	
	// dump the AST
	Dump d;
	cout << "Dumping AST..." << endl;
	cout << m->apply(d);
	cout << "AST dumped." << endl;
	
	//LOG << endl << endl;
	//t.printSymbols();
}
