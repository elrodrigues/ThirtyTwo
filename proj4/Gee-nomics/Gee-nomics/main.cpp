#include <iostream>
#include <fstream>
#include "Trie.h"
#include "provided.h"
using namespace std;

int main()
{

  Trie<int>* t = new Trie<int>;
  t->insert("hit", 1);
  t->insert("hit", 2);
  t->insert("hip", 10);
  t->insert("hip", 20);
  t->insert("hi", 9);
  t->insert("hi", 17);
  t->insert("hat", 10);
  t->insert("hat", 2);
  t->insert("hat", 5);
  t->insert("hat", 10);
  t->insert("a", 14);
  t->insert("to", 22);
  t->insert("to", 23);
  t->insert("hap", 19);
  t->insert("hap", 6);
  t->insert("hap", 32);

  vector<int> val = t->find("hit", true);
  for(size_t k = 0; k < val.size(); k++)
    cerr << val[k] << ",";
  cerr << endl;
  vector<int> val2 = t->find("hit", false);
  for(size_t k = 0; k < val2.size(); k++)
    cerr << val2[k] << ",";
  cerr << endl;
  vector<int> val3 = t->find("hkt", false);
  for(size_t k = 0; k < val3.size(); k++)
    cerr << val3[k] << ",";
  cerr << endl;
  t->reset();
  t->insert("ACTG", 42);
  val3 = t->find("ACTG", false);
  for(size_t k = 0; k < val3.size(); k++)
    cerr << val3[k] << ",";
  cerr << endl;
  val3 = t->find("hkt", false);
  for(size_t k = 0; k < val3.size(); k++)
    cerr << val3[k] << ",";
  // Specify the full path and name of the gene data file on your hard drive.
  string filename = "../data/Ferroplasma_acidarmanus.txt";
  // Open the data file and get a ifstreamobject that can be used to read its// contents.
  ifstream strm(filename);
  if (!strm)
  {
    cout << "Cannot open " << filename << endl;
    delete t;
    return -1;
  }
  vector<Genome> vg;
  bool success = Genome::load(strm, vg);
  // Load the data via the stream.
  if (success)
  {
    cout << "Loaded " << vg.size() << " genomes successfully:"<< endl;
    for (int k = 0; k != vg.size(); k++)
      cout << vg[k].name() << endl;
  }
  else
    cout << "Error loading genome data"<< endl;

  cerr << endl << "Test Passed" << endl;
  delete t;

}
