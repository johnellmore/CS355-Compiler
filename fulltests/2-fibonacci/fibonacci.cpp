#include <iostream>
#include <cassert>
using namespace std;

extern "C" int qFibonacci(int n);

int cFibonacci(int n) {
	if (n <= 2) return 1;
	return cFibonacci(n-1) + cFibonacci(n-2);
}

int main() {
	cout << "cFibonacci called on 5: " << cFibonacci(10) << endl;
	cout << "qFibonacci called on 5: " << qFibonacci(10) << endl;
	
	assert(cFibonacci(1) == qFibonacci(1));
	assert(cFibonacci(2) == qFibonacci(2));
	assert(cFibonacci(3) == qFibonacci(3));
	assert(cFibonacci(4) == qFibonacci(4));
	assert(cFibonacci(5) == qFibonacci(5));
	assert(cFibonacci(6) == qFibonacci(6));
	assert(cFibonacci(7) == qFibonacci(7));
	assert(cFibonacci(8) == qFibonacci(8));
	assert(cFibonacci(9) == qFibonacci(9));
	
	cout << "you made it!\n";
}
