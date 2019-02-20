#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp
class StudentWorld;
class Actor : public GraphObject
{
public:
  Actor(int imageID, int col, int row, Direction stDir, int depth, StudentWorld* wld, bool mort);
  virtual void doSomething() = 0;
  virtual bool isMortal() const;
  bool getWorld() const;
  virtual bool isAlive() const = 0;
  virtual void setLife(bool state) = 0;
  virtual ~Actor(){}
private:
  bool m_mort;
  StudentWorld* m_wld;
};
//// FIXED
class Fixed : public Actor
{
public:
  Fixed(int imageID, int col, int row, Direction stDir, int depth, StudentWorld* wld);
  virtual void doSomething() = 0;
  virtual bool isAlive() const {return false;}
  virtual void setLife(bool state){return;}
};
// WALL, PIT AND EXIT HERE
class Wall : public Fixed
{
public:
  Wall(int imageID, int col, int row, Direction stDir, int depth, StudentWorld* wld);
  virtual void doSomething();
  virtual ~Wall(){}
};

//// MORTAL MEMBERS
class Mortal : public Actor // Adds functions for "mortal" objects
{
public:
  Mortal(int imageID, int col, int row, Direction stDir, int depth, StudentWorld* wld);
  virtual void doSomething() = 0;
  virtual bool isAlive() const;
  virtual void setLife(bool state);
private:
  bool m_life; // set true
}; // Class for everything else.

class Penelope : public Mortal
{
public:
  Penelope(int imageID, int col, int row, Direction stDir, int depth, StudentWorld* wld);
  virtual void doSomething();
  virtual ~Penelope(){}
private:
  int m_inftick; // If 500, Penelope dies
  bool m_infected;
};

#endif // ACTOR_H_
