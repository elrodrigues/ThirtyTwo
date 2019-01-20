#include "Set.h"
#include <iostream>
#include <cassert>
using namespace std;

int main()
{
	Set ss;
	ss.insert("lavash");
	ss.insert("roti");
	ss.insert("chapati");
	ss.insert("injera");
	ss.insert("roti");
	ss.insert("matzo");
	ss.insert("injera");
	assert(ss.size() == 5);  // duplicate "roti" and "injera" were not added
	string x;
	ss.get(0, x);
	assert(x == "chapati");  // "chapati" is greater than exactly 0 items in ss
	ss.get(4, x);
	assert(x == "roti");  // "roti" is greater than exactly 4 items in ss
	ss.get(2, x);
	assert(x == "lavash");  // "lavash" is greater than exactly 2 items in ss
	cout << "Passed all tests!" << endl;
}
