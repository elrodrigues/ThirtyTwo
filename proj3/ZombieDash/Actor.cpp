#include "Actor.h"
#include "StudentWorld.h"
#include "GameConstants.h"
#include <cmath>

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
: Actor(w, imageID, x, y, dir, depth), m_tick(0)
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
void ActivatingObject::doSomethingAndTick(ActivatingObject* a)
{
  a->world()->activateOnAppropriateActors(a);
  a->m_tick++;
  if(a->m_tick == 2)
    a->setDead();
}
//// FLAME
Flame::Flame(StudentWorld* w, double x, double y, int dir)
: ActivatingObject(w, IID_FLAME, x, y, dir, 0)
{}
void Flame::doSomething()
{
	if(isDead())
    return;
  doSomethingAndTick(this);

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
  if(isDead())
    return;
  doSomethingAndTick(this);
}
void Vomit::activateIfAppropriate(Actor* a)
{
  if(a->triggersZombieVomit())
  {
    a->setInfected();
  }
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
: Actor(w, imageID, x, y, dir, 0), m_paral(false)
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
void Human::setInfected()
{
  m_istat = true;
}
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
  if(isDead())
    return;
  if(isInfected())
  {
    if(getInfectionDuration() == 500)
    {
      setDead();
      world()->playSound(SOUND_PLAYER_DIE);
      return;
    }
    increaseInfection();
  }
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
: Human(w, IID_CITIZEN, x, y)
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
    increaseInfection();
  }
  if(getParal())
    setParal(false);
  else
  {
    setParal(true);
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
          if(bestDist < dist)
          {
            bestDist = dist;
            escape = right;
          }
        }
        if(!world()->isAgentMovementBlockedAt(x - 2, y, this))
        {
          world()->locateNearestCitizenThreat(x - 2, y, closeX, closeY, dist);
          if(bestDist < dist)
          {
            bestDist = dist;
            escape = left;
          }
        }
        if(!world()->isAgentMovementBlockedAt(x, y + 2, this))
        {
          world()->locateNearestCitizenThreat(x, y + 2, closeX, closeY, dist);
          if(bestDist < dist)
          {
            bestDist = dist;
            escape = up;
          }
        }
        if(!world()->isAgentMovementBlockedAt(x, y - 2, this))
        {
          world()->locateNearestCitizenThreat(x, y - 2, closeX, closeY, dist);
          if(bestDist < dist)
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
: Agent(w, IID_ZOMBIE, x, y, right), m_step(0)
{}
bool Zombie::triggersCitizens() const
{
  return true;
}
bool Zombie::threatensCitizens() const
{
  return true;
}
//// DumbZombie
DumbZombie::DumbZombie(StudentWorld* w, double x, double y)
: Zombie(w, x, y)
{}
void DumbZombie::doSomething()
{
  if(isDead())
    return;
  if(getParal())
  {
    setParal(false);
    return;
  }
  else
  {
    setParal(true);
    int dir = getDirection();
    double vx, vy = -1;
    switch(dir)
    {
      case left:
        vx = getX() - SPRITE_WIDTH; vy = getY();
        break;
      case right:
        vx = getX() + SPRITE_WIDTH; vy = getY();
        break;
      case up:
        vx = getX(); vy = getY() + SPRITE_HEIGHT;
        break;
      case down:
        vx = getX(); vy = getY() - SPRITE_HEIGHT;
        break;
      default:
        break;
    }
    if(world()->isZombieVomitTriggerAt(vx, vy) && randInt(0, 2) == 2)
    {
      world()->addActor(new Vomit(world(), vx, vy, getDirection()));
      world()->playSound(SOUND_ZOMBIE_VOMIT);
      return;
    }
    if(getStep() == 0)
    {
      setStep(randInt(3, 10));
      switch(randInt(1, 4))
      {
        case 1:
          setDirection(left);
          break;
        case 2:
          setDirection(right);
          break;
        case 3:
          setDirection(up);
          break;
        case 4:
          setDirection(down);
          break;
      }
    }
    double destx, desty = -1;
    switch(getDirection())
    {
      case left:
        destx = getX() - 1; desty = getY();
        break;
      case right:
        destx = getX() + 1; desty = getY();
        break;
      case up:
        destx = getX(); desty = getY() + 1;
        break;
      case down:
        destx = getX(); desty = getY() - 1;
        break;
    }
    if(!world()->isAgentMovementBlockedAt(destx, desty, this))
    {
      moveTo(destx, desty);
      decStep();
    }
    else
    {
      resetStep();
    }

  }
}
void DumbZombie::dieByFallOrBurnIfAppropriate()
{
  setDead();
  world()->increaseScore(1000);
  world()->playSound(SOUND_ZOMBIE_DIE);
}
//// SmartZombie
SmartZombie::SmartZombie(StudentWorld* w, double x, double y)
: Zombie(w, x, y)
{}
void SmartZombie::doSomething()
{
  if(isDead())
    return;
  if(getParal())
  {
    setParal(false);
    return;
  }
  else
  {
    setParal(true);
    int dir = getDirection();
    double vx, vy = -1;
    switch(dir)
    {
      case left:
        vx = getX() - SPRITE_WIDTH; vy = getY();
        break;
      case right:
        vx = getX() + SPRITE_WIDTH; vy = getY();
        break;
      case up:
        vx = getX(); vy = getY() + SPRITE_HEIGHT;
        break;
      case down:
        vx = getX(); vy = getY() - SPRITE_HEIGHT;
        break;
      default:
        break;
    }
    if(world()->isZombieVomitTriggerAt(vx, vy) && randInt(0, 2) == 2)
    {
      world()->addActor(new Vomit(world(), vx, vy, getDirection()));
      world()->playSound(SOUND_ZOMBIE_VOMIT);
      return;
    }
    if(getStep() == 0)
    {
      setStep(randInt(3, 10));
      double ox, oy = -1;
      double dist = -1;
      double x = getX();
      double y = getY();
      world()->locateNearestVomitTrigger(x, y, ox, oy, dist);
      if(dist <= 80)
      {
        if(ox == x)
        {
          if(y < oy)
            setDirection(up);
          else
            setDirection(down);
        }
        else if(oy == y)
        {
          if(x < ox)
            setDirection(right);
          else
            setDirection(left);
        }
        else
        {
          switch(randInt(0, 1))
          {
            case 0:
            {
              if(x > ox)
              {
                setDirection(left);
              }
              else if(x < ox)
              {
                setDirection(right);
              }
              break;
            }
            case 1:
            {
              if(y > oy)
              {
                setDirection(down);
              }
              else if(y < oy)
              {
                setDirection(up);
              }
              break;
            }
        }
      }
      switch(randInt(1, 4))
      {
        case 1:
          setDirection(left);
          break;
        case 2:
          setDirection(right);
          break;
        case 3:
          setDirection(up);
          break;
        case 4:
          setDirection(down);
          break;
      }
    }
    double destx, desty = -1;
    switch(getDirection())
    {
      case left:
        destx = getX() - 1; desty = getY();
        break;
      case right:
        destx = getX() + 1; desty = getY();
        break;
      case up:
        destx = getX(); desty = getY() + 1;
        break;
      case down:
        destx = getX(); desty = getY() - 1;
        break;
    }
    if(!world()->isAgentMovementBlockedAt(destx, desty, this))
    {
      moveTo(destx, desty);
      decStep();
    }
    else
    {
      resetStep();
    }
}
  }
}
void SmartZombie::dieByFallOrBurnIfAppropriate()
{
  setDead();
  world()->increaseScore(2000);
  world()->playSound(SOUND_ZOMBIE_DIE);
  
}
