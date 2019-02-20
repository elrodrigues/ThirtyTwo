#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "Actor.h"
#include <string>
#include <list>

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class StudentWorld : public GameWorld
{
public:
    StudentWorld(std::string assetPath);
    virtual int init();
    virtual int move();
    virtual void cleanUp();
    virtual void addActor(Actor* a);
    virtual void remActor(std::list<Actor*>::iterator p);
    ~StudentWorld(){}

private:
    std::list<Actor*> m_contain;
    std::list<Actor*>::iterator m_player;
};

#endif // STUDENTWORLD_H_
