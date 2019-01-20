#include "Set.h"
#include "SSNSet.h"
#include <iostream>
using namespace std;

SSNSet::SSNSet(){};

bool SSNSet::add(unsigned long ssn)
{
	return m_nset.insert(ssn);
}

int SSNSet::size() const
{
	return m_nset.size();
}

void SSNSet::print() const
{
	unsigned long x;
	for(int i = 0; i < m_nset.size(); i++)
	{
		m_nset.get(i, x);
		cout << x << endl;
	}
}
