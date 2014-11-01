#include "Actor.h"
#include "MyMaze.h"
#include "World.h"

#include "GraphManager.h"	//For Directions
#include "UserInterface.h"	//To get user input
#include "BGIgraphics.h"	//Actor colors

#include <cstdlib>
#include <ctime>
#include <iostream>
using namespace std;

/***************************************************************/
//Actor Implementations

Actor::Actor(World* w, int ActorID, colors ac, int x, int y)
: m_world(w), m_actorID(ActorID), m_state(NA), m_ActorColor(ac), TickDirectionSet(false), m_coordX(x), m_coordY(y), m_dir(NONE), m_pdir(NONE)
{}
																												  
Actor::~Actor()
{}

//	Getters
colors Actor::GetDisplayColor() const
{
	return m_ActorColor;
}
int Actor::getMyID() const
{
	return m_actorID;
}
int Actor::GetX() const
{
	return m_coordX;
}
int Actor::GetY() const
{
	return m_coordY;
}
ActorState Actor::GetActorState() const
{
	return m_state;
}
Direction Actor::GetActorDirection() const
{
	return m_dir;
}
Direction Actor::GetActor_prev_Direction() const
{
	return m_pdir;
}
bool Actor::GetCurrentTickMove() const
{	
	return TickDirectionSet;
}

//Setters
void Actor::SetActorDirection(Direction dir)
{
	m_dir				= dir;
	TickDirectionSet	= true;
}
void Actor::SetActor_prev_Direction(Direction dir)
{
	m_pdir	= dir;
	TickDirectionSet	= false;
}
void Actor::SetX(int x)
{
	m_coordX	= x;
}

void Actor::SetY(int y)
{
	m_coordY	= y;
}
			
void Actor::MoveActorInCurrentDirection(bool Monster)
{
	Maze* cur_maze	= m_world->GetMaze();
	if (Monster)	{	//Monster Move
		switch(m_dir)			{
			case WEST: 
				if(cur_maze->GetGridContents(GetX()-1, GetY()) != WALL )
					SetX(GetX()-1);
				break;
			case EAST: 
				if(cur_maze->GetGridContents(GetX()+1, GetY()) != WALL )
					SetX(GetX()+1);
				break;
			case NORTH: 
				if(cur_maze->GetGridContents(GetX(), GetY()-1) != WALL )
					SetY(GetY()-1);
				break;
			case SOUTH:	
				if(cur_maze->GetGridContents(GetX(), GetY()+1) != WALL )
					SetY(GetY()+1);
				break;
			default:	break;	}
	}
	else	{			//NachMan Move
		switch(m_dir)	{
			case WEST: 
				if(cur_maze->GetGridContents(GetX()-1, GetY()) != WALL && cur_maze->GetGridContents(GetX()-1, GetY()) != CAGEDOOR)
					SetX(GetX()-1);
				break;
			case EAST: 
				if(cur_maze->GetGridContents(GetX()+1, GetY()) != WALL && cur_maze->GetGridContents(GetX()+1, GetY()) != CAGEDOOR)
					SetX(GetX()+1);
				break;
			case NORTH: 
				if(cur_maze->GetGridContents(GetX(), GetY()-1) != WALL && cur_maze->GetGridContents(GetX(), GetY()-1) != CAGEDOOR)
					SetY(GetY()-1);
				break;
			case SOUTH:	
				if(cur_maze->GetGridContents(GetX(), GetY()+1) != WALL && cur_maze->GetGridContents(GetX(), GetY()+1) != CAGEDOOR)
					SetY(GetY()+1);
				break;
			default:	break;
			}		}
}		
		
void Actor::SetDisplayColor(int ID)
{
	switch (ID)			{
		case 0: m_ActorColor	= LIGHTBLUE;	break;		//Monster in Vulnerable State Color
		case 1: m_ActorColor	= LIGHTGRAY;	break;		//Monster in ReturnToHome State Color
		case 4:	m_ActorColor	= LIGHTRED;		break;		
		case 5:	m_ActorColor	= LIGHTGREEN;	break;
		case 6: m_ActorColor	= LIGHTMAGENTA; break;
		case 7: m_ActorColor	= LIGHTCYAN;	break;
		default:	break;
						}
}

void Actor::SetActorState(ActorState ac, bool monster) 
{
	if (monster)			{
		switch(ac)	{
			case NORMAL: SetDisplayColor(getMyID());	break;
			case MONSTERDIE: SoundFX::playNachManSound(PAC_SOUND_BIG_EAT);	break;
			case RETURNTOHOME:	SetDisplayColor(1);	break;
			default:	break;
					}		}
	else					{
		m_dir	= NONE;
		m_pdir	= NONE;		}
	m_state	= ac;
}

