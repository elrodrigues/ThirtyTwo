#include "Actor.h"
#include "StudentWorld.h"
#include "GameConstants.h"

// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp
// Description of functions in Actor.h. Added some description for func behavior
///// BASE
Actor::Actor(StudentWorld* w, int imageID, double x, double y, int dir, int depth)
: GraphObject(imageID, x, y, dir, depth), m_w(w), m_life(true)
{}
// Actor::~Actor(){}
bool Actor::isDead() const
{
  return !m_life;
}
void Actor::setDead()
{
  m_life = false;
}
StudentWorld* Actor::world() const
{
  return m_w; // Needed to access Actor's private pointer.
}
void Actor::activateIfAppropriate(Actor* a)
{
  return;
}
void Actor::useExitIfAppropriate()
{
  return;
}
void Actor::beVomitedOnIfAppropriate()
{
  return;
}
void Actor::dieByFallOrBurnIfAppropriate()
{
  return;
}
void Actor::pickUpGoodieIfAppropriate(Goodie* g)
{
  return;
}
bool Actor::blocksMovement() const
{
  return false;
}
bool Actor::blocksFlame() const
{
  return false;
}
bool Actor::triggersOnlyActiveLandmines() const
{
  return false;
}
bool Actor::triggersZombieVomit() const
{
  return false;
}
bool Actor::threatensCitizens() const
{
  return false;
}
bool Actor::triggersCitizens() const
{
  return false;
}
Penelope* Actor::ptr()
{
  return nullptr;
}
bool Actor::isPit() const
{
  return false;
}
////  WALL
Wall::Wall(StudentWorld* w, double x, double y)
: Actor(w, IID_WALL, x, y, right, 0)
{
  setDead();
}
void Wall::doSomething()
{
  return;
}
bool Wall::blocksMovement() const
{
  return true;
}
bool Wall::blocksFlame() const
{
  return true;
}
//// ACTIVATINGOBJECT
ActivatingObject::ActivatingObject(StudentWorld* w, int imageID, double x, double y, int dir, int depth)
: Actor(w, imageID, x, y, dir, depth)
{}
//// EXIT
Exit::Exit(StudentWorld* w, double x, double y)
: ActivatingObject(w, IID_EXIT, x, y, right, 1)
{
  setDead();
}
void Exit::doSomething()
{
  world()->activateOnAppropriateActors(this);
}
void Exit::activateIfAppropriate(Actor* a)
{
  if(a->triggersZombieVomit())
  {
      a->useExitIfAppropriate();
  }
}
bool Exit::blocksFlame() const
{
  return true;
}
//// PIT
Pit::Pit(StudentWorld* w, double x, double y)
: ActivatingObject(w, IID_PIT, x, y, right, 0)
{
  setDead();
}
void Pit::doSomething()
{
  return;
}
void Pit::activateIfAppropriate(Actor* a)
{
  return;
}
bool Pit::isPit() const
{
  return true;
}
//// FLAME
Flame::Flame(StudentWorld* w, double x, double y, int dir)
: ActivatingObject(w, IID_FLAME, x, y, dir, 0)
{}
void Flame::doSomething()
{
  return;
}
void Flame::activateIfAppropriate(Actor* a)
{
  return;
}
//// VOMIT
Vomit::Vomit(StudentWorld* w, double x, double y, int dir)
: ActivatingObject(w, IID_VOMIT, x, y, dir, 0)
{}
void Vomit::doSomething()
{
  return;
}
void Vomit::activateIfAppropriate(Actor* a)
{
  return;
}
//// LANDMINES
Landmine::Landmine(StudentWorld* w, double x, double y)
: ActivatingObject(w, IID_LANDMINE, x, y, right, 1), m_safetick(0)
{}
void Landmine::doSomething()
{
  if(m_safetick <= 30)
  {
    m_safetick++;
    return;
  }
  world()->activateOnAppropriateActors(this);
}
void Landmine::activateIfAppropriate(Actor* a)
{
  if(a->ptr() != nullptr) //// WILL BE CHANGED`
  {
    std::cerr << "Boom" << std::endl;
    setDead();
  }
}
void Landmine::dieByFallOrBurnIfAppropriate()
{
  return;
}

