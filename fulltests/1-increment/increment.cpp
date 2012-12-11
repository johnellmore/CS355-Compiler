#include <iostream>
#include <cassert>
using namespace std;

extern "C" int increment(int n);
extern "C" int multiply(int a, int b);
extern "C" int divide(int a, int b);
extern "C" int subtract(int a, int b);
extern "C" int mod(int a, int b);
extern "C" bool lessb(int a, int b);
extern "C" bool greaterb(int a, int b);
extern "C" bool lesseq(int a, int b);
extern "C" bool greatereq(int a, int b);
extern "C" bool equal(int a, int b);
extern "C" bool notequal(int a, int b);

int main() {
	cout << "increment called on 5: " << increment(5) << endl;
	cout << "increment called on 12: " << increment(12) << endl;
	
	cout << "multiply called on 3,5: " << multiply(3,5) << endl;
	cout << "multiply called on 7,12: " << multiply(7,12) << endl;
	
	cout << "divide called on 10,3: " << divide(10,3) << endl;
	
	cout << "subtract called on 8,13: " << subtract(8,13) << endl;
	
	cout << "mod called on 16,3: " << mod(16,3) << endl;
	
	cout << "lessb called on 8,5: " << lessb(8,5) << endl;
	
	cout << "greaterb called on 8,5: " << greaterb(8,5) << endl;
	
	cout << "lesseq called on 8,8: " << lesseq(8,8) << endl;
	
	cout << "greaterer called on 8,8: " << greatereq(8,8) << endl;
	
	cout << "equal called on 7,4: " << equal(7,4) << endl;
	
	cout << "notequal called on 7,4: " << greatereq(7,4) << endl;
	
	bool test = false;
	char *x = reinterpret_cast<char*>(&test);
	cout << "Value: " << (int)*x << endl;
	
	cout << "done!\n";
}
