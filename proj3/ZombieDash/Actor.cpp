#include "Actor.h"
#include "StudentWorld.h" // Needed for collision and key function
#include "GameConstants.h"

// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp
///// BASE
Actor::Actor(int imageID, int col, int row, Direction stDir, int depth, StudentWorld* wld, bool mort)
: GraphObject(imageID, col, row, stDir, depth), m_mort(mort), m_wld(wld)
{}
// Actor::~Actor(){}
bool Actor::isMortal() const
{
  return m_mort;
}
StudentWorld* Actor::getWorld() const
{
  return m_wld;
}
//// FIXED
Fixed::Fixed(int imageID, int col, int row, Direction stDir, int depth, StudentWorld* wld)
: Actor(imageID, col, row, stDir, depth, wld, false)
{}
// WALL, EXIT AND PIT
Wall::Wall(int imageID, int col, int row, Direction stDir, int depth, StudentWorld* wld)
: Fixed(imageID, col, row, stDir, depth, wld)
{}
void Wall::doSomething()
{
  return;
}
// Wall::~Wall(){std::cerr << "Wall at " << getX() << "," <<getY() << " rip." << std::endl;}
//// MORTALS
Mortal::Mortal(int imageID, int col, int row, Direction stDir, int depth, StudentWorld* wld)
: Actor(imageID, col, row, stDir, depth, wld, true), m_life(true)
{}
bool Mortal::isAlive() const
{
  return m_life;
}
void Mortal::setLife(bool state)
{
  m_life = state;
}
// Mortal::~Mortal(){}

// PENELOPE
Penelope::Penelope(int imageID, int col, int row, Direction stDir, int depth, StudentWorld* wld)
: Mortal(imageID, col, row, stDir, depth, wld), m_inftick(0), m_infected(false)
{}
int Penelope::getInfRate() const
{
  return m_inftick;
}

void Penelope::doSomething()
{
  StudentWorld* world = getWorld();
  int key = (*world).fetchKey();
  switch(key){
    case KEY_PRESS_UP:
    {
      setDirection(up);
      int x = getX(); int y = getY() + 4;
      if(x < 0 || y < 0 || x >= VIEW_WIDTH || y >= VIEW_HEIGHT)
        return;
      if((*world).checkColl(x, y))
        return;
      moveTo(x, y);
      break;
    }
    case KEY_PRESS_RIGHT:
    {
      setDirection(right);
      int x = getX() + 4; int y = getY();
      if(x < 0 || y < 0 || x >= VIEW_WIDTH || y >= VIEW_HEIGHT)
        return;
      if((*world).checkColl(x, y))
        return;
      moveTo(x, y);
      break;
    }
    case KEY_PRESS_DOWN:
    {
      setDirection(down);
      int x = getX(); int y = getY() - 4;
      if(x < 0 || y < 0 || x >= VIEW_WIDTH || y >= VIEW_HEIGHT)
        return;
      if((*world).checkColl(x, y))
        return;
      moveTo(x, y);
      break;
    }
    case KEY_PRESS_LEFT:
    {
      setDirection(left);
      int x = getX() - 4; int y = getY();
      if(x < 0 || y < 0 || x >= VIEW_WIDTH || y >= VIEW_HEIGHT)
        return;
      if((*world).checkColl(x, y))
        return;
      moveTo(x, y);
      break;
    }
    default:
      break;
  }
  return;
}
