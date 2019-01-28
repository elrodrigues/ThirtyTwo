#include "Set.h"

Set::Set() // CONSTRUCTOR
{
  m_head = nullptr;
  m_size = 0;
}

Set::~Set() // DESTRUCTOR
{
  Node* p = m_head;
  p = p->m_prev;
  Node* temp; // Swapping mech.

  while(p != m_head)
  {
    temp = p->m_prev;
    delete p;
    p = temp;
  }
  delete p;
}

Set::Set(const Set& other) // COPY CONSTRUCTOR
{
  Node* p;
  int c = 0;
  for(p = other.m_head; c < other.m_size; p = p->m_next)
  {
    c++;
    insert(p->m_data);
  }
  m_size = other.m_size; // Redundant, but just in case.
}

Set& Set::operator=(const Set& rhs) // ASSIGNMENT OVERLOADER
{
  Set temp(rhs);
  swap(temp);
  return *this;
}

bool Set::empty() const
{
  return (m_head == nullptr);
}

int Set::size() const
{
  return m_size;
}

bool Set::contains(const ItemType& value) const
{
  Node* p;
  int c = 0;
  if(m_head == nullptr)
    return false;
  for(p = m_head; c < m_size && p->m_data != value; p = p->m_next)
  {
    c++;
  }
  return (c == m_size);
}

bool Set::get(int pos, ItemType& value) const
{
  if(m_head == nullptr)
    return false;

  Node* p = m_head;
  if(pos < 0 || pos >= m_size)
    return false;
  for(int i = 0; i < pos; i++)
    p = p->m_next;
  value = p->m_data;
  return true;
}

bool Set::insert(const ItemType& value)
{
  if(m_head == nullptr) // "INITIALIZES A SET"
  {
    m_head = new Node();
    m_size = 1;
    m_head->m_next = m_head;
    m_head->m_prev = m_head;
    m_head->m_data = value;
    return true;
  }
  if(contains(value)) // CHECKS IF ALREADY IN SET
    return false;

  Node* p;
  Node* prv;
  Node* newGuy = new Node();
  newGuy->m_data = value;

  int c = 0;

  for(p = m_head; c < m_size; p = p->m_next)
  {
    c++;
    if(p->m_data > value) // INSERTION
    {

      prv = p->m_prev;
      newGuy->m_next = p;
      newGuy->m_prev = prv;

      prv->m_next = newGuy;

      p->m_prev = newGuy;
      if(p == m_head)
      {
        m_head = newGuy;
      }
      ++m_size;
      return true;
    }
  }
  // INSERTION AT END
  p = p->m_prev; // Set to last node
  newGuy->m_next = m_head;
  newGuy->m_prev = p;

  p->m_next = newGuy;

  m_head->m_prev = newGuy;

  ++m_size;

  return true;
}

bool Set::erase(const ItemType& value)
{
  if(!(contains(value)))
  {
    return false;
  }

  Node* p;
  for(p = m_head; p->m_data != value; p=p->m_next);
  if(p == m_head)
  {
    if(m_size == 1)
    {
      delete m_head;
      m_head = nullptr;
      m_size = 0;
      return true;
    }
    m_head = m_head->m_next;
  }

  Node* nxt = p->m_next;
  Node* prv = p->m_prev;

  prv->m_next = nxt;
  nxt->m_prev = prv;

  delete p;
  --m_size;
  return true;
}

void Set::swap(Set& other)
{
  int tsz = other.m_size; // SWAP SIZE
  other.m_size = m_size;
  m_size = tsz;

  Node* temp = other.m_head; // SWAP HEADS
  other.m_head = m_head;
  m_head = temp;
}

void unite(const Set& s1, const Set& s2, Set& result)
{
  Set ts1(s1);
  Set ts2(s2); // ALIASING

  ItemType val;

  while(!(result.empty())) // EMPTY RESULT
  {
    result.get(0, val);
    result.erase(val);
  }

  for(int i = 0; i < ts1.size(); i++) // UNION OP
  {
    ts1.get(i, val);
    result.insert(val);
  }
  for(int i = 0; i < ts2.size(); i++)
  {
    ts2.get(i, val);
    result.insert(val);
  }
}

void subtract(const Set& s1, const Set& s2, Set& result)
{
  Set ts1(s1);
  Set ts2(s2); // ALIASING

  ItemType val;

  while(!(result.empty())) // EMPTY RESULT
  {
    result.get(0, val);
    result.erase(val);
  }

  for(int i = 0; i < ts1.size(); i++) // MINUS OP
  {
    ts1.get(i, val);
    if(!(ts2.contains(val)))
      result.insert(val);
  }
}
