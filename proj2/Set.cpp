#include "Set.h"

Set::Set() // CONSTRUCTOR
{
  m_head = nullptr; // Set head to nullptr. No node to point to.
  m_size = 0; // Nodes initialized in insert(), so size 0.
}

Set::~Set() // DESTRUCTOR
{
  Node* p = m_head; // Start at head.
  if(m_head != nullptr) // Checks if set is non-empty.
  {
    p = p->m_prev; // Sets p to "last" node. Deletion is backwards.
    Node* temp; // Pointer for swapping mechanism below.

    while(p != m_head) // Circular list, so stop at head.
    {
      temp = p->m_prev; // Set to previous node
      delete p; // Delete current node.
      p = temp; // Set p to previous node
    }
    delete p; // Finally, delete head node.
  }
}

Set::Set(const Set& other) // COPY CONSTRUCTOR
{
  Node* p;
  int c = 0; // Counter used to ensure one-time traversal of list.
  ItemType val; // Holds data of node.
  m_head = nullptr; // Initializes head to nullptr. See insert() for more info.
  for(p = other.m_head; c < other.m_size; p = p->m_next)
  {
    other.get(c, val);
    c++;
    insert(val); // Calls insert() which handles m_head initialization.
  }
  m_size = other.m_size; // Redundant, but just in case.
}

Set& Set::operator=(const Set& rhs) // ASSIGNMENT OVERLOADER
{
  Set temp(rhs); // Copy-and-Swap
  swap(temp);
  return *this; // Return reference/address.
}

bool Set::empty() const // EMPTY
{
  return (m_head == nullptr); // Could also use m_size == 0.
}

int Set::size() const // SIZE
{
  return m_size; // Return size.
}

bool Set::contains(const ItemType& value) const // CONTAINS
{
  Node* p;
  int c = 0; //  Counter for one-time list traversal
  if(m_head == nullptr)
    return false; // If list empty, don't bother checking.
  for(p = m_head; c < m_size && p->m_data != value; p = p->m_next)
  {
    c++; // Loop checks for value. See condition p->m_data in loop.
  }
  return (c != m_size); // If item not there, then full list traversed.
}

bool Set::get(int pos, ItemType& value) const // GET
{
  if(m_head == nullptr) // If set empty, don't bother checking
    return false;

  Node* p = m_head; // Start at head
  if(pos < 0 || pos >= m_size) // Boundary condition checker
    return false;
  for(int i = 0; i < pos; i++)
    p = p->m_next; // Get pointer for that position
  value = p->m_data; // Set value contained in node.
  return true;
}

bool Set::insert(const ItemType& value) // INSERT (the absolute unit)
{
  if(m_head == nullptr) // *INITIALIZES A SET* when Set is empty
  {
    m_head = new Node(); // Create new node with head as pointer.
    m_size = 1; // Set size to 1.
    m_head->m_next = m_head; // Next node is itself. Circular design
    m_head->m_prev = m_head; // Previous node is itself. Circular design
    m_head->m_data = value; // Set value
    return true;
  }
  if(contains(value)) // CHECKS IF ALREADY IN SET
    return false;

  Node* p; // Traversing pointer
  Node* prv; // Pointer previous to p.
  Node* newGuy = new Node(); // Pointer to new node.
  newGuy->m_data = value; // Initialize value

  int c = 0; // Counter for 1-time list traversal

  for(p = m_head; c < m_size; p = p->m_next)
  {
    c++;
    if(p->m_data > value) // INSERTION if not at end.
    {

      prv = p->m_prev; // Get previous pointer
      newGuy->m_next = p; // Set new node's next to current.
      newGuy->m_prev = prv; // Set new node's prev to one before current.

      prv->m_next = newGuy; // Set prev's next ptr to new node.

      p->m_prev = newGuy; // Set current node's next ptr to new node.
      if(p == m_head) // IF INSERTION AT HEAD
      {
        m_head = newGuy; // Change head pointer to new node.
      }
      ++m_size; // Increase size by 1
      return true;
    }
  }
  // INSERTION AT END OF LIST
  p = p->m_prev; // Set to last node
  newGuy->m_next = m_head; // Same insertion as in loop.
  newGuy->m_prev = p;

  p->m_next = newGuy;

  m_head->m_prev = newGuy;

  ++m_size;

  return true;
}

bool Set::erase(const ItemType& value) // ERASE
{
  if(!(contains(value))) // If set doesn't have value, nothing to erase
  {
    return false;
  }

  Node* p;
  for(p = m_head; p->m_data != value; p=p->m_next); // Find ptr for value.
  if(p == m_head) // ERASE IF AT HEAD POINTER
  {
    if(m_size == 1) // SPECIAL CASE: Last element in list.
    {
      delete m_head; // Delete last item
      m_head = nullptr; // Set head to nullptr
      m_size = 0; // Size to 0
      return true; // Stop function
    }
    m_head = m_head->m_next; // Else, move head to next node.
  }

  Node* nxt = p->m_next; // Fetch next node
  Node* prv = p->m_prev; // Fetch previous node

  prv->m_next = nxt; // Change next pointers in list.
  nxt->m_prev = prv; // CHange prev pointers in list.

  delete p; // Delete current node
  --m_size; // Reduce size by 1.
  return true;
}

void Set::swap(Set& other) // SWAP
{
  int tsz = other.m_size; // Swap size
  other.m_size = m_size;
  m_size = tsz;

  Node* temp = other.m_head; // Swap heads
  other.m_head = m_head;
  m_head = temp;
} // Only need to swap heads and sizes, list can stay as is.

//// NON-MEMBER FUNCTIONS
void unite(const Set& s1, const Set& s2, Set& result) // UNITE
{
  Set ts1(s1);
  Set ts2(s2); // ALIASING: takes care of aliasing problem

  ItemType val;

  while(!(result.empty())) // EMPTY RESULT: case where Set result is non-empty
  {
    result.get(0, val); // Erase at head pointer
    result.erase(val);
  }
   // UNION OPERATION
  for(int i = 0; i < ts1.size(); i++)
  {
    ts1.get(i, val);
    result.insert(val); // Insert s1 copy into result
  }
  for(int i = 0; i < ts2.size(); i++)
  {
    ts2.get(i, val); // Insert s2 copy into result
    result.insert(val); // Uses insert()'s logic to prevent duplicates.
  }
}

void subtract(const Set& s1, const Set& s2, Set& result) // SUBTRACT
{
  Set ts1(s1);
  Set ts2(s2); // ALIASING: takes care of aliasing problem

  ItemType val;

  while(!(result.empty())) // EMPTY RESULT: useful if result is non-empty
  {
    result.get(0, val);
    result.erase(val);
  }

  for(int i = 0; i < ts1.size(); i++) // MINUS OP
  {
    ts1.get(i, val);
    if(!(ts2.contains(val))) // Uses contains() for subtract logic.
      result.insert(val);
  }
}
