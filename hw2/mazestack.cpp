#include <iostream>
#include <stack>
using namespace std;

class Coord
{
  public:
    Coord(int rr, int cc) : m_r(rr), m_c(cc) {}
    int r() const { return m_r; }
    int c() const { return m_c; }
  private:
    int m_r;
    int m_c;
};

bool pathExists(string maze[], int nRows, int nCols, int sr, int sc, int er, int ec)
{
  Coord start(sr, sc);
  Coord end(er, ec);
  stack<Coord> codSt;
  // Never Eat Shredded Wheat - where's my sense of direction?

  codSt.push(start);
  while(!codSt.empty())
  {
    Coord cur = codSt.top();
    int r = cur.r(); int c = cur.c();
    codSt.pop();
    // cout << "(" << r << "," << c << ")" << endl;
    if(r == end.r() && c == end.c())
      return true;
    if(r + 1 < nRows && maze[r + 1][c] != '*' && maze[r+1][c] != 'X') // SOUTH
    {
      codSt.push(Coord(r + 1, c));
      maze[r + 1][c] = '*';
    }
    if(c - 1 >= 0 && maze[r][c - 1] != '*' && maze[r][c - 1] != 'X') // WEST
    {
      codSt.push(Coord(r, c - 1));
      maze[r][c - 1] = '*';
    }
    if(r - 1 >= 0 && maze[r - 1][c] != '*' && maze[r - 1][c] != 'X') // NORTH
    {
      codSt.push(Coord(r - 1, c));
      maze[r - 1][c] = '*';
    }
    if(c + 1 < nCols && maze[r][c+1] != '*' && maze[r][c+1] != 'X') // EAST
    {
      codSt.push(Coord(r, c + 1));
      maze[r][c + 1] = '*';
    }
  }
  return false;
}
//// INSERT MAIN ROUTINE BELOW
