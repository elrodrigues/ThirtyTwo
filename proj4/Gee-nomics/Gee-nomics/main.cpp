#include <iostream>
#include "Trie.h"
using namespace std;

int main()
{
  Trie<int>* test = new Trie<int>;
  test->reset();
  cerr << "Test Passed" << endl;
  delete test;
}
