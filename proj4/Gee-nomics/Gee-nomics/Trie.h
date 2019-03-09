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

    //// DELETE!
    bool test(const std::string& key) const;
private:
    struct Node
    {
      Node();
      char m_label;
      std::vector<ValueType> m_val;
      std::vector<Node*> m_child;
      bool hasNoChildren;
    };
    Node* m_root;

    void deleteAllNodes(Node*& ptr);
    int findByRecursion(const std::string& key, bool exactMatchOnly, int pos, Node* ptr, Node*& res) const;
    Node* insertNode(Node* ptr, char c);


};
// PRIVATE
template<typename ValueType>
Trie<ValueType>::Node::Node()
: hasNoChildren(true)
{}

template<typename ValueType> // DELETE
bool Trie<ValueType>::test(const std::string& key) const
{
  Node* p = nullptr;
  bool f = findByRecursion(key, true, 0, m_root, p) == key.size();
  for(size_t i = 0; i < p->m_val.size(); i++)
    std::cerr << p->m_val[i] << std::endl;
  return f;
}

template<typename ValueType>
void Trie<ValueType>::deleteAllNodes(Node*& ptr)
{
  if(ptr == nullptr || ptr->hasNoChildren)
    return;
  for(std::size_t t = 0; t < ptr->m_child.size(); t++)
  {
    deleteAllNodes(ptr->m_child[t]);
    delete ptr->m_child[t];
  }
}

template<typename ValueType>
int Trie<ValueType>::findByRecursion(const std::string& key,
  bool exactMatchOnly, int pos, Node* ptr, Node*& res) const
{
  if(pos == key.size() || ptr == nullptr || ptr->hasNoChildren)
  {
    res = ptr;
    return pos;
  }
  int low = 0; int high = ptr->m_child.size() - 1;
  int mid = (low + high) / 2;
  while(low <= high)
  {
    char c = ptr->m_child[mid]->m_label;
    if(c == key[pos])
      return findByRecursion(key, exactMatchOnly, pos + 1, ptr->m_child[mid], res);
    (c < key[pos]) ? (low = mid + 1) : (high = mid - 1);
    mid = (low + high) / 2;
  }
  res = ptr;
  return pos;
}

template<typename ValueType>
typename Trie<ValueType>::Node* Trie<ValueType>::insertNode(Node* ptr, char c)
{
  Node* n = new Node();
  n->m_label = c;
  if(ptr->m_child.size() == 0)
  {
    ptr->m_child.push_back(n);
    ptr->hasNoChildren = false;
  }
  else
  {
    for(typename std::vector<Node*>::iterator p = ptr->m_child.begin(); p != ptr->m_child.end(); p++)
      if((*p)->m_label > c)
      {
        ptr->m_child.insert(p, n);
        break;
      }
  }
  return n;
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
  if(m_root == nullptr)
  {
    m_root = new Node();
    m_root->m_child.push_back(new Node());
    m_root->m_child[0]->m_label = key[0];
    m_root->hasNoChildren = false;
  }
  Node* r = nullptr;
  int p = findByRecursion(key, true, 0, m_root, r);
  for(; p < key.size(); p++)
  {
    r = insertNode(r, key[p]);
  }
  r->m_val.push_back(value);
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
