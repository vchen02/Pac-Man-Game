#ifndef ACTOR_H
#define ACTOR_H

//#include <string>
#include "BGIgraphics.h"	//For Actor Color
#include "constants.h"		//For Actor ID
#include "GraphManager.h"	//For Directions

class World;
/***************************************************************/
//Actor Declarations

class Actor
{
public:
	Actor(World* w, int ActorID, int x, int y);
	virtual ~Actor();
	virtual void doSomething()	= 0;
	
	//	Getters
	virtual colors GetDisplayColor() const = 0;
	int getMyID() const
	{
		return m_actorID;
	}
	int GetX() const
	{
		return m_coordX;
	}
	
	int GetY() const
	{
		return m_coordY;
	}
	
	//	Setters
	void SetX(int x)
	{
		m_coordX	= x;
	}
	
	void SetY(int y)
	{
		m_coordY	= y;
	}
	
private:
	World*	m_world;
	int		m_actorID;
	int		m_coordX;
	int		m_coordY;
};

/***************************************************************/
//NachMan Declarations

class NachMan: public Actor
{
public:
	NachMan(World* w, int x, int y);
	virtual ~NachMan();
	void doSomething();
	void MoveNachManInCurrentDirection();
	void EatMazeXY();
	void DecrementNumLives()
	{
		m_life --;
	}
	
	//	Setters
	void SetNachDirection(Direction dir);
	void SetNachScore(int score)
	{
		m_score += score;
	}
	void SetNachManState(bool alive)
	{
		m_dir	= NONE;
		m_pdir	= NONE;
		m_alive	= alive;
	}
	
	//	Getters
	bool GetNachManState() const
	{
		return m_alive;
	}
	int GetScore() const
	{
		return m_score;
	}
	colors GetDisplayColor() const
	{
		return YELLOW;
	}
	int GetNumLivesLeft() const
	{
		return m_life;
	}
	
private:
	World*		m_world;
	int			m_life;
	Direction	m_pdir;		//Holds Direction of previous tick 
	Direction	m_dir;		//Holds Direction of current tick
	int			m_score;
	bool		m_alive;	//returns true if NachMan is Alive
};


/***************************************************************/
//Monster Declarations
/*
enum MonsterState
{	NA, NORMAL, VULNERABLE, MONSTERDIE, RETURNTOHOME	};

class Monster: public Actor
{
public:
	Monster(World* w, int ActorID, colors mc, MonsterState MS, int x, int y);
	virtual	~Monster()	{}
	
	colors GetDisplayColor()
	{
		return	m_monColor;
	}
	void SetDisplayColor(colors mc)
	{
		m_monColor	= mc;
	}
	void SetToNormalState()
	{
		m_state	= NORMAL;
	}
	virtual void SetToVulnState()	= 0;
	void SetToMonsterDieState()
	{
		m_state	= MONSTERDIE;
		SoundFX::playNachManSound(PAC_SOUND_BIG_EAT);
		SetToReturnHomeState();
	}
	void SetToReturnHomeState()
	{
		m_state	= RETURNTOHOME;
		//Picks optimum direction to return to start position.
		/*if(start monster position x,y == current monster x, y)
			SetToNormalState();
		else
			continue moving toward start monster position x,y
		*/
//	}
/*	
private:
	int				nVulnerableTicks;
	World*			m_world;
	MonsterState	m_state;
	colors			m_monColor;
};

/***************************************************************/
//Clyde  Declarations
/*
class Clyde: public Monster
{
public:
	Clyde(World* w, int x, int y);
	virtual	~Clyde()	{}
	void doSomething();
	void SetToVulnState();

private:
	
	Direction		m_pdir;		//Holds Direction of previous tick 
	//Direction		m_dir;		//Holds Direction of current tick
	World*			m_world;	
};

/***************************************************************/
//Stinky  Declarations
/*
class Stinky: public Monster
{
public:
	Stinky(World* w, int x, int y);
	virtual	~Stinky()	{}
	void doSomething();
	void SetToVulnState();
	
private:
	Direction		m_pdir;		//Holds Direction of previous tick 
	//Direction		m_dir;		//Holds Direction of current tick
	World*			m_world;
};

/***************************************************************/
//Dinky  Declarations
/*
class Dinky: public Monster
{
public:
	Dinky(World* w, int x, int y);
	virtual	~Dinky()	{}
	void doSomething();
	void SetToVulnState();
	
private:
	Direction		m_pdir;		//Holds Direction of previous tick 
	//Direction		m_dir;		//Holds Direction of current tick
	World*			m_world;
};

/***************************************************************/
//Inky  Declarations
/*
class Inky: public Monster
{
public:
	Inky(World* w, int x, int y);
	virtual	~Inky()	{}
	void doSomething();
	void SetToVulnState();
	
private:
	Direction		m_pdir;		//Holds Direction of previous tick 
	Direction		m_dir;		//Holds Direction of current tick
	World*			m_world;
};
*/
#endif	//ACTOR_H