/***************************************************************/
//NachMan Implementations

NachMan::NachMan(World* w, int x, int y)
: Actor(w, ITEM_NACHMAN, YELLOW, x, y), m_world(w),  m_life(3), m_score(0)
{}

NachMan::~NachMan()
{}

void NachMan::DecrementNumLives()
{
	m_life --;
}

//	Setters
void NachMan::SetNachScore(int score)
{
	m_score += score;
}

//	Getters	
colors NachMan::GetDisplayColor() const
{
	return YELLOW;
}
int NachMan::GetScore() const
{		
	return m_score;
}	
int NachMan::GetNumLivesLeft() const
{		
	return m_life;
}

void NachMan::doSomething()
{
	char ch;
	if (getCharIfAny(ch))				{
		switch(ch)		{
			case ARROW_LEFT:	checkNachDirection(WEST);	break;
			case ARROW_RIGHT:	checkNachDirection(EAST);	break;
			case ARROW_UP:		checkNachDirection(NORTH);	break;
			case ARROW_DOWN:    checkNachDirection(SOUTH);	break;
			default:	break;
						}				}
	MoveActorInCurrentDirection(false);
	SetActor_prev_Direction(GetActorDirection());
	EatMazeXY();	
	
	//	Check if NachMan Landed on same coordinate as Monsters
	for (int z = 0; z < NUM_MONSTERS; z++)	
	{
		Actor* Monst	= m_world->GetMonster(z);
		
		if (GetX() == Monst->GetX() && GetY() == Monst->GetY() )
		{
			//Monster in VULNERABLE State
			if (Monst->GetActorState() == VULNERABLE)
			{
				SetNachScore(1000);
				Monst->SetActorState(MONSTERDIE, true);
			}
			
			//Monster in NORMAL State
			if (Monst->GetActorState() == NORMAL)
				SetActorState(DEAD, false);
		}
	}
}

void NachMan::EatMazeXY()
{
	Maze* cur_maze	= m_world->GetMaze();
	GridContents gc	= cur_maze->GetGridContents(GetX(), GetY());
	switch(gc)		//EMPTY, WALL, PELLET, POWERPELLET, CAGEDOOR
	{	
		case PELLET:	
			cur_maze->SetGridContents(GetX(), GetY(), EMPTY);		//This also decrements the remaining food by 1
			m_score += 10;
			SoundFX::playNachManSound(PAC_SOUND_SMALL_EAT);
			break;
		case POWERPELLET: 
			cur_maze->SetGridContents(GetX(), GetY(), EMPTY);		//This also decrements the remaining food by 1
			m_score += 100;
			SoundFX::playNachManSound(PAC_SOUND_BIG_EAT);
		
			for (int i=0;i<NUM_MONSTERS;i++)						//Set all monsters to Vulnerable state if possible
			{
				Monster* Monst = m_world->GetMonster(i);
				if (Monst->GetActorState() == NORMAL || Monst->GetActorState() == VULNERABLE)
					Monst->SetToVulnState();
			}		
			break;
		default:	break;
	}
}

void NachMan::checkNachDirection(Direction dir)
{
	Maze* cur_maze	= m_world->GetMaze();
	switch(dir)
	{
		case WEST:	
			if(cur_maze->GetGridContents(GetX()-1, GetY()) == WALL || cur_maze->GetGridContents(GetX()-1, GetY()) == CAGEDOOR)
				SetActorDirection(GetActor_prev_Direction());
			else
				SetActorDirection(WEST);		
			break;
		case EAST:	
			if(cur_maze->GetGridContents(GetX()+1, GetY()) == WALL || cur_maze->GetGridContents(GetX()+1, GetY()) == CAGEDOOR)
				SetActorDirection(GetActor_prev_Direction());
			else
				SetActorDirection(EAST);		
			break;
		case NORTH:	
			if(cur_maze->GetGridContents(GetX(), GetY()-1) == WALL || cur_maze->GetGridContents(GetX(), GetY()-1) == CAGEDOOR)
				SetActorDirection(GetActor_prev_Direction());
			else
				SetActorDirection(NORTH);	
			break;
		case SOUTH:	
			if(cur_maze->GetGridContents(GetX(), GetY()+1) == WALL || cur_maze->GetGridContents(GetX(), GetY()+1) == CAGEDOOR)
				SetActorDirection(GetActor_prev_Direction());
			else
				SetActorDirection(SOUTH);	
			break;
		default:		break;
	}
}

