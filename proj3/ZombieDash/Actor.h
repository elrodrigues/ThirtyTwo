#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp
class Actor : public GraphObject
{
public:
  Actor(int imageID, int col, int row, Direction stDir, int depth);
  virtual void doSomething(); /// Implement! Ignored by Fixed
};
//// FIXED MEMBERS
class Wall : public Actor
{
public:
  Wall(int imageID, int col, int row, Direction stDir, int depth);
};

//// MORTAL MEMBERS
class Mortal : public Actor
{
public:
  Mortal(int imageID, int col, int row, Direction stDir, int depth);
  virtual bool isAlive();
  virtual void setLife(bool state);
private:
  bool m_life = true;
}; // Class for everything else.

class Penelope : public Mortal
{
public:
  Penelope(int imageID, int col, int row, Direction stDir, int depth);
private:
  int m_inftick; // If 500, Penelope dies
  bool m_infected;
};

#endif // ACTOR_H_
