#ifndef TRIE_INCLUDED
#define TRIE_INCLUDED

#include <string>
#include <vector>

int NUM_BASE = 4;

template<typename ValueType>
class Trie
{
public:
    Trie();
    ~Trie();
    void reset();
    void insert(const std::string& key, const ValueType& value);
    std::vector<ValueType> find(const std::string& key, bool exactMatchOnly) const;

      // C++11 syntax for preventing copying and assignment
    Trie(const Trie&) = delete;
    Trie& operator=(const Trie&) = delete;
private:
    struct Node
    {
      std::string m_label;
      std::vector<ValueType> m_val;
      Node* m_child[NUM_BASE];
    };
    Node* m_root;
    bool hasNoChildren;
    void deleteAllNodes(Node*& ptr);
    bool isChildrenEmpty(Node*& ptr) const;
    bool findByRecursion(const std::string& key, bool exactMatchOnly, int& pos, Node* ptr, Node*& res) const;
};
// PRIVATE: ARRAY VER
template<typename ValueType>
bool Trie<ValueType>::isChildrenEmpty(Node*& ptr)
{
  return haveNoChildren;
}


template<typename ValueType>
void Trie<ValueType>::deleteAllNodes(Node*& ptr)
{
  if(ptr == nullptr || ptr->hasNoChildren)
    return;
  for(int t = 0; t < NUM_BASE; t++)
  {
    deleteAllNodes(ptr->m_child[t]);
    delete ptr->m_child[t];
  }
}

template<typename ValueType>
bool Trie<ValueType>::findByRecursion(const std::string& key,
  bool exactMatchOnly, int& pos, Node* ptr, Node*& res) const
{ // Make position, ptr ref. Set return to bool instead.
  if(pos == key.size())
  {
    res = ptr;
    return true;
  }
  if(ptr == nullptr || ptr->hasNoChildren)
    return false;
  int p = pos + 1;
  for(int i = 0; i < NUM_BASE; i++)
  {
      if(findByRecursion(key, exactMatchOnly, p, ptr->m_child[1], res))
      {

      }
  }
}

// PUBLIC
template<typename ValueType>
Trie<ValueType>::Trie()
: m_root(nullptr)
{}

template<typename ValueType>
Trie<ValueType>::~Trie()
{
  deleteAllNodes(m_root);
  delete m_root;
  std::cerr << "Trie deleted" << std::endl;
}

template<typename ValueType>
void Trie<ValueType>::insert(const std::string& key, const ValueType& value)
{
  // int index = 0;
  // Node* ptr = m_root
  // for(int i = 0; i < key.size(); i++)
  // {
  //   if()
  // }
}

template<typename ValueType>
std::vector<ValueType> Trie<ValueType>::find(const std::string& key, bool exactMatchOnly) const
{
  Node* result = nullptr;
  findByRecursion(key, exactMatchOnly, 0, m_root, result);
  std::vector<ValueType> v;
  return v;
}

template<typename ValueType>
void Trie<ValueType>::reset()
{
  deleteAllNodes(m_root);
  delete m_root;
  m_root = new Node();
}

#endif // TRIE_INCLUDED
