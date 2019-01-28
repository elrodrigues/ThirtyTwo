#ifndef SET_INCLUDED
#define SET_INCLUDED
#include <string>

using ItemType = std::string;

class Set
{
public:
  Set();
  ~Set(); // Destructor
  Set(const Set& other); // Copy Constructor
  Set& operator=(const Set& rhs); // Assignment Overloader

  bool empty() const; // Empty func
  int size() const; // Size func
  bool insert(const ItemType& value); // Insert func
  bool erase(const ItemType& value); // Erase func
  bool contains(const ItemType& value) const; // Contains func
  bool get(int pos, ItemType& value) const; // Get func
  void swap(Set& other); // Swapper func

private:
  struct Node // Class for node in circular doubly-linked list.
  {
    ItemType m_data; // Holds data of type ItemType.
    Node* m_next; // Points to next node
    Node* m_prev; // Points to previous node
  };
  Node* m_head; // Pointer to starting node.
  int m_size; // Stores size;
};
///// NON-MEMBER FUNCTIONS
void unite(const Set& s1, const Set& s2, Set& result);
void subtract(const Set& s1, const Set& s2, Set& result);
#endif // SET_INCLUDED
