#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp
class StudentWorld; // Prototype for pointer declaration below
//// DRAFT CLASS STRUCTURE: () Actor -> (1) Fixed, (2) Mortal -> (1a) Wall, (2a) Penelope
class Actor : public GraphObject
{
public:
  Actor(int imageID, int col, int row, Direction stDir, int depth, StudentWorld* wld, bool mort);
  virtual void doSomething() = 0;
  virtual bool isMortal() const; // Identifies Fixed and Mortal objects
  StudentWorld* getWorld() const; // Access to StudentWorld
  virtual ~Actor(){} // Destructor does nothing
  //// Additionals below
  virtual bool isAlive() const = 0; // Needed to call functions in Mortal class
  virtual void setLife(bool state) = 0;
  virtual int getInfProg() const {return -1;} // Needed to use func in Penelope class
private:
  bool m_mort; // Is object Fixed or Mortal
  StudentWorld* m_wld; // Pointer to current StudentWorld
};
//// FIXED
class Fixed : public Actor // Collection of mort = false
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
class Mortal : public Actor // Collection of mort = true
{
public:
  Mortal(int imageID, int col, int row, Direction stDir, int depth, StudentWorld* wld);
  virtual void doSomething() = 0;
  virtual bool isAlive() const; // Checks if object "alive" (includes goodies, flames etc)
  virtual void setLife(bool state); // Change m_life, used by StudentWorld
private:
  bool m_life; // set true on construction
}; // Class for everything else.

class Penelope : public Mortal
{
public:
  Penelope(int imageID, int col, int row, Direction stDir, int depth, StudentWorld* wld);
  virtual void doSomething();
  virtual int getInfProg() const; // Get infection progress, used by World
  virtual ~Penelope(){} // Destructor does nothing for now
private:
  int m_inftick; // If 500, Penelope dies
  bool m_infected; // Is infected or not. Not needed but nice to have.
};

#endif // ACTOR_H_
