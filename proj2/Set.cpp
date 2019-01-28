#include "Set.h"

Set::Set()
{
  m_head = nullptr;
  m_size = 0;
}

Set::~Set()
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
  value = p->m_date;
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
  if(this->contains(value)) // CHECKS IF ALREADY IN SET
    return false;

  Node* p;
  Node* prv;
  Node* newGuy = new Node();
  newGuy->m_data = value;

  int c = 0;

  for(p = m_head; c < m_size; p = p->next)
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
  if(!(this->contains(value)))
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
    m_head = m_head->next;
  }

  Node* nxt = p->m_next;
  Node* prv = p->m_prev;

  prv->m_next = nxt;
  nxt->m_prev = prv;

  delete p;
  --m_size;
  return true;
}
