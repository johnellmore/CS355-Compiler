#include <fstream>
#include "Compiler.h"

int main(int argc, char *argv[]) {
	// check for file argument
	if (argc > 3) {
		cerr << "Too many arguments. Two arguments (input filename and output filename) are required." << endl;
		return 0;
	} else if (argc < 3) {
		cerr << "Too few arguments." << endl;
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
	
	// Output the symbol table
	//LOG << endl;
	//t.debugSymbols();
	
	// open the output file
	fstream asmout(argv[2], fstream::out);
	if (!asmout.is_open()) {
		cerr << "Unable to create output file" << endl;
		return 0;
	}
	
	// create the final assembly code
	CodeGen g;
	cout << "Generating final assembly..." << endl;
	asmout << m->apply(g);
	cout << "Generated!" << endl;
	
	asmout.close();
}