/***************************************************************/
//Monster Implementations

Monster::Monster(World* w, int ActorID, colors mc, int x, int y)
: m_world(w), Actor(w, ActorID, mc, x, y), nVulnerableTicks(0)
{}

//	Getters
int Monster::GetVulnTicks() const
{
	return nVulnerableTicks;
}

//	Setters
void Monster::DecremVulnTicks()
{
	nVulnerableTicks --;
}

void Monster::doSomething()
{
	// doSomething when Monster is in MONSTERDIE state
	if (GetActorState() == MONSTERDIE)
	{	
		SetActorState(RETURNTOHOME, true);			
		return;						//No TICK MOVEMENT
	}
	
	//	doSomething when Monster is in RETURNTOHOME state
	if (GetActorState() == RETURNTOHOME)
	{
		Maze* cur_maze	= m_world->GetMaze();
		int newX	= GetX();
		int newY	= GetY();
		cur_maze->GetNextCoordinate(GetX(), GetY(), newX, newY);
		fastMove(newX, newY);		//CURRENT TICK MOVEMENT
		if (GetX() == cur_maze->GetMonsterStartX() && GetY() == cur_maze->GetMonsterStartY() )
			SetActorState(NORMAL, true);
		sameNachManXY(RETURNTOHOME);//Will do nothing
		return;
	}
	
	// doSomething when Monster is in VULNERABLE STATE
	if(GetActorState() == VULNERABLE )
	{
		randomMove();				//CURRENT TICK MOVEMENT
		DecremVulnTicks();			//Decrement one vulnerable Tick
		if (GetVulnTicks() == 0)
			SetActorState(NORMAL, true);
		sameNachManXY(VULNERABLE);	//check current XY with NachMan current XY
		return;
	}
}

void Monster::normalMove(int x, int y)		//position x, y given here are the target positions
{
	int counter			= 0;
	Direction check;
	Maze* cur_maze		= m_world->GetMaze();
	
	if (GetActorState() == NORMAL || GetActorState() == VULNERABLE)	{
		while (counter < 2 && !GetCurrentTickMove())	
		{
			if (counter == 0) check	= EastWestCol(x,y);
			else if( counter ==1) check = NorthSouthRow(x,y);
			else	break;
			switch (check)									{
				case WEST:	checkDirection(WEST);  break;
				case EAST:	checkDirection(EAST);  break;
				case NORTH:	checkDirection(NORTH); break;
				case SOUTH:	checkDirection(SOUTH); break;	
				default: break;								}
			counter++;
		}
		counter = 0;
		
		if (!GetCurrentTickMove() )								{
			int dir				= rand() % 4;
			while (counter < 4 && !GetCurrentTickMove()) {
				switch(dir)	{
					case 0:	checkDirection(EAST);	break;
					case 1:	checkDirection(SOUTH);	break;
					case 2:	checkDirection(NORTH);	break;
					case 3:	checkDirection(WEST);	break;
					default:	break;
				}
				if(!GetCurrentTickMove())	{
					switch(dir)		{
						case 0:	
						case 1:	
						case 2:
							dir ++;		break;
						case 3: dir = 0;	break;
						default: break;
									}		}
				counter++;								}		}
		
		if( counter == 4)	{	//Monster cannot move without reversing its course, set direction to previous direction
			switch(GetActor_prev_Direction())	{
				case NORTH:	SetActorDirection(SOUTH);	break;
				case SOUTH:	SetActorDirection(NORTH);	break;
				case EAST:	SetActorDirection(WEST);	break;
				case WEST:	SetActorDirection(EAST);	break;
				default: break;
			}
		}
		MoveActorInCurrentDirection(true);
		SetActor_prev_Direction(GetActorDirection());
	}
}

void Monster::randomMove()				//Randomly picks a x & y and move closer to it	
{
	normalMove(rand() , rand());			
}

void Monster::fastMove(int x, int y)	//Do not need to check for walls because GetNextCoord from MyMaze checked it
{
	if (GetActorState() == RETURNTOHOME)	{
		if (GetX() != x)	SetX(x);
		else				SetY(y);		}
	else					return;
}

void Monster::sameNachManXY(ActorState ms)
{
	NachMan* Nach	= m_world->GetNachMan();
	if(GetX() == Nach->GetX() && GetY() == Nach->GetY() )	{
		if (ms == NORMAL )			//	MonsterState is NORMAL
			Nach->SetActorState(DEAD, false);
		else if (ms == VULNERABLE )	//	MonsterState is VULNERABLE
		{
			Nach->SetNachScore(1000);
			SetActorState(MONSTERDIE, true);
		}
		else	return;				//Monster is RETURNTOHOME 
															}
}

