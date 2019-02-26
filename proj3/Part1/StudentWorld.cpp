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
: GameWorld(assetPath), m_key(-1) // Initialize m_key to -1.
{
}
StudentWorld::~StudentWorld()
{
	cleanUp(); // Just call cleanUp() for now.
}
int StudentWorld::init()
{
	Level lev(assetPath());
	ostringstream oss; // My first try at stringstream. Left it in.
	int lvl = getLevel();
	if(lvl < 10)
		oss << "level0" << getLevel() << ".txt";
	else
		oss << "level" << getLevel() << ".txt";
	string levelFile = oss.str();
	// Level loading below taken from spec.
	Level::LoadResult result = lev.loadLevel(levelFile);
	if (result == Level::load_fail_file_not_found)
		cerr<< "Cannot find " << levelFile << " data file" << endl;
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
						m_contain.push_front(new Wall(IID_WALL, SPRITE_WIDTH*i, SPRITE_HEIGHT*j, GraphObject::right, 0, this));
						break; // Note: 'this' pointer in parameters above get sent to Actor's m_wld
					case Level::player:
						m_contain.push_front(new Penelope(IID_PLAYER, SPRITE_WIDTH*i, SPRITE_HEIGHT*j, GraphObject::right, 0, this));
						m_player = m_contain.begin(); // Set iterator to point to player
						break;
				}
			}
	}
	return GWSTATUS_CONTINUE_GAME; // Continue game
}
string statScore(const int sc) // Wrote this for score display.
{
	ostringstream o;
	int c = 0;
	int n = sc;
	while(n != 0)
	{
		++c;
		n = n/10;
	}
	for(int i = 0; i < 7 - c; i++)
		o << "0";
	if(c > 0)
		o << sc;
	return o.str();
}
int StudentWorld::move()
{
    // This code is here merely to allow the game to build, run, and terminate after you hit enter.
    // Notice that the return value GWSTATUS_PLAYER_DIED will cause our framework to end the current level.
		//// MAKE ACTORS DO SOMETHING
		m_key = -1;
		getKey(m_key); // Sets m_key to current key pressed. Useful later

		for(list<Actor*>::iterator p = m_contain.begin(); p != m_contain.end(); p++)
		{
			if(!(*p)->isMortal() || (*p)->isAlive()) // THIS WILL CHANGE
				(*p)->doSomething();
		}
		if(!(*m_player)->isAlive()) // Mortal's isAlive() func used
		{
			decLives();
			return GWSTATUS_PLAYER_DIED; // Player died, end game
		}
		//// REMOVE DEAD ACTORS
		// To Be Implemented!
		//// PRINT STATS
		ostringstream oss;
		oss << "Score: " << statScore(getScore()) << "  Level:  " << getLevel() << "  Lives: "
		<< getLives() << "  Vacc:  " << 0 << "  Flames:  " << 0 << "  Mines:  " << 0
		<< "  Infected: " << (*m_player)->getInfProg();
		setGameStatText(oss.str());
		return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp()
{
	for(list<Actor*>::iterator p = m_contain.begin(); p != m_contain.end(); p++)
			delete *p; // Delete all dynamically-allocated actors
	m_contain.erase(m_contain.begin(), m_contain.end()); // Remove hanging ptrs
}

int StudentWorld::fetchKey() const
{
	return m_key;
}

bool StudentWorld::checkColl(const int& col, const int& row)
{
	for(list<Actor*>::iterator p = m_contain.begin(); p != m_contain.end(); p++)
	{ // Checks for each actor in container
		double dx = (*p)->getX() - col; double dy = (*p)->getY() - row;
		dx = (dx >= 0 ? dx/16 : (-dx)/16); // Absolute Value
		dy = (dy >= 0 ? dy/16 : (-dy)/16);
		double diff = (dx > dy ? dx : dy); // Square Metric to determine collision
		if(p != m_player && diff < 1)
		{
			// cerr << "Pos: " << (*p)->getX() << "," << (*p)->getY() << endl;
			// cerr << "Dest: " << col << "," << row << endl;
			// cerr << "diff: " << diff << endl;
			return true; // BOXES INTERSECT
		}
	}
	return false; // ALL BOXES DONT INTERSECT
}
