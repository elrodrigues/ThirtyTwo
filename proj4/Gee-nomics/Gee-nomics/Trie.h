#ifndef TRIE_INCLUDED
#define TRIE_INCLUDED

#include <string>
#include <vector>

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
      Node* m_children[4];
    };
    Node* m_root;

    void deleteAllNodes(Node*& ptr);
    bool emptyChildren(Node*& ptr) const;
    Node* findByRecursion(const std::string& key, bool exactMatchOnly, int& pos, Node* ptr) const;
};
// PRIVATE: ARRAY VER
template<typename ValueType>


template<typename ValueType>
void Trie<ValueType>::deleteAllNodes(Node*& ptr)
{
  if()
    return;
  for(std::size_t t = 0; t < ptr->m_children.size(); t++)
  {
    deleteAllNodes(ptr->m_children[t]);
    delete ptr->m_children[t];
    --t;
  }
}

template<typename ValueType>
typename Trie<ValueType>::Node* Trie<ValueType>::findByRecursion(const std::string& key,
  bool exactMatchOnly, int& pos, Node* ptr) const
{ // Make position, ptr ref. Set return to bool instead.
  if(ptr == nullptr)
    return nullptr;
  for(int i = 0; i < ptr->m_children.size; i++)
  {

  }
  return nullptr;
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
  findByRecursion(key, exactMatchOnly, 0, m_root);
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
