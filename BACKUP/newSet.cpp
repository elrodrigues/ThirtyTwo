#include "newSet.h"

void shift(ItemType* st, int start, int size, bool mode)
{
	if(mode)
	{
		for(int j = size - 1; j >= start; j--)
			st[j+1] = st[j];
	}
	else
	{
		for(int j = start; (j + 1) < size; j++)
			st[j] = st[j + 1];
	}
} // Useful function for insert and erase

Set::Set()
{
	m_lim = DEFAULT_MAX_ITEMS;
	m_size = 0;
	m_nset = new ItemType[DEFAULT_MAX_ITEMS];
} // Default constructor

Set::Set(int size)
{
	m_lim = size;
	m_size = 0;
	m_nset = new ItemType[m_lim];
} // Custom constructor

Set::~Set()
{
	delete [] m_nset;
} // Destructor

Set::Set(const Set& other)
{
	m_lim = other.m_lim;
	m_size = other.m_size;
	m_nset = new ItemType[m_lim];
	for(int i = 0; i < m_size; i++)
	{
		m_nset[i] = other.m_nset[i];
	}
} // Copy constructor

Set& Set::operator=(const Set& rhs)
{
	if(this != &rhs)
	{
		Set temp(rhs);
		swap(temp);
	}
	return *this;
} // Assignment overloader

bool Set::empty() const
{
	return (m_size == 0);
}

int Set::size() const
{
	return m_size;
}

bool Set::insert(const ItemType& value)
{
	//// ERROR CHECKING
	if(m_size == m_lim) // Checks if array full
		return false;

	if(this->contains(value)){
		return false;
	} // Checks if value already there.

	if(m_size == 0) {
		m_nset[0] = value;
		m_size++;
		return true;
	} // Inserts first element. Redundant but nice.
	//// INSERTION
	for(int j = 0; j < m_size; j++) 
	{
		if(m_nset[j] > value) {
			shift(m_nset, j, m_size, true);
			m_nset[j] = value;
			m_size++;
			return true;
		}
	}
	m_nset[m_size++] = value;
	return true;
}

bool Set::erase(const ItemType& value)
{
	bool chk = true;
	int pos;
	for(int i = 0; i < m_size; i++) {
		if(m_nset[i] == value) {
			pos = i;
			chk = false;
		}
	} // Checks if value in set

	if(chk)
		return false;
	
	shift(m_nset, pos, m_size, false);
	m_size--;
	return true;
}

bool Set::contains(const ItemType& value) const
{
	for(int i = 0; i < m_size; i++)
	{
		if(m_nset[i] == value) {
			return true;
		}
	}
	return false;
}

bool Set::get(int i, ItemType& value) const
{
	if(i < 0 || i >= m_size)
		return false;
	value = m_nset[i];
	return true;
}

void Set::swap(Set& other)
{
	//// Temporary Variable
	ItemType* tmp = other.m_nset;
	int tsize = other.m_size;
	int tlim = other.m_lim;

	//// Swap
	// Pointers
	other.m_nset = m_nset;
	m_nset = tmp;

	// Size
	other.m_size = m_size;
	m_size = tsize;

	// Limits
	other.m_lim = m_lim;
	m_lim = tlim;
} // Uses pointers. Executes same number of lines.

