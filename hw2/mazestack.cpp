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
  stack<Coord> codStack;
  // Never Eat Shredded Wheat - my sense of direction

  codStack.push(start);
  Coord prev(-1, -1);
  while(!codStack.empty())
  {
    Coord cur = codStack.top();
    int r = cur.r(); int c = cur.c();
    cout << "(" << r << "," << c << ")" << endl;
    if(r == end.r() && c == end.c())
      return true;
    if(r - 1 >= 0 && maze[r - 1][c] != '*' && maze[r - 1][c] != 'X') // NORTH
    {
      codStack.push(Coord(r - 1, c));
      maze[r - 1][c] = '*';
    }
    else if(c + 1 < nCols && maze[r][c+1] != '*' && maze[r][c+1] != 'X') // EAST
    {
      codStack.push(Coord(r, c + 1));
      maze[r][c + 1] = '*';
    }
    else if(r + 1 < nRows && maze[r + 1][c] != '*' && maze[r+1][c] != 'X') // SOUTH
    {
      codStack.push(Coord(r + 1, c));
      maze[r + 1][c] = '*';
    }
    else if(c - 1 >= 0 && maze[r][c - 1] != '*' && maze[r][c - 1] != 'X') // WEST
    {
      codStack.push(Coord(r, c - 1));
      maze[r][c - 1] = '*';
    }
    else if(!codStack.empty()) // PANIC!
    {
      maze[r][c] = '*';
      codStack.pop();
      if(!codStack.empty())
      {
        prev = codStack.top();
        maze[prev.r()][prev.c()] = '.';
      }
    }
  }
  return false;
}
int main()
{
    string maze[10] = {
        "XXXXXXXXXX",
        "X....X...X",
        "X.XX.XX..X",
        "XXX....X.X",
        "X.XXX.XXXX",
        "X.X...X..X",
        "X...X.X..X",
        "XXXXX.X.XX",
        "X........X",
        "XXXXXXXXXX"
    };

    if (pathExists(maze, 10,10, 3,5, 8,8))
        cout << "Solvable!" << endl;
    else
        cout << "Out of luck!" << endl;
}
