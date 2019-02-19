#include "Actor.h"
#include "StudentWorld.h"

// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp
///// BASE
Actor::Actor(int imageID, int col, int row, Direction stDir, int depth)
: GraphObject(imageID, col, row, stDir, depth)
{}
void Actor::doSomething()
{
  return;
}
//// FIXED
// WALL
Wall::Wall(int imageID, int col, int row, Direction stDir, int depth)
: Actor(imageID, col, row, stDir, depth)
{}

//// MORTALS
Mortal::Mortal(int imageID, int col, int row, Direction stDir, int depth)
: Actor(imageID, col, row, stDir, depth), m_life(true)
{}

bool Mortal::isAlive()
{
  return m_life;
}

void Mortal::setLife(bool state)
{
  m_life = state;
}

// PENELOPE
Penelope::Penelope(int imageID, int col, int row, Direction stDir, int depth)
: Mortal(imageID, col, row, stDir, depth), m_inftick(0), m_infected(false)
{}
