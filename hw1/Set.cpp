#include <string>
#include "Set.h"

Set::Set()
{
	m_size = 0;
}

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
	if(m_size == DEFAULT_MAX_ITEMS) // Checks if array full
		return false;

	if(this->contains(value)){
		return false;
	} // Checks if value already there.

	if(m_size == 0) {
		m_set[0] = value;
		m_size++;
		return true;
	} // Inserts first element
	//// INSERTION
	for(int j = 0; j < m_size; j++) 
	{
		if(m_set[j] > value) {
			shift(m_set, j, m_size, true);
			m_set[j] = value;
			m_size++;
			return true;
		}
	}
	m_set[m_size++] = value;
	return true;
}

bool Set::erase(const ItemType& value)
{
	bool chk = true;
	int pos;
	for(int i = 0; i < m_size; i++) {
		if(m_set[i] == value) {
			pos = i;
			chk = false;
		}
	} // Checks if value in set

	if(chk)
		return false;
	
	shift(m_set, pos, m_size, false);
	m_size--;
	return true;
}

bool Set::contains(const ItemType& value) const
{
	for(int i = 0; i < m_size; i++)
	{
		if(m_set[i] == value) {
			return true;
		}
	}
	return false;
}

bool Set::get(int i, ItemType& value)
{
	if(i < 0 || i >= m_size)
		return false;
	value = m_set[i];
	return true;
}

void Set::swap(Set& other)
{
	int tsz = m_size;
	ItemType tmpSet[DEFAULT_MAX_ITEMS];

	for(int i = 0; i < tsz; i++)
		tmpSet[i] = m_set[i];

	for(int j = 0; j < other.m_size; j++)
		m_set[j] = other.m_set[j];

	for(int i = 0; i < tsz; i++)
		other.m_set[i] = tmpSet[i];
	
	m_size = other.m_size;
	other.m_size = tsz;

}
