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
  world()->activateOnAppropriateActors(this);
}
void Pit::activateIfAppropriate(Actor* a)
{
  a->dieByFallOrBurnIfAppropriate();
}
bool Pit::isPit() const
{
  return true;
}
//// FLAME
Flame::Flame(StudentWorld* w, double x, double y, int dir)
: ActivatingObject(w, IID_FLAME, x, y, dir, 0), m_flametick(0)
{}
void Flame::doSomething()
{
	if(isDead())
    return;
  world()->activateOnAppropriateActors(this);
  m_flametick++;
  if (m_flametick == 2) // 2
    setDead();

}
void Flame::activateIfAppropriate(Actor* a)
{
  if(!a->blocksFlame())
    a->dieByFallOrBurnIfAppropriate();
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
: ActivatingObject(w, IID_LANDMINE, x, y, right, 1), m_safetick(30)
{}
void Landmine::doSomething()
{
  if(m_safetick >= 0)
  {
    m_safetick--;
    return;
  }
  world()->activateOnAppropriateActors(this);
}
void Landmine::activateIfAppropriate(Actor* a)
{
  if(!isDead() && !a->blocksFlame()) //// WILL BE CHANGED`
  {
    setDead();
    world()->playSound(SOUND_LANDMINE_EXPLODE);
    double x = getX();
	  double y = getY();
    world()->addActor(new Flame(world(), x, y, up));
    for(int i = -1; i < 2; i++)
      for(int j = -1; j < 2; j++)
        if((i != 0 || j != 0) && !world()->isFlameBlockedAt(x + SPRITE_WIDTH*j, y + SPRITE_HEIGHT*i))
            world()->addActor(new Flame(world(), x + SPRITE_WIDTH*j , y + SPRITE_HEIGHT*i, up));
    world()->addActor(new Pit(world(), x, y));
  }
}
void Landmine::dieByFallOrBurnIfAppropriate()
{
  if(!isDead())
    activateIfAppropriate(this);
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
  setDead();
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
: Agent(w, imageID, x, y, right), m_inf(0), m_istat(false)
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
  m_istat = false;
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
  if(getInfectionDuration() == 500)
  {
    setDead();
    world()->playSound(SOUND_PLAYER_DIE);
  }
  if(isDead())
    return;
  if(getInfectionDuration() > 0)
    increaseInfection();
  StudentWorld* wld = world();
  int key = (*wld).fetchKey(); // StudentWorld Func to fetch (not get) pressed key.
  switch(key){ // Probably a better way to handle this
    case KEY_PRESS_UP:
    {
      setDirection(up); // Set direction regardless of collision
      double x = getX(); double y = getY() + 4;
      if(x < 0 || y < 0 || x >= VIEW_WIDTH || y >= VIEW_HEIGHT) // Just in case
        return;
      if(world()->isAgentMovementBlockedAt(x, y, this)) // This function checks for collision, in StudentWorld
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
      if(world()->isAgentMovementBlockedAt(x, y, this))
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
      if(world()->isAgentMovementBlockedAt(x, y, this))
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
      if(world()->isAgentMovementBlockedAt(x, y, this))
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
	  break;
    }
    case KEY_PRESS_SPACE:
    {
      if(getNumFlameCharges() > 0)
      {
        m_flame--;
        int dir = getDirection();
        double x = getX();
        double y = getY();
        world()->playSound(SOUND_PLAYER_FIRE);
        int i = 1;
        switch(dir){
          case up:
            for(; i < 4; i++)
              if(!world()->isFlameBlockedAt(x, y + SPRITE_HEIGHT*i))
                world()->addActor(new Flame(world(), x, y + SPRITE_HEIGHT*i, up));
            break;
          case down:
            for(; i < 4; i++)
              if(!world()->isFlameBlockedAt(x, y - SPRITE_HEIGHT*i))
                world()->addActor(new Flame(world(), x, y - SPRITE_HEIGHT*i, down));
            break;
          case left:
          for(; i < 4; i++)
            if(!world()->isFlameBlockedAt(x - SPRITE_WIDTH*i, y))
              world()->addActor(new Flame(world(), x - SPRITE_WIDTH*i, y, right));
            break;
          case right:
            for(; i < 4; i++)
              if(!world()->isFlameBlockedAt(x + SPRITE_WIDTH*i, y))
                world()->addActor(new Flame(world(), x + SPRITE_WIDTH*i, y, right));
            break;
          default:
            break;
        }
      }
      break;
    }
    case KEY_PRESS_ENTER:
    {
      if(getNumVaccines() > 0)
      {
        --m_vacc;
        clearInfection();
      }
      break;
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
  world()->playSound(SOUND_PLAYER_DIE);
  setDead();
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
: Human(w, IID_CITIZEN, x, y), m_paral(false)
{}
void Citizen::doSomething()
{
  int BLOCKDIST = 2;
  if(isDead())
    return;
  if(isInfected())
  {
    if(getInfectionDuration() == 500)
    {
      setDead();
      world()->playSound(SOUND_ZOMBIE_BORN);
      world()->recordCitizenGone(0);
      randInt(1, 10) > 7 ? world()->addActor(new SmartZombie(world(), getX(), getY())) :
      world()->addActor(new DumbZombie(world(), getX(), getY()));
      return;
    }
    if(getInfectionDuration() > 0)
      increaseInfection();
  }
  if(m_paral)
    m_paral = false;
  else
  {
    m_paral = true;
    double closeX, closeY, dist = -1;
    double x = getX(); double y = getY();
    bool isZombie;
    bool found = world()->locateNearestCitizenTrigger(x, y, closeX, closeY, dist, isZombie);
    if(!found || dist > 80)
      return;
    if(isZombie)
    {
      found = world()->locateNearestCitizenThreat(x, y, closeX, closeY, dist);
      if(!found)
        return;
      if(dist <= 80)
      {
        int escape = -1;
        double bestDist = dist;
        if(!world()->isAgentMovementBlockedAt(x + 2, y, this))
        {
          world()->locateNearestCitizenThreat(x + 2, y, closeX, closeY, dist);
          if(bestDist > dist)
          {
            bestDist = dist;
            escape = right;
          }
        }
        if(!world()->isAgentMovementBlockedAt(x - 2, y, this))
        {
          world()->locateNearestCitizenThreat(x - 2, y, closeX, closeY, dist);
          if(bestDist > dist)
          {
            bestDist = dist;
            escape = left;
          }
        }
        if(!world()->isAgentMovementBlockedAt(x, y + 2, this))
        {
          world()->locateNearestCitizenThreat(x, y + 2, closeX, closeY, dist);
          if(bestDist > dist)
          {
            bestDist = dist;
            escape = up;
          }
        }
        if(!world()->isAgentMovementBlockedAt(x, y - 2, this))
        {
          world()->locateNearestCitizenThreat(x, y - 2, closeX, closeY, dist);
          if(bestDist > dist)
          {
            bestDist = dist;
            escape = down;
          }
        }

        switch(escape)
        {
          case left:
          {
            setDirection(left);
            moveTo(x - 2, y); break;
          }
          case right:
          {
            setDirection(right);
            moveTo(x + 2, y); break;
          }
          case up:
          {
            setDirection(up);
            moveTo(x, y + 2); break;
          }
          case down:
          {
            setDirection(down);
            moveTo(x, y - 2); break;
          }
          default:
            break;
        }
        return;
      }
    }
    else
    {
      if(y == closeY) // Same row
      {
        if(x > closeX)
        {
          setDirection(left);
          if(!world()->isAgentMovementBlockedAt(x - BLOCKDIST, y, this))
            moveTo(x - 2, y);
        }
        else if(x < closeX)
        {
          setDirection(right);
          if(!world()->isAgentMovementBlockedAt(x + BLOCKDIST, y, this))
            moveTo(x + 2, y);
        }
        return;
      }
      else if(x == closeX) // Same column
      {
        if(y > closeY)
        {
          setDirection(down);
          if(!world()->isAgentMovementBlockedAt(x, y - BLOCKDIST, this))
            moveTo(x, y - 2);
        }
        else if(y < closeY)
        {
          setDirection(up);
          if(!world()->isAgentMovementBlockedAt(x, y + BLOCKDIST, this))
            moveTo(x, y + 2);
        }
        return;
      }
      else // Random
      {
        switch(randInt(0, 1))
        {
          case 0:
          {
            if(x > closeX)
            {
              setDirection(left);
              if(!world()->isAgentMovementBlockedAt(x - BLOCKDIST, y, this))
                moveTo(x - 2, y);
            }
            else if(x < closeX)
            {
              setDirection(right);
              if(!world()->isAgentMovementBlockedAt(x + BLOCKDIST, y, this))
                moveTo(x + 2, y);
            }
            return;
          }
          case 1:
          {
            if(y > closeY)
            {
              setDirection(down);
              if(!world()->isAgentMovementBlockedAt(x, y - BLOCKDIST, this))
                moveTo(x, y - 2);
            }
            else if(y < closeY)
            {
              setDirection(up);
              if(!world()->isAgentMovementBlockedAt(x, y + BLOCKDIST, this))
                moveTo(x, y + 2);
            }
            return;
          }
        }
      }
    }
  }

}
void Citizen::dieByFallOrBurnIfAppropriate()
{
  setDead();
  world()->playSound(SOUND_CITIZEN_DIE);
  world()->recordCitizenGone(0);
}
void Citizen::useExitIfAppropriate()
{
  setDead();
  world()->recordCitizenGone(1);
  world()->playSound(SOUND_CITIZEN_SAVED);
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
