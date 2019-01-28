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

  bool empty() const;
  int size() const;
  bool insert(const ItemType& value);
  bool erase(const ItemType& value);
  bool contains(const ItemType& value) const;
  bool get(int pos, ItemType& value) const;
  void swap(Set& other);

private:
  struct Node // Class for node in linked list.
  {
    ItemType m_data;
    Node* m_next;
    Node* m_prev;
  };
  Node* m_head; // Pointer to starting node.
  int m_size; // Stores size;
};

void unite(const Set& s1, const Set& s2, Set& result);
void subtract(const Set& s1, const Set& s2, Set& result);
#endif // SET_INCLUDED