//// GOODIE
Goodie::Goodie(StudentWorld* w, int imageID, double x, double y)
: ActivatingObject(w, imageID, x, y, right, 1)
{}
void Goodie::activateIfAppropriate(Actor* a)
{
  if(a->ptr() != nullptr)
    a->pickUpGoodieIfAppropriate(this);
}
void Goodie::dieByFallOrBurnIfAppropriate()
{
  return;
}
//// VaccineGoodie
VaccineGoodie::VaccineGoodie(StudentWorld* w, double x, double y)
: Goodie(w, IID_VACCINE_GOODIE, x, y)
{}
void VaccineGoodie::doSomething()
{
  if(!isDead())
    world()->activateOnAppropriateActors(this);
}
void VaccineGoodie::pickUp(Penelope* p)
{
  p->increaseVaccines();
  setDead();
}
//// GasCanGoodie
GasCanGoodie::GasCanGoodie(StudentWorld* w, double x, double y)
: Goodie(w, IID_GAS_CAN_GOODIE, x, y)
{}
void GasCanGoodie::doSomething()
{
  if(!isDead())
    world()->activateOnAppropriateActors(this);
}
void GasCanGoodie::pickUp(Penelope* p)
{
  p->increaseFlameCharges();
  setDead();
}
//// LandmineGoodie
LandmineGoodie::LandmineGoodie(StudentWorld* w, double x, double y)
: Goodie(w, IID_LANDMINE_GOODIE, x, y)
{}
void LandmineGoodie::doSomething()
{
  if(!isDead())
    world()->activateOnAppropriateActors(this);
}
void LandmineGoodie::pickUp(Penelope* p)
{
  p->increaseLandmines();
  setDead();
}
//// Agent
Agent::Agent(StudentWorld* w, int imageID, double x, double y, int dir)
: Actor(w, imageID, x, y, dir, 0)
{}
bool Agent::blocksMovement() const
{
  return true;
}
bool Agent::triggersOnlyActiveLandmines() const
{
  return true;
}
//// Human
Human::Human(StudentWorld* w, int imageID, double x, double y)
: Agent(w, imageID, x, y, right), m_inf(0)
{}
void Human::beVomitedOnIfAppropriate()
{
  return;
}
bool Human::triggersZombieVomit() const
{
  return true;
}
void Human::clearInfection()
{
  m_inf = 0;
}
int Human::getInfectionDuration() const
{
  return m_inf;
}
// PENELOPE
Penelope::Penelope(StudentWorld* w, double x, double y)
: Human(w, IID_PLAYER, x, y), m_vacc(0), m_land(0), m_flame(0)
{}
void Penelope::doSomething()
{
  if(isDead())
    return;
  StudentWorld* wld = world();
  int key = (*wld).fetchKey(); // StudentWorld Func to fetch (not get) pressed key.
  switch(key){ // Probably a better way to handle this
    case KEY_PRESS_UP:
    {
      setDirection(up); // Set direction regardless of collision
      double x = getX(); double y = getY() + 4;
      if(x < 0 || y < 0 || x >= VIEW_WIDTH || y >= VIEW_HEIGHT) // Just in case
        return;
      if((*wld).isAgentMovementBlockedAt(x, y)) // This function checks for collision, in StudentWorld
        return;
      moveTo(x, y);
      break;
    } // Repeat this times 3
    case KEY_PRESS_RIGHT:
    {
      setDirection(right);
      double x = getX() + 4; double y = getY();
      if(x < 0 || y < 0 || x >= VIEW_WIDTH || y >= VIEW_HEIGHT)
        return;
      if((*wld).isAgentMovementBlockedAt(x, y))
        return;
      moveTo(x, y);
      break;
    }
    case KEY_PRESS_DOWN:
    {
      setDirection(down);
      double x = getX(); double y = getY() - 4;
      if(x < 0 || y < 0 || x >= VIEW_WIDTH || y >= VIEW_HEIGHT)
        return;
      if((*wld).isAgentMovementBlockedAt(x, y))
        return;
      moveTo(x, y);
      break;
    }
    case KEY_PRESS_LEFT:
    {
      setDirection(left);
      double x = getX() - 4; double y = getY();
      if(x < 0 || y < 0 || x >= VIEW_WIDTH || y >= VIEW_HEIGHT)
        return;
      if((*wld).isAgentMovementBlockedAt(x, y))
        return;
      moveTo(x, y);
      break;
    }
    case KEY_PRESS_TAB:
    {
      if(getNumLandmines() > 0)
      {
          world()->addActor(new Landmine(world(), getX(), getY()));
          m_land--;
      }
    }
    default:
      break;
  }
  return;
}
void Penelope::useExitIfAppropriate() // Common Function
{
  world()->recordLevelFinishedIfAllCitizensGone();
  world()->playSound(SOUND_LEVEL_FINISHED);
}
void Penelope::dieByFallOrBurnIfAppropriate()
{
  return;
}
void Penelope::pickUpGoodieIfAppropriate(Goodie* g)
{
  world()->playSound(SOUND_GOT_GOODIE);
  world()->increaseScore(50);
  g->pickUp(this);
}
void Penelope::increaseVaccines()
{
  ++m_vacc;
}
void Penelope::increaseLandmines()
{
 m_land+=2;
}
void Penelope::increaseFlameCharges()
{
  m_flame+=5;
}
int Penelope::getNumVaccines() const
{
  return m_vacc;
}
int Penelope::getNumLandmines() const
{
  return m_land;
}
int Penelope::getNumFlameCharges() const
{
  return m_flame;
}
Penelope* Penelope::ptr()
{
  return this;
}
//// Citizen
Citizen::Citizen(StudentWorld* w, double x, double y)
: Human(w, IID_CITIZEN, x, y)
{}
void Citizen::doSomething()
{
  return;
}
void Citizen::dieByFallOrBurnIfAppropriate()
{
  return;
}
void Citizen::useExitIfAppropriate()
{
  return;
}
//// Zombie
Zombie::Zombie(StudentWorld* w, double x, double y)
: Agent(w, IID_ZOMBIE, x, y, right)
{}
//// DumbZombie
DumbZombie::DumbZombie(StudentWorld* w, double x, double y)
: Zombie(w, x, y)
{}
void DumbZombie::doSomething()
{
  return;
}
void DumbZombie::dieByFallOrBurnIfAppropriate()
{
  return;
}
//// SmartZombie
SmartZombie::SmartZombie(StudentWorld* w, double x, double y)
: Zombie(w, x, y)
{}
void SmartZombie::doSomething()
{
  return;
}
void SmartZombie::dieByFallOrBurnIfAppropriate()
{
  return;
}
