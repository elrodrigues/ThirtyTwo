bool pathExists(string maze[], int nRows, int nCols, int sr, int sc, int er, int ec)
{
  if(sr == er && sc == ec)
    return true;
  maze[sr][sc] = '*';
  if(sr + 1 < nRows && maze[sr + 1][sc] != '*' && maze[sr + 1][sc] != 'X' &&
  pathExists(maze, nRows, nCols, sr + 1, sc, er, ec))
    return true;
  else if(sc - 1 >= 0 && maze[sr][sc - 1] != '*' && maze[sr][sc - 1] != 'X' &&
  pathExists(maze, nRows, nCols, sr, sc - 1, er, ec))
    return true;
  else if(sr - 1 >= 0 && maze[sr - 1][sc] != '*' && maze[sr - 1][sc] != 'X' &&
  pathExists(maze, nRows, nCols, sr - 1, sc, er, ec))
    return true;
  else if(sc + 1 < nCols && maze[sr][sc + 1] != '*' && maze[sr][sc + 1] != 'X' &&
  pathExists(maze, nRows, nCols, sr, sc + 1, er, ec))
    return true;
  else
    return false;
}
