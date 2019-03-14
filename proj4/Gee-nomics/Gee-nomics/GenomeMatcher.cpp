#include "provided.h"
#include "Trie.h"
#include <string>
#include <vector>
#include <map>
#include <algorithm>
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
  Trie<int> m_t; // Trie
  vector<Genome> m_genomes; // Vector of Genomes
  int m_searchLength; // minimum search length

  int checkSequence(int number, int position, const string& fragment, int minLength,
    bool exactMatchOnly, string& res) const; // Used in findGenomesWithThisDNA()
  static bool predicateByPercent(GenomeMatch g1, GenomeMatch g2); // Predicate for sorting by percent
  static bool predicateByName(GenomeMatch g1, GenomeMatch g2); // Predicate for sorting by name
};
// PRIVATE
int GenomeMatcherImpl::checkSequence(int number, int position, const string& fragment, int minLength,
  bool exactMatchOnly, string& res) const
{
  int countMismatch = 0; // Keep track of mismatches
  int len = minLength;
  string postTree; // name should be changed.
  bool flag_errorCaught = false; // Flag to check if last character should be removed
  bool flag_Mismatch = false; // Flag to check for Mismatch
  while(m_genomes[number].extract(position, len, res)) // gets largest possible sequence
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
        if(countMismatch == 2) // Can't tolerate two mismatches of SNiP allowed
        {
          flag_errorCaught = true;
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
  if(flag_errorCaught) // Correct sequence
    res = res.substr(0, res.size() - 1);
  return res.size();
}
bool GenomeMatcherImpl::predicateByPercent(GenomeMatch g1, GenomeMatch g2) // STATIC
{
  return g1.percentMatch >= g2.percentMatch;
}
bool GenomeMatcherImpl::predicateByName(GenomeMatch g1, GenomeMatch g2) // STATIC
{
  if(g1.percentMatch == g2.percentMatch)
    return g1.genomeName <= g2.genomeName;
  return false;
}
// PUBLIC
GenomeMatcherImpl::GenomeMatcherImpl(int minSearchLength)
{
    (minSearchLength > 0) ? (m_searchLength = minSearchLength) : (m_searchLength = 1); // Error Checking
}

void GenomeMatcherImpl::addGenome(const Genome& genome)
{
    m_genomes.push_back(genome); // Add genome
    int pos = m_genomes.size() - 1;
    int i = 0;
    string frag;
    while(m_genomes[pos].extract(i, m_searchLength, frag)) // For each sequence of size m_searchLength
    {
      m_t.insert(frag, pos); // Add Genome Index in m_genomes
      m_t.insert(frag, i); // Add position of frag in Genome sequence
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
    bool flag_matchFound = false; // flag for match found
    if(m_searchLength > minimumLength) // Error checking
      return false;
    if(frag_size < minimumLength || frag_size < m_searchLength)
      return false;
    string trieFrag = fragment.substr(0, m_searchLength); // Get fragment of fragment of size = depth of Trie
    vector<int> pValues = m_t.find(trieFrag, exactMatchOnly); // Get vector containing Genome Index and DNA sequence positions

    for(size_t i = 0; i < pValues.size(); i += 2)
    {
      bool flag_noNewReplacement = false;
      int genNum = pValues[i]; // Index and Sequence positions are stored next to each other, guaranteed by addGenome()
      int genPos = pValues[i + 1];
      string resSeq;
      int N = checkSequence(genNum, genPos, fragment, minimumLength, exactMatchOnly, resSeq); // Check full fragment now.
      if(N >= minimumLength)
      {
        for(vector<DNAMatch>::iterator p = matches.begin(); p != matches.end(); p++)
        {
          if((*p).genomeName == m_genomes[genNum].name()) // Check for duplicates
          {
            if(N > (*p).length)
              matches.erase(p);
            else
              flag_noNewReplacement = true; // Nothing to replace
            break;
          }
        }
        if(flag_noNewReplacement)
          continue; // Nothing to replace so go to next Genome Index and sequence position
        flag_matchFound = true; // A match is found
        DNAMatch match; // Add new DNAMatch to matches
        match.genomeName = m_genomes[genNum].name();
        match.length = N;
        match.position = genPos;
        matches.push_back(match);
      }
    }
    return flag_matchFound;
}

bool GenomeMatcherImpl::findRelatedGenomes(const Genome& query, int fragmentMatchLength, bool exactMatchOnly, double matchPercentThreshold, vector<GenomeMatch>& results) const
{
    if(fragmentMatchLength < m_searchLength) // Error Checking
      return false;
    int numSeq = query.length() / fragmentMatchLength; // Total number of possible matches
    if(numSeq < 1)
      return false;
    if(matchPercentThreshold < 0 || matchPercentThreshold > 100)
      return false;
    string fragment;
    results.erase(results.begin(), results.end()); // Clear results vector
    map<string, double> matches; //  Pair of Genome Name and Number of Matches
    int adjustedLength = numSeq * fragmentMatchLength; // Number of bases to look at.
    for(int i = 0; i < adjustedLength; i += fragmentMatchLength)
    {
      query.extract(i, fragmentMatchLength, fragment); // Get fragment of length fragmentMatchLength
      vector<DNAMatch> similarGenomes;
      bool found = findGenomesWithThisDNA(fragment, fragmentMatchLength, exactMatchOnly, similarGenomes);
      if(found)
      {
        for(size_t k = 0; k < similarGenomes.size(); k++)
        {
          if(matches.find(similarGenomes[k].genomeName) == matches.end()) // Cound number of matches
            matches[similarGenomes[k].genomeName] = 0.0;
          matches[similarGenomes[k].genomeName] += 1.0;
        }
      }
    }
    for(map<string, double>::iterator p = matches.begin(); p != matches.end(); p++) // Iterate through map
    {
      double percent = (p->second / numSeq)*100;
      if(percent >= matchPercentThreshold) // add GenomeMatch objects to results
      {
        GenomeMatch gMatch;
        gMatch.genomeName = p->first;
        gMatch.percentMatch = percent;
        results.push_back(gMatch);
      }
    }
    sort(results.begin(), results.end(), GenomeMatcherImpl::predicateByPercent); // Sort by Percent in descending
    sort(results.begin(), results.end(), GenomeMatcherImpl::predicateByName); // Sort by Name in ascending if needed
    return results.size() > 0;
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
