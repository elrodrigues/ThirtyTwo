#include "provided.h"
#include <string>
#include <cctype>
#include <vector>
#include <iostream>
#include <istream>
using namespace std;

class GenomeImpl
{
public:
    GenomeImpl(const string& nm, const string& sequence);
    static bool load(istream& genomeSource, vector<Genome>& genomes);
    int length() const;
    string name() const;
    bool extract(int position, int length, string& fragment) const;
private:
    string m_name; // name
    string m_seq; // DNA sequence
    int m_size; // size of sequence
};

GenomeImpl::GenomeImpl(const string& nm, const string& sequence)
: m_name(nm), m_seq(sequence)
{
    m_size = m_seq.size(); // Set to sequence size.
}

bool GenomeImpl::load(istream& genomeSource, vector<Genome>& genomes)
{
    string s; // Line from file
    bool flag_hasName = false; // has a Name
    bool flag_hasSeq = false; // has a Sequence
    string name;
    string seq;
    while(getline(genomeSource, s))
    {
      if(s == "" || s == ">") // Check Format
        return false;
      if(!flag_hasName && s[0] != '>')
        return false;
      if(s[0] == '>')
      {
        if(flag_hasName) // If at next Genome
        {
          genomes.push_back(Genome(name, seq)); // Push current one in
          name = ""; // Reset
          seq = "";
          flag_hasSeq = false;
        }
        else
        {
          flag_hasName = true; // Set flag to true
        }
        name = s.substr(1, s.size() - 1);
      }
      else
      {
        flag_hasSeq = true;
        for(int i = 0; i < s.size(); i++) // Go Through Line
          switch(s[i]){
            case 'A':
            case 'a':
            case 'C':
            case 'c':
            case 'G':
            case 'g':
            case 'T':
            case 't':
            case 'N':
            case 'n':
              seq += toupper(s[i]); break; // Add base to sequence
            default:
              return false; // Wrong base
          }
      }
    }
    if(!flag_hasSeq) // Error checking
      return false;
    else
      genomes.push_back(Genome(name, seq)); // Add final genome
    return true;
}

int GenomeImpl::length() const
{
    return m_size;
}

string GenomeImpl::name() const
{
    return m_name;
}

bool GenomeImpl::extract(int position, int length, string& fragment) const
{
    if(position >= m_size || position < 0 || length < 0) // Error checking
      return false;
    if(position + length > m_size)
      return false;
    fragment = "";
    for(int i = 0; i < length; i++) // Extract Fragment
      fragment += m_seq[position + i];
    return true;
}

//******************** Genome functions ************************************

// These functions simply delegate to GenomeImpl's functions.
// You probably don't want to change any of this code.

Genome::Genome(const string& nm, const string& sequence)
{
    m_impl = new GenomeImpl(nm, sequence);
}

Genome::~Genome()
{
    delete m_impl;
}

Genome::Genome(const Genome& other)
{
    m_impl = new GenomeImpl(*other.m_impl);

}
Genome& Genome::operator=(const Genome& rhs)
{
    GenomeImpl* newImpl = new GenomeImpl(*rhs.m_impl);
    delete m_impl;
    m_impl = newImpl;
    return *this;
}

bool Genome::load(istream& genomeSource, vector<Genome>& genomes)
{
    return GenomeImpl::load(genomeSource, genomes);
}

int Genome::length() const
{
    return m_impl->length();
}

string Genome::name() const
{
    return m_impl->name();
}

bool Genome::extract(int position, int length, string& fragment) const
{
    return m_impl->extract(position, length, fragment);
}
