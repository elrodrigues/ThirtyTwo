#include "Set.h"
#include <iostream>
using namespace std;

int main()
{
	Set a;
	a.insert(5);
	a.insert(2);
	a.insert(6);
	a.insert(4);

	ItemType t;
	a.get(0, t);
	cout << t << endl;
	a.get(1, t);
	cout << t << endl;
	a.get(2, t);
	cout << t << endl;
	a.get(3, t);
	cout << t << endl;

	cout << "====" << endl;
	a.erase(2);
	a.get(0, t);
	cout << t << endl;
	a.get(1, t);
	cout << t << endl;
	a.get(2, t);
	cout << t << endl;
	cout << "Size:" << a.size() << endl;

	cout << "====" << endl;
	a.erase(4);
	a.get(0, t);
	cout << t << endl;
	a.get(1, t);
	cout << t << endl;
	cout << "Size:" << a.size() << endl;

	cout << "====" << endl;
	a.erase(5);
	a.get(0, t);
	cout << t << endl;
	cout << "Size:" << a.size() << endl;

	cout << "====" << endl;
	a.erase(6);
	cout << "Size:" << a.size() << endl;
	cout << "Empty: " << a.empty() << endl;
}
