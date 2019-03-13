#include <iostream>
struct Node
{
  int m_val;
  Node* m_children[2];
  Node* m_parent;
};

int main()
{
  Node a;
  a.m_val = 5;
  a.m_children[0] = nullptr;
  a.m_children[1] = nullptr;
  std::cout << a.m_val << std::endl;
}
