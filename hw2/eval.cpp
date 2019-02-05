#include "Set.h"
#include <iostream>
#include <stack>
#include <cctype>
#include <string>
using namespace std;
bool hasPred(char c1, char c2);
bool checkSyntax(string in);
int evaluate(string infix, const Set& trueValues, const Set& falseValues, string& postfix, bool& result);

bool hasPred(char c1, char c2)
{
  string pred = "|&!";
  int t1, t2;
  for(int i = 0; i < 3; i ++)
  {
    if(c1 == pred[i])
      t1 = i;
    if(c2 == pred[i])
      t2 = i;
  }
  return (t1 <= t2);
}

bool checkSyntax(string in)
{
  Set ops;
  ops.insert('&'); ops.insert('|'); ops.insert('!'); ops.insert('('); ops.insert(')');
  int sz = in.size();
  int numB = 0;
  for(int i = 0; i < sz; i++)
  {
    char t = in[i];
    if(!ops.contains(t))
    {
      if(isupper(t) || isdigit(t) || !isalnum(t))
        return false;
    }
    if(isalpha(t)) // operand check
    {
      if(i + 1 < sz && isalpha(in[i + 1]))
        return false;
      else if(i - 1 >= 0 && isalpha(in[i - 1]))
        return false;

    }
    else // operator check
    {
      if(t == '(')
      {
        numB++;
        if(i + 1 < sz && (in[i + 1] == '&' || in[i + 1] == '|'))
          return false;

      }
      else if(t == ')')
      {
        numB--;
        if(i - 1 >= 0 && (in[i - 1] == '&' || in[i - 1] == '|' || in[i - 1] == '!'))
          return false;
      }
      else if(t == '!')
      {
        if(i + 1 >= sz)
          return false;
        else if(i - 1 >= 0 && isalpha(in[i - 1]))
          return false;
        else if(in[i + 1] == '&' || in[i + 1] == '|')
          return false;
      }
      else if(t == '&' || t == '|')
      {
        if(i + 1 >=sz)
          return false;
        else if(i - 1 >= 0 && (in[i - 1] == '&' || in[i - 1] == '|' || in[i - 1] == '!'))
          return false;
        else if(in[i + 1] == '&' || in[i + 1] == '|' )
          return false;
      }
    }
  }
  return (numB == 0);
}

int evaluate(string infix, const Set& trueValues, const Set& falseValues, string& postfix, bool& result)
{
  //// INLINE SYNTAX CHECK
  string inf;
  stack<char> opstack; // Operator / Operand stack
  for(int i = 0; i < infix.size(); i++)
  {
    if(infix[i] != ' ' && infix[i] != '\t')
      inf = inf + infix[i];
  }
  if(!checkSyntax(inf))
  {
    cerr << "Bad syntax" << endl;
    return false;
  }

  //// INFIX TO POSTFIX
  postfix = "";
  for(int i = 0; i < inf.size(); i++)
  {
    char t = inf[i];
    if(isalpha(t))
      postfix = postfix + t;
    else if(t == '(')
      opstack.push(t);
    else if(t == ')')
    {
      while(opstack.top() != '(')
      {
        postfix = postfix + opstack.top();
        opstack.pop();
      }
      opstack.pop();
    }
    else
    {
      while(!opstack.empty() && opstack.top() != '(' && hasPred(t,opstack.top()))
      {
        postfix = postfix + opstack.top();
        opstack.pop();
      }
      opstack.push(t);
    }
  }
  while(!opstack.empty())
  {
    postfix = postfix + opstack.top();
    opstack.pop();
  }
  //// EVALUATE POSTFIX
  // Sanity check
  // Actual evaluation: opstack converted to operand stack now.
  char evT; // Temporary evaluation variable
  char evT2; // Temporary evaluation variable
  for(int i = 0; i < postfix.size(); i++)
  {
    char opn = postfix[i];
    if(isalpha(opn))
    {
      if(!trueValues.contains(opn) && !falseValues.contains(opn))
        return 2;
      else if(trueValues.contains(opn) && falseValues.contains(opn))
        return 3;
      if(trueValues.contains(opn))
        opstack.push('T');
      else
        opstack.push('F');
    }
    else
    {
      switch(opn){
        case '!':
          evT = opstack.top();
          opstack.pop();
          (evT == 'T') ? opstack.push('F') : opstack.push('T');
          break;
        case '|':
          evT = opstack.top();
          opstack.pop();
          evT2 = opstack.top();
          opstack.pop();
          (evT == 'F' && evT2 == 'F') ? opstack.push('F') : opstack.push('T');
          break;
        case '&':
          evT = opstack.top();
          opstack.pop();
          evT2 = opstack.top();
          opstack.pop();
          (evT == 'T' && evT2 == 'T') ? opstack.push('T') : opstack.push('F');
          break;
        default:
          cerr << "Fatal error" << endl;
      }
    }
  }
  (opstack.top() == 'T') ? (result = true) : (result = false);
  opstack.pop();
  return 0;
}

int main()
{
  Set t;
  t.insert('a');
  t.insert('c');
  t.insert('l');
  t.insert('u');
  Set f;
  f.insert('n');
  f.insert('s');
  f.insert('x');
  string in = "a&!(s|u&c|n)|!!!(s&u&n)";
  string post="bro";
  bool res;
  evaluate(in, t, f, post, res);
  cout << post << endl;
  cout << res << endl;
}