void Monster::SetToVulnState()
{
	SetDisplayColor(0);
	SetActorState(VULNERABLE, true);
	int curLevel	= m_world->GetLevel();
	if (curLevel <= 8)			
		nVulnerableTicks = 100 - (curLevel * 10);
	else
		nVulnerableTicks = 20;
}

void Monster::checkDirection(const Direction dir) 		//return true if its valid move, else false
{
	Maze* cur_maze	= m_world->GetMaze();
	switch(dir)				{
		case NORTH:
			if (cur_maze->GetGridContents(GetX(), GetY()-1) != WALL && GetActor_prev_Direction() != SOUTH )	//Not in same row, set direction toward NORTH
				SetActorDirection(NORTH);
			break;
		case SOUTH:
			if (cur_maze->GetGridContents(GetX(), GetY()+1) != WALL && GetActor_prev_Direction() != NORTH )	//Not in same row, set direction toward SOUTH
				SetActorDirection(SOUTH);
			break;
		case EAST:
			if (cur_maze->GetGridContents(GetX()+1, GetY()) != WALL && GetActor_prev_Direction() != WEST )		//Not in same column, set direction toward EAST
				SetActorDirection(EAST);
			break;
		case WEST:
			if(cur_maze->GetGridContents(GetX()-1, GetY()) != WALL && GetActor_prev_Direction() != EAST )		//Not in same column, set direction toward WEST
				SetActorDirection(WEST);
			break;
		default: break;		}
}

Direction Monster::EastWestCol(int x, int y) const
{
	if (GetX() == x)					{
		if (GetY() > y)	return NORTH;
		else			return SOUTH;	}
	else if (GetX() > x)return WEST;	
	else				return EAST;
}

Direction Monster::NorthSouthRow(int x, int y) const
{
	if (GetY() == y)					{
		if (GetX() > x)	return WEST;
		else			return EAST;	}
	else if (GetY() > y)return NORTH;	
	else				return SOUTH;
}

/***************************************************************/	
//Clyde Implementations

Clyde::Clyde(World* w, int x, int y)
: m_world(w), Monster(w, ITEM_MONSTER4, LIGHTCYAN, x, y)
{}

void Clyde::doSomething()
{
	//	doSomething when Clyde is in NORMAL state
	if (GetActorState() == NORMAL)		{
		randomMove();		
		sameNachManXY(NORMAL);
		return;							}
	
	//	doSomething when Clyde is in VULNERABLE state
	if (GetActorState() ==VULNERABLE )	{
		int x, y;
		find_n_setNachManQuadrant(x, y);	
		normalMove(x,y);		
		DecremVulnTicks();				//Decrement one vulnerable Tick
		if (GetVulnTicks() == 0)
			SetActorState(NORMAL, true);
		sameNachManXY(VULNERABLE);
		return;							}
	
	// doSomething when Clyde is in MONSTERDIE state
	// doSomething when Clyde is in RETURNTOHOME state
	else								{
		Monster::doSomething();
		return;							}
}

void Clyde::find_n_setNachManQuadrant(int& x, int& y) const
{
	Actor* Nach	= m_world->GetNachMan();
	
	if (Nach->GetX() >= MAZE_WIDTH/2)				{	//	Right Quadrants
		if (Nach->GetY() > MAZE_HEIGHT/2)	{			//BottomRight
			x = 0;
			y = 0;						}
		else									{		//TopRight
			x = 0;
			y = MAZE_HEIGHT - 1;			}	}
	else											{	//	Left Quadrants
		if (Nach->GetY() > MAZE_HEIGHT/2)	{			//BottomLeft
			x = MAZE_WIDTH -1;
			y = 0;						}						
		else									{		//TopLeft
			x = MAZE_WIDTH -1;
			y = MAZE_HEIGHT -1;				}	}
}

/***************************************************************/	
//Stinky Implementations

Stinky::Stinky(World* w, int x, int y)
: m_world(w), Monster(w, ITEM_MONSTER2, LIGHTGREEN, x, y)
{}

void Stinky::doSomething()
{
	// doSomething when Stinky is in NORMAL state
	if (GetActorState() == NORMAL)
	{
		Actor* Nach	= m_world->GetNachMan();
		if(find_n_setNachMan5() )
			normalMove(Nach->GetX() , Nach->GetY() );//CURRENT TICK MOVEMENT
		else	
			randomMove();							//CURRENT TICK MOVEMENT
		sameNachManXY(NORMAL);
		return;
	}
	
	// doSomething when Stinky is in MONSTERDIE state
	// doSomething when Stinky is in RETURNTOHOME state
	// doSomething when Stinky is in VULNERABLE state
	else		{
		Monster::doSomething();
		return;	}
}

