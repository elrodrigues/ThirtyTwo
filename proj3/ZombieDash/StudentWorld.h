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
    virtual ~StudentWorld(); // Just calls cleanUp() for now
    virtual int fetchKey() const; // Grabs private pressed key for Penelope
    virtual bool checkColl(const int& col, const int& row); // Checks collision between actors

private:
    std::list<Actor*> m_contain; // The Container for actors
    std::list<Actor*>::iterator m_player; // Iterator that points to player in Container
    int m_key; // Temporary storage of pressed key. Pretty handy.
};

#endif // STUDENTWORLD_H_
