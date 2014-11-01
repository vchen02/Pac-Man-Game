#ifndef ACTOR_H
#define ACTOR_H

#include "BGIgraphics.h"	//For Actor Color
#include "constants.h"		//For Actor ID
#include "GraphManager.h"	//For Directions

class World;
/***************************************************************/
//Actor Declarations

enum ActorState
{ NORMAL, VULNERABLE, MONSTERDIE, RETURNTOHOME, ALIVE, DEAD, NA	};

class Actor
{
public:
	Actor(World* w, int ActorID, colors ac, int x, int y);
	virtual ~Actor();
	virtual void doSomething()	= 0;
	void MoveActorInCurrentDirection(bool Monster);	
	
	//	Getters
	virtual colors GetDisplayColor() const;
	int getMyID() const;
	int GetX() const;
	int GetY() const;
	ActorState GetActorState() const;
	Direction GetActorDirection() const;
	Direction GetActor_prev_Direction() const;
	bool GetCurrentTickMove() const;
	
	//	Setters
	void SetDisplayColor(int ID);
	void SetActorState(ActorState ac, bool monster);
	void SetActorDirection(Direction dir);
	void SetActor_prev_Direction(Direction dir);
	void SetX(int x);
	void SetY(int y);
	
private:
	
	World*		m_world;
	ActorState	m_state;
	colors		m_ActorColor;
	bool		TickDirectionSet;
	int			m_actorID;
	int			m_coordX;
	int			m_coordY;
	Direction	m_pdir;		//Holds Direction of previous tick 
	Direction	m_dir;		//Holds Direction of current tick
};

/***************************************************************/
//NachMan Declarations

class NachMan: public Actor
{
public:
	NachMan(World* w, int x, int y);
	virtual ~NachMan();
	void doSomething();
	
	//Getters
	colors GetDisplayColor() const;
	int GetScore() const;
	int GetNumLivesLeft() const;

	//Setters
	void DecrementNumLives();
	void SetNachScore(int score);
		
private:
	void checkNachDirection(Direction dir);
	void EatMazeXY();
	
	World*		m_world;
	int			m_life;
	int			m_score;
};

/***************************************************************/
//Monster Declarations

class Monster: public Actor
{
public:
	Monster(World* w, int ActorID, colors mc, int x, int y);
	virtual	~Monster()	{}
	virtual void doSomething();			//Monsters in RETURNTOHOME or MONSTERDIE must use this doSomething()
	
	void normalMove(int x, int y);		//For Monster's movement algorithm in normal or vulnerable state
	void randomMove();					//Randomly picks a x & y and move closer to it
	void fastMove(int x, int y);		//Only for monsters in RETURNTOHOME state follows this movement algorithm
	void sameNachManXY(ActorState ms);	//Do appropriate behavior when landing on same XY with Nachman
	
	//	Getters
	int GetVulnTicks() const;
	
	//	Setters
	void DecremVulnTicks();
	void SetToVulnState();
	
private:

	void checkDirection(const Direction dir); 
	Direction EastWestCol(int x, int y) const;
	Direction NorthSouthRow(int x, int y) const;
	
	World*			m_world;
	int				nVulnerableTicks;
};

/***************************************************************/
//Clyde  Declarations

class Clyde: public Monster
{
public:
	Clyde(World* w, int x, int y);
	virtual	~Clyde()	{}
	void doSomething();
	
private:
	void find_n_setNachManQuadrant(int& x, int& y) const;
	
	World*			m_world;	
};

/***************************************************************/
//Stinky  Declarations

class Stinky: public Monster
{
public:
	Stinky(World* w, int x, int y);
	virtual	~Stinky()	{}
	void doSomething();
	
private:
	bool find_n_setNachMan5() const;
	
	World*			m_world;
};

/***************************************************************/
//Dinky  Declarations

class Dinky: public Monster
{
public:
	Dinky(World* w, int x, int y);
	virtual	~Dinky()	{}
	void doSomething();
	
private:
	bool find_n_setNachManLine() const;
	
	World*			m_world;
};

/***************************************************************/
//Inky  Declarations

class Inky: public Monster
{
public:
	Inky(World* w, int x, int y);
	virtual	~Inky()	{}
	void doSomething();
	
private:
	void InkyDecide();
	
	World*			m_world;
	ActorState		m_prevState;
	int				ChaseCounter;
	int				CruiseCounter;
};

#endif	//ACTOR_H