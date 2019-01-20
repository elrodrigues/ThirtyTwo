#include "Set.h"
#include "SSNSet.h"
#include <iostream>
#include <cassert>
using namespace std;

int main()
{
	SSNSet a;
	a.add(123456789);
	a.add(412552636);
	a.add(121212124);
	a.add(000000000);
	cout << a.size() << endl;
	a.print();
}
