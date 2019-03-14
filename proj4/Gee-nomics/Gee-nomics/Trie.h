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
      Node();
      char m_label; // label
      std::vector<ValueType> m_val; // vector of values
      std::vector<Node*> m_child; // vector of children
      bool hasNoChildren;
    };
    Node* m_root;

    void deleteAllNodes(Node*& ptr); // private function to recursively delete nodes.
    int findByRecursion(const std::string& key, bool exactMatchOnly, int pos, Node* ptr, std::vector<Node*>& res) const; // find Node with recursion
    Node* insertNode(Node* ptr, char c); // private function to insert new Node appropriately


};
// PRIVATE
template<typename ValueType>
Trie<ValueType>::Node::Node()
: hasNoChildren(true) // Constructor sets hasNoChildren to true for each Node
{}

template<typename ValueType>
void Trie<ValueType>::deleteAllNodes(Node*& ptr)
{
  if(ptr == nullptr || ptr->hasNoChildren) // end of recursion
    return;
  for(std::size_t t = 0; t < ptr->m_child.size(); t++)
  {
    deleteAllNodes(ptr->m_child[t]); // Find the bottom level of tree recusively
    delete ptr->m_child[t]; // Then delete dynamically allocated Node
  }
}

template<typename ValueType>
int Trie<ValueType>::findByRecursion(const std::string& key, bool exactMatchOnly, int pos, Node* ptr, std::vector<Node*>& res) const
{
  if(pos == key.size() || ptr == nullptr || ptr->hasNoChildren) // end of recursion
  {
    res.push_back(ptr); // add leaf to res
    return pos; // return current index of the key
  }
  int low = 0; int high = ptr->m_child.size() - 1;
  int mid = (low + high) / 2;
  int p = pos;
  while(low <= high) // Binary Search through a tree
  {
    char c = ptr->m_child[mid]->m_label; // Get label of child
    if(c == key[pos])
    {
      p = findByRecursion(key, exactMatchOnly, pos + 1, ptr->m_child[mid], res); // Move to next index of key
      break;
    }
    (c < key[pos]) ? (low = mid + 1) : (high = mid - 1);
    mid = (low + high) / 2;
  }
  if(p == pos) // if loop above cannot find character
    res.push_back(ptr); // Add current node to res. Useful for other functions
  if(!exactMatchOnly && pos > 0) // FOR exactMatchOnly = FALSE
  {
    int oth_p;
    for(std::size_t i = 0; i < ptr->m_child.size(); i++)
    {
      if(ptr->m_child[i]->m_label != key[pos]) // Check other possible sequences
      {
        oth_p = findByRecursion(key, true, pos + 1, ptr->m_child[i], res); // set exactMatchOnly parameter to true for next call.
        if(oth_p != key.size()) // If sequence not the same size as key
          res.pop_back(); // Remove sequence from res.
      }
    }
  }
  return p;
}

template<typename ValueType>
typename Trie<ValueType>::Node* Trie<ValueType>::insertNode(Node* ptr, char c)
{
  Node* n = new Node();
  n->m_label = c;
  if(ptr->m_child.size() == 0) // If Node has no children. Should change to hasNoChildren instead.
  {
    ptr->m_child.push_back(n);
    ptr->hasNoChildren = false;
  }
  else
  {
    bool flag = true; // Flag to check for insertion
    for(typename std::vector<Node*>::iterator p = ptr->m_child.begin(); p != ptr->m_child.end(); p++)
      if((*p)->m_label > c)
      {
        ptr->m_child.insert(p, n); // Insert in sorted order
        flag = false; // Set flag to false
        break;
      }
    if(flag) // So if at end of children vector
      ptr->m_child.insert(ptr->m_child.end(), n); // Add at end
  }
  return n; // Return pointer to new Node
}
// PUBLIC
template<typename ValueType>
Trie<ValueType>::Trie()
: m_root(nullptr)
{}

template<typename ValueType>
Trie<ValueType>::~Trie()
{
  deleteAllNodes(m_root); // Delete Nodes recursively
  delete m_root; // Delete root node
}

template<typename ValueType>
void Trie<ValueType>::insert(const std::string& key, const ValueType& value)
{
  if(m_root == nullptr) // If empty Trie
  {
    m_root = new Node(); // Create root node
    m_root->m_child.push_back(new Node());
    m_root->m_child[0]->m_label = key[0];
    m_root->hasNoChildren = false;
  }
  std::vector<Node*> vec_r;
  int p = findByRecursion(key, true, 0, m_root, vec_r); // Get Index where that character of key not found
  Node* r = vec_r[0]; // Set to last node (leaf) in Trie that satisfies key
  for(; p < key.size(); p++) // Go through remaining fragment of key
  { // If p = key.size(), then full key found in Trie, so skip loop
    r = insertNode(r, key[p]); // Insert new Node with label at that Index p of key
  }
  r->m_val.push_back(value); // Push value to Node.
}

template<typename ValueType>
std::vector<ValueType> Trie<ValueType>::find(const std::string& key, bool exactMatchOnly) const
{
  std::vector<Node*> result;
  std::vector<ValueType> v;
  int pos = 0;
  pos = findByRecursion(key, exactMatchOnly, pos, m_root, result); // Get all nodes that fit key and SNiP parameter
  for(std::size_t i = 0; i < result.size(); i++)
  {
    v.insert(v.end(), result[i]->m_val.begin(), result[i]->m_val.end()); // Add values at end of vector
  }
  return v;
}

template<typename ValueType>
void Trie<ValueType>::reset()
{
  deleteAllNodes(m_root); // Recursively delete all nodes in Trie
  delete m_root; // Delete root node
  m_root = new Node(); // Assign new root node.
}

#endif // TRIE_INCLUDED
