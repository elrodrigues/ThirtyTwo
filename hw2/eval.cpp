
#include "Set.h"  // with ItemType being a type alias for char
#include <iostream>
#include <string>
#include <stack>
#include <cctype>
#include <cassert> // Needed for tests!
using namespace std;
bool hasPred(char c1, char c2);
bool checkSyntax(string in);

int evaluate(string infix, const Set& trueValues, const Set& falseValues,
  string& postfix, bool& result)
{
  //// INLINE SYNTAX CHECK
  string inf;
  stack<char> opstack; // Operator / Operand stack
  for(int i = 0; i < infix.size(); i++)
  {
    if(infix[i] != ' ' && infix[i] != '\t')
      inf = inf + infix[i];
  }
  if(!checkSyntax(inf)) // Outsourcing Deluxe!
    return 1; // Bad Syntax

  //// INFIX TO POSTFIX
  postfix = ""; // Just in case.
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
        return 2; // Contained in neither
      else if(trueValues.contains(opn) && falseValues.contains(opn))
        return 3; // Contained in both
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
          (evT == 'T') ? opstack.push('F') : opstack.push('T'); // Emulate ops.
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
  opstack.pop(); // Final pop.
  return 0; // All g.
}

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
  ops.insert('&'); ops.insert('|'); ops.insert('!'); ops.insert('(');
  ops.insert(')');
  int sz = in.size();
  if(sz == 0)
    return false;
  int numB = 0;
  for(int i = 0; i < sz; i++)
  {
    char t = in[i];
    if(!ops.contains(t))
    {
      if(isupper(t) || isdigit(t) || !isalnum(t))
        return false;
    }
    if(isalpha(t)) // operand check, much easier.
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
        if(i + 1 < sz && (in[i + 1] == '&' || in[i + 1] == '|' ||
        in[i + 1] == ')'))
          return false;
        else if(i - 1 >= 0 && isalpha(in[i - 1]))
          return false;

      }
      else if(t == ')')
      {
        numB--;
        if(i - 1 >= 0 && (in[i - 1] == '&' || in[i - 1] == '|' ||
        in[i - 1] == '!'))
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
        else if(i - 1 >= 0 && (in[i - 1] == '&' || in[i - 1] == '|' ||
        in[i - 1] == '!'))
          return false;
        else if(in[i + 1] == '&' || in[i + 1] == '|' )
          return false;
      }
    }
  }
  return (numB == 0); // Final bracket check.
}

int main()
{
    string trueChars  = "tywz";
    string falseChars = "fnx";
    Set trues;
    Set falses;
    for (int k = 0; k < trueChars.size(); k++)
        trues.insert(trueChars[k]);
    for (int k = 0; k < falseChars.size(); k++)
        falses.insert(falseChars[k]);

    string pf;
    bool answer;
    assert(evaluate("w| f", trues, falses, pf, answer) == 0  &&  pf == "wf|" &&  answer);
    assert(evaluate("y|", trues, falses, pf, answer) == 1);
    assert(evaluate("n t", trues, falses, pf, answer) == 1);

    assert(evaluate("nt", trues, falses, pf, answer) == 1);
    cout << "test" << endl;
    assert(evaluate("()", trues, falses, pf, answer) == 1);
    cout << "ts" << endl;
    assert(evaluate("y(n|y)", trues, falses, pf, answer) == 1);
    assert(evaluate("t(&n)", trues, falses, pf, answer) == 1);
    assert(evaluate("(n&(t|7)", trues, falses, pf, answer) == 1);
    assert(evaluate("", trues, falses, pf, answer) == 1);
    assert(evaluate("f  |  !f & (t&n) ", trues, falses, pf, answer) == 0
                           &&  pf == "ff!tn&&|"  &&  !answer);
    assert(evaluate(" x  ", trues, falses, pf, answer) == 0  &&  pf == "x"  &&  !answer);
    trues.insert('x');
    assert(evaluate("((x))", trues, falses, pf, answer) == 3);
    falses.erase('x');
    assert(evaluate("((x))", trues, falses, pf, answer) == 0  &&  pf == "x"  &&  answer);
    trues.erase('w');
    assert(evaluate("w| f", trues, falses, pf, answer) == 2);
    falses.insert('w');
    assert(evaluate("w| f", trues, falses, pf, answer) == 0  &&  pf == "wf|" &&  !answer);
    cout << "Passed all tests" << endl;
}
