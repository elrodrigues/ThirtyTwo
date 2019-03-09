#include <iostream>
#include "Trie.h"
using namespace std;

int main()
{
  Trie<int>* t = new Trie<int>;
  t->insert("ATG", 4);
  cerr << t->test("ATG") << endl;
  t->insert("ACG", 9);
  cerr << t->test("ACG") << endl;
  t->insert("ACG", 69);
  cerr << t->test("ACG") << endl;
  t->insert("ATG", 420);
  cerr << t->test("ATG") << endl;
  cerr << t->test("AT") << endl;

  t->insert("ACGTG", 55);
  cerr << t->test("ACGTG") << endl;
  cerr << "Test Passed" << endl;
  delete t;
}
