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
	
	// start the parsing process!
	Tokenizer t(file, cout);
	t.nextToken(); // start the process
	Parser p;
	bool result = p.parse(&t);
	
	// output the results
	if (result) {
		cout << "Parsing succeeded!" << endl;
	} else {
		cout << "Parsing failed." << endl;
	}
	
	// close our input file
	file.close();
}
