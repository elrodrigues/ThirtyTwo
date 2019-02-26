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

StudentWorld::StudentWorld(string assetDir)
: GameWorld(assetDir), m_key(-1), m_fin(false) // Initialize m_key to -1.
{
}
StudentWorld::~StudentWorld()
{
	cleanUp(); // Just call cleanUp() for now.
}
void StudentWorld::addActor(Actor* a)
{
	m_contain.push_front(a);
}
void StudentWorld::recordLevelFinishedIfAllCitizensGone()
{
	m_fin = true;
}
int StudentWorld::init()
{
	m_fin = false;
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
		return GWSTATUS_PLAYER_WON;
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
						addActor(new Wall(this, SPRITE_WIDTH*i, SPRITE_HEIGHT*j));
						break; // Note: 'this' pointer in parameters above get sent to Actor's m_wld
					case Level::player:
						addActor(new Penelope(this, SPRITE_WIDTH*i, SPRITE_HEIGHT*j));
						m_player = m_contain.begin(); // Set iterator to point to player
						break;
					case Level::exit:
						addActor(new Exit(this, SPRITE_WIDTH*i, SPRITE_HEIGHT*j));
						break;
					case Level::vaccine_goodie:
						addActor(new VaccineGoodie(this, SPRITE_WIDTH*i, SPRITE_HEIGHT*j));
						break;
					case Level::gas_can_goodie:
						addActor(new GasCanGoodie(this, SPRITE_WIDTH*i, SPRITE_HEIGHT*j));
						break;
					case Level::landmine_goodie:
						addActor(new LandmineGoodie(this, SPRITE_WIDTH*i, SPRITE_HEIGHT*j));
						break;
					case Level::citizen:
						addActor(new Citizen(this, SPRITE_WIDTH*i, SPRITE_HEIGHT*j));
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
		if(m_fin)
			return GWSTATUS_FINISHED_LEVEL;
		m_key = -1;
		getKey(m_key); // Sets m_key to current key pressed. Useful later

		for(list<Actor*>::iterator p = m_contain.begin(); p != m_contain.end(); p++)
		{
				(*p)->doSomething();
				if(p != m_player && (*p)->isDead() && !(*p)->blocksFlame() && !(*p)->isPit())
				{
						delete *p;
						p = m_contain.erase(p);
				}
		}
		if((*m_player)->ptr()->isDead()) // Mortal's isAlive() func used
		{
			decLives();
			return GWSTATUS_PLAYER_DIED; // Player died, end game
		}
		//// REMOVE DEAD ACTORS
		//// PRINT STATS
		ostringstream oss;
		oss << "Score: " << statScore(getScore()) << "  Level:  " << getLevel() << "  Lives: "
		<< getLives() << "  Vacc: " << (*m_player)->ptr()->getNumVaccines() << "  Flames: " <<
		(*m_player)->ptr()->getNumFlameCharges() <<	"  Mines: " << (*m_player)->ptr()->getNumLandmines()	<<
		"  Infected: " << (*m_player)->ptr()->getInfectionDuration();
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
bool StudentWorld::isAgentMovementBlockedAt(double col, double row)
{
	for(list<Actor*>::iterator p = m_contain.begin(); p != m_contain.end(); p++)
	{ // Checks for each actor in container
		double dx = (*p)->getX() - col; double dy = (*p)->getY() - row;
		dx = (dx >= 0 ? dx/16 : (-dx)/16); // Absolute Value
		dy = (dy >= 0 ? dy/16 : (-dy)/16);
		double diff = (dx > dy ? dx : dy); // Square Metric to determine collision
		if(p != m_player && (*p)->blocksMovement() && diff < 1)
		{
			// cerr << "Pos: " << (*p)->getX() << "," << (*p)->getY() << endl;
			// cerr << "Dest: " << col << "," << row << endl;
			// cerr << "diff: " << diff << endl;
			return true; // BOXES INTERSECT
		}
	}
	return false; // ALL BOXES DONT INTERSECT
}

void StudentWorld::activateOnAppropriateActors(Actor* a)
{
	double x_a = a->getX();
	double y_a = a->getY();
	for(list<Actor*>::iterator p = m_contain.begin(); p != m_contain.end(); p++)
	{
		double dx = (*p)->getX() - x_a; double dy = (*p)->getY() - y_a;
		if((dx * dx) + (dy * dy) <= 100)
			a->activateIfAppropriate(*p);
	}
}
