#include "provided.h"
#include "Trie.h"
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
using namespace std;

class GenomeMatcherImpl
{
public:
    GenomeMatcherImpl(int minSearchLength);
    void addGenome(const Genome& genome);
    int minimumSearchLength() const;
    bool findGenomesWithThisDNA(const string& fragment, int minimumLength, bool exactMatchOnly, vector<DNAMatch>& matches) const;
    bool findRelatedGenomes(const Genome& query, int fragmentMatchLength, bool exactMatchOnly, double matchPercentThreshold, vector<GenomeMatch>& results) const;
private:
  Trie<int> m_t;
  vector<Genome> m_genomes;
  int m_searchLength;

  int checkSequence(int number, int position, const string& fragment, int minLength,
    bool exactMatchOnly, string& res) const;
};
// PRIVATE
int GenomeMatcherImpl::checkSequence(int number, int position, const string& fragment, int minLength,
  bool exactMatchOnly, string& res) const
{
  int countMismatch = 0;
  int len = minLength;
  string postTree;
  bool flag_errorCaught = false;
  bool flag_Mismatch = false;
  while(m_genomes[number].extract(position, len, res))
  {
    postTree = fragment.substr(0, len);
    if(res != postTree)
    {
      if(exactMatchOnly)
      {
        flag_errorCaught = true;
        break;
      }
      for(int k = 0; k < len; k++)
      {
        if(res[k] != postTree[k])
          countMismatch++;
        if(countMismatch == 2)
        {
          flag_errorCaught = true;
          // cerr << "loop: " << res << endl;
          flag_Mismatch = true;
          break;
        }
      }
      if(flag_Mismatch)
        break;
    }
    if(len >= fragment.size())
      break;
    len++;
    countMismatch = 0;
  }
  if(flag_errorCaught)
    res = res.substr(0, res.size() - 1);
  // cerr << res << endl;
  return res.size();
}

// PUBLIC
GenomeMatcherImpl::GenomeMatcherImpl(int minSearchLength)
{
    (minSearchLength > 0) ? (m_searchLength = minSearchLength) : (m_searchLength = 1);
}

void GenomeMatcherImpl::addGenome(const Genome& genome)
{
    m_genomes.push_back(genome);
    int pos = m_genomes.size() - 1;
    int i = 0;
    string frag;
    while(m_genomes[pos].extract(i, m_searchLength, frag))
    {
      m_t.insert(frag, pos);
      m_t.insert(frag, i);
      ++i;
    }
}

int GenomeMatcherImpl::minimumSearchLength() const
{
    return m_searchLength;
}

bool GenomeMatcherImpl::findGenomesWithThisDNA(const string& fragment, int minimumLength, bool exactMatchOnly, vector<DNAMatch>& matches) const
{
    int frag_size = fragment.size();
    bool flag_matchFound = false;
    if(m_searchLength > minimumLength)
      return false;
    if(frag_size < minimumLength || frag_size < m_searchLength)
      return false;
    string trieFrag = fragment.substr(0, m_searchLength);
    vector<int> pValues = m_t.find(trieFrag, exactMatchOnly);

    for(size_t i = 0; i < pValues.size(); i += 2)
    {
      bool flag_noNewReplacement = false;
      int genNum = pValues[i];
      int genPos = pValues[i + 1];
      string resSeq;
      int N = checkSequence(genNum, genPos, fragment, minimumLength, exactMatchOnly, resSeq);
      if(N >= minimumLength)
      {
        for(vector<DNAMatch>::iterator p = matches.begin(); p != matches.end(); p++)
        {
          if((*p).genomeName == m_genomes[genNum].name())
          {
            if(N > (*p).length)
              matches.erase(p);
            else
              flag_noNewReplacement = true;
            break;
          }
        }
        if(flag_noNewReplacement)
          continue;
        flag_matchFound = true;
        DNAMatch match;
        match.genomeName = m_genomes[genNum].name();
        match.length = N;
        match.position = genPos;
        matches.push_back(match);
      }
    }
    return flag_matchFound;  // This compiles, but may not be correct
}

bool GenomeMatcherImpl::findRelatedGenomes(const Genome& query, int fragmentMatchLength, bool exactMatchOnly, double matchPercentThreshold, vector<GenomeMatch>& results) const
{
    return false;  // This compiles, but may not be correct
}

//******************** GenomeMatcher functions ********************************

// These functions simply delegate to GenomeMatcherImpl's functions.
// You probably don't want to change any of this code.

GenomeMatcher::GenomeMatcher(int minSearchLength)
{
    m_impl = new GenomeMatcherImpl(minSearchLength);
}

GenomeMatcher::~GenomeMatcher()
{
    delete m_impl;
}

void GenomeMatcher::addGenome(const Genome& genome)
{
    m_impl->addGenome(genome);
}

int GenomeMatcher::minimumSearchLength() const
{
    return m_impl->minimumSearchLength();
}

bool GenomeMatcher::findGenomesWithThisDNA(const string& fragment, int minimumLength, bool exactMatchOnly, vector<DNAMatch>& matches) const
{
    return m_impl->findGenomesWithThisDNA(fragment, minimumLength, exactMatchOnly, matches);
}

bool GenomeMatcher::findRelatedGenomes(const Genome& query, int fragmentMatchLength, bool exactMatchOnly, double matchPercentThreshold, vector<GenomeMatch>& results) const
{
    return m_impl->findRelatedGenomes(query, fragmentMatchLength, exactMatchOnly, matchPercentThreshold, results);
}
