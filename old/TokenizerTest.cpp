#include <fstream>
#include "../compiler.h"

int main(int argc, char *argv[]) {
	// check for file argument
	if (argc > 2) {
		cout << "Too many arguments. One argument (filename) is required." << endl;
		return 0;
	} else if (argc < 2) {
		cout << "Q input filename is missing." << endl;
		return 0;
	}
	
	// check that file exists
	fstream file(argv[1], fstream::in);
	if (!file.is_open()) {
		cout << "Given Q input file does not exist." << endl;
		return 0;
	}
	
	// kick off the tokenizer
	Tokenizer t(file, cout);
	t.nextToken();
	while (!t.peek(TT_EOF)) {
		t.nextToken();
	}
	
	// print symbol table
	t.printSymbols();
	
	// close our input file
	file.close();
}
