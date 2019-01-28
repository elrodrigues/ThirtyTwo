#include "Set.h"
#include <iostream>
#include <cassert>
using namespace std;

int main()
{
	Set a;
	a.insert(5);
	a.insert(2);
	a.insert(6);
	a.insert(4);

	Set b;
	b.insert(25);
	b.insert(2);
	b.insert(6);
	b.insert(420);
	b.insert(1);

	assert(b.size() == 5 && !(b.empty()));
	assert(a.size() == 4 && !(a.empty()));
	Set bah;
	assert(!bah.erase(69));
	Set res;
	ItemType val;

	unite(a, b, res);
	for(int i = 0; i < res.size(); i++)
	{
		res.get(i, val);
		cout << val << endl;
	}
	cout << "====== res^" << endl;

	subtract(a, b, res);
	for(int i = 0; i < res.size(); i++)
	{
		res.get(i, val);
		cout << val << endl;
	}
	cout << "====== res^ sub" << endl;
	a.swap(b);
	assert(b.size() == 4 && !(b.empty()));
	assert(a.size() == 5 && !(a.empty()));
	for(int i = 0; i < a.size(); i++)
	{
		a.get(i, val);
		cout << val << endl;
	}
	cout << "====== a^" << endl;
	for(int i = 0; i < b.size(); i++)
	{
		b.get(i, val);
		cout << val << endl;
	}
	cout << "====== b^" << endl;
	Set c = b;
	for(int i = 0; i < c.size(); i++)
	{
		c.get(i, val);
		cout << val << endl;
	}
	cout << "====== c <- b ^" << endl;
	c = a;
	for(int i = 0; i < c.size(); i++)
	{
		c.get(i, val);
		cout << val << endl;
	}
	cout << "====== c <- a ^" << endl;
}
