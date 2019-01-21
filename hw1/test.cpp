#include "Set.h"
#include <iostream>

#include <cassert> // Bob

using namespace std;

int main()
{
	Set a;
	a.insert(12);
	a.insert(25);
	a.insert(20);
	assert(!(a.insert(69)));
	a.erase(20);
	a.dump();
}
