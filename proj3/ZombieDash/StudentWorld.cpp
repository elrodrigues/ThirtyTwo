#include "StudentWorld.h"
#include "GameConstants.h"
#include "Level.h"
#include <iostream>
#include <sstream>
#include <string>
using namespace std;

GameWorld* createStudentWorld(string assetPath)
{
	return new StudentWorld(assetPath);
}

// Students:  Add code to this file, StudentWorld.h, Actor.h and Actor.cpp

StudentWorld::StudentWorld(string assetPath)
: GameWorld(assetPath)
{
}
void StudentWorld::addActor(Actor* a)
{
	m_contain.push_front(a);
}
void StudentWorld::remActor(list<Actor*>::iterator p)
{
	m_contain.erase(p);
}
int StudentWorld::init()
{
	Level lev(assetPath());
	ostringstream oss;
	int lvl = getLevel();
	if(lvl < 10)
		oss << "level0" << getLevel() << ".txt";
	else
		oss << "level" << getLevel() << ".txt";
	string levelFile = oss.str();

	Level::LoadResult result = lev.loadLevel(levelFile);
	if (result == Level::load_fail_file_not_found)
		cerr<< "Cannot find level01.txt data file" << endl;
	else if (result == Level::load_fail_bad_format)
		cerr<< "Your level was improperly formatted" << endl;

	else if (result == Level::load_success)
	{
		cerr << "Level loaded successfully!" << endl;
		Level::MazeEntry ge;
		for(int i = 0; i < LEVEL_WIDTH; i++)
			for(int j = 0; j < LEVEL_HEIGHT; j++)
			{
				ge = lev.getContentsOf(i, j);
				switch(ge){
					case Level::wall:
						m_contain.push_front(new Wall(IID_WALL, SPRITE_WIDTH*i, SPRITE_HEIGHT*j, GraphObject::right, 0));
						break;
					case Level::player:
						m_contain.push_front(new Penelope(IID_PLAYER, SPRITE_WIDTH*i, SPRITE_HEIGHT*j, GraphObject::right, 0));
						break;
				}
			}
	}
	return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
    // This code is here merely to allow the game to build, run, and terminate after you hit enter.
    // Notice that the return value GWSTATUS_PLAYER_DIED will cause our framework to end the current level.
		int key = 0;
		if(getKey(key) && key == KEY_PRESS_TAB)
		{
			decLives();
			return GWSTATUS_PLAYER_DIED;
		}
		return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp()
{
	for(list<Actor*>::iterator p = m_contain.begin(); p != m_contain.end(); p++)
			delete *p;
	m_contain.erase(m_contain.begin(), m_contain.end());
}
