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
