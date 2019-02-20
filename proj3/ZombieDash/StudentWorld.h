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
    virtual ~StudentWorld();
    virtual int fetchKey() const;
    virtual bool checkColl(const int& col, const int& row);

private:
    std::list<Actor*> m_contain;
    std::list<Actor*>::iterator m_player;
    int m_key;
};

#endif // STUDENTWORLD_H_
