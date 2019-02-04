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
  Coord prev(-1, -1);
  while(!codQ.empty())
  {
    Coord cur = codQ.front();
    int r = cur.r(); int c = cur.c();
    // cout << "(" << r << "," << c << ")" << endl;
    if(r == end.r() && c == end.c())
      return true;

    if(r + 1 < nRows && maze[r + 1][c] != '*' && maze[r+1][c] != 'X') // SOUTH
    {
      int sz = codQ.size();
      int* rM = new int[sz];
      int* cM = new int[sz];
      for(int i = 0; i < sz; i++)
      {
        rM[i] = codQ.front().r();
        cM[i] = codQ.front().c();
        codQ.pop();
      }
      codQ.push(Coord(r + 1, c));
      for(int i = 0; i < sz; i++)
        codQ.push(Coord(rM[i], cM[i]));
      maze[r + 1][c] = '*';
      delete [] rM;
      delete [] cM;
    }
    else if(c - 1 >= 0 && maze[r][c - 1] != '*' && maze[r][c - 1] != 'X') // WEST
    {
      int sz = codQ.size();
      int* rM = new int[sz];
      int* cM = new int[sz];
      for(int i = 0; i < sz; i++)
      {
        rM[i] = codQ.front().r();
        cM[i] = codQ.front().c();
        codQ.pop();
      }
      codQ.push(Coord(r, c - 1));
      for(int i = 0; i < sz; i++)
        codQ.push(Coord(rM[i], cM[i]));
      maze[r][c - 1] = '*';
      delete [] rM;
      delete [] cM;
    }
    else if(r - 1 >= 0 && maze[r - 1][c] != '*' && maze[r - 1][c] != 'X') // NORTH
    {
      int sz = codQ.size();
      int* rM = new int[sz];
      int* cM = new int[sz];
      for(int i = 0; i < sz; i++)
      {
        rM[i] = codQ.front().r();
        cM[i] = codQ.front().c();
        codQ.pop();
      }
      codQ.push(Coord(r - 1, c));
      for(int i = 0; i < sz; i++)
        codQ.push(Coord(rM[i], cM[i]));
      maze[r - 1][c] = '*';
      delete [] rM;
      delete [] cM;
    }
    else if(c + 1 < nCols && maze[r][c+1] != '*' && maze[r][c+1] != 'X') // EAST
    {
      int sz = codQ.size();
      int* rM = new int[sz];
      int* cM = new int[sz];
      for(int i = 0; i < sz; i++)
      {
        rM[i] = codQ.front().r();
        cM[i] = codQ.front().c();
        codQ.pop();
      }
      codQ.push(Coord(r, c + 1));
      for(int i = 0; i < sz; i++)
        codQ.push(Coord(rM[i], cM[i]));
      maze[r][c + 1] = '*';
      delete [] rM;
      delete [] cM;
    }
    else if(!codQ.empty()) // PANIC!
    {
      maze[r][c] = '*';
      codQ.pop();
      if(!codQ.empty())
      {
        prev = codQ.front();
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
