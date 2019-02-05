#include <iostream>
#include <queue>
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
  queue<Coord> codQ;
  // Never Eat Shredded Wheat - my sense of direction

  codQ.push(start);
  while(!codQ.empty())
  {
    Coord cur = codQ.front();
    int r = cur.r(); int c = cur.c();
    codQ.pop();
    // cout << "(" << r << "," << c << ")" << endl;
    if(r == end.r() && c == end.c())
      return true;
    if(r + 1 < nRows && maze[r + 1][c] != '*' && maze[r+1][c] != 'X') // SOUTH
    {
      codQ.push(Coord(r + 1, c));
      maze[r + 1][c] = '*';
    }
    if(c - 1 >= 0 && maze[r][c - 1] != '*' && maze[r][c - 1] != 'X') // WEST
    {
      codQ.push(Coord(r, c - 1));
      maze[r][c - 1] = '*';
    }
    if(r - 1 >= 0 && maze[r - 1][c] != '*' && maze[r - 1][c] != 'X') // NORTH
    {
      codQ.push(Coord(r - 1, c));
      maze[r - 1][c] = '*';
    }
    if(c + 1 < nCols && maze[r][c+1] != '*' && maze[r][c+1] != 'X') // EAST
    {
      codQ.push(Coord(r, c + 1));
      maze[r][c + 1] = '*';
    }
  }
  return false;
}
//// INSERT MAIN ROUTINE BELOW