bool Stinky::find_n_setNachMan5() const
{
	Actor* Nach	= m_world->GetNachMan();
	if (abs(Nach->GetX() - GetX()) <= 5 && abs(Nach->GetY() - GetY()) <= 5)
		return true;
	else
		return false;
}

/***************************************************************/	
//Dinky Implementations

Dinky::Dinky(World* w, int x, int y)
: m_world(w), Monster(w, ITEM_MONSTER3, LIGHTMAGENTA, x, y)
{}

void Dinky::doSomething()
{
	// doSomething when Dinky is in NORMAL state
	if (GetActorState() == NORMAL)
	{
		Actor* Nach	= m_world->GetNachMan();
		if(find_n_setNachManLine() )
			normalMove(Nach->GetX() , Nach->GetY() );//CURRENT TICK MOVEMENT
		else
			randomMove();
		sameNachManXY(NORMAL);
		return;
	}
	
	// doSomething when Dinky is in MONSTERDIE state
	// doSomething when Dinky is in RETURNTOHOME state
	// doSomething when Dinky is in Vulnerable state
	else			{
		Monster::doSomething();
		return;		}
}

bool Dinky::find_n_setNachManLine() const
{
	Actor* Nach		= m_world->GetNachMan();
	Maze*	 cur_maze	= m_world->GetMaze();
	if (Nach->GetY() == GetY() )				//Dinky and NachMan in same row
	{
		if (GetX() - Nach->GetX() > 0)			//Positive, so NachMan is WEST of Dinky
		{	
			for (int k = 1; k < GetX() - Nach->GetX(); k++)
			{
				if (cur_maze->GetGridContents(GetX()-k, GetY()) == WALL)
					return false;
			}
			return true;
		}
		else									//Negative, so NachMan is EAST of Dinky
		{
			for (int g = 1; g < Nach->GetX() - GetX(); g++)
			{
				if (cur_maze->GetGridContents(GetX()+g, GetY()) == WALL)
					return false;
			}
			return true;
		}
	}
	
	else if (Nach->GetX() == GetX() )			//Dinky and NachMan in same column
	{
		if (GetY() - Nach->GetY() > 0)			//Positive, so NachMan is NORTH of Dinky
		{	
			for (int k = 1; k < GetY() - Nach->GetY(); k++)
			{
				if (cur_maze->GetGridContents(GetX(), GetY()-k) == WALL)
					return false;
			}
			return true;
		}
		else									//Negative, so NachMan is SOUTH of Dinky
		{
			for (int g = 1; g < Nach->GetY() - GetY(); g++)
			{
				if (cur_maze->GetGridContents(GetX(), GetY()+g) == WALL)
					return false;
			}
			return true;
		}
	}
	
	else
		return false;
}

/***************************************************************/	
//Inky Implementations

Inky::Inky(World* w, int x, int y)
: m_world(w), Monster(w, ITEM_MONSTER1, LIGHTRED, x, y), m_prevState(NA), ChaseCounter(0), CruiseCounter(0)
{}

void Inky::doSomething()
{
	if (m_prevState != NORMAL && GetActorState() == NORMAL)
		InkyDecide();
	
	// doSomething when Inky is in NORMAL state
	if (GetActorState() == NORMAL)
	{
		Actor* Nach	= m_world->GetNachMan();
		
		if(ChaseCounter > 10)
			InkyDecide();
		else if (CruiseCounter > 10)
			InkyDecide();
		
		if (ChaseCounter > 0)
		{
			normalMove(Nach->GetX(), Nach->GetY()); //CURRENT TICK MOVEMENT
			ChaseCounter++;
		}
		else if(CruiseCounter > 0)
		{
			randomMove();							//CURRENT TICK MOVEMENT
			CruiseCounter++;
		}	
		sameNachManXY(NORMAL);
	}
	
	// doSomething when Inky is in MONSTERDIE state
	// doSomething when Inky is in RETURNTOHOME state
	// doSomething when Inky is in VULNERABLE state
	else
		Monster::doSomething();
	m_prevState	= GetActorState();
}

void Inky::InkyDecide()
{
	CruiseCounter	= 0;
	ChaseCounter	= 0;
	int InkChase	= rand() % 100;
	if (InkChase <= 80 )
		ChaseCounter	= 1;
	else 
		CruiseCounter	= 1;
}