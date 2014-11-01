#include "Actor.h"
#include "GraphManager.h"	//For Directions
#include "UserInterface.h"	//To get user input
#include "BGIgraphics.h"	//Actor colors
#include "Maze.h"
#include "World.h"

/***************************************************************/
//Actor Implementations

Actor::Actor(World* w, int ActorID, int x, int y)
: m_world(w), m_actorID(ActorID), m_coordX(x), m_coordY(y)
{
	
}

Actor::~Actor()
{
	
}

/***************************************************************/
//NachMan Implementations

NachMan::NachMan(World* w, int x, int y)
: Actor(w, ITEM_NACHMAN, x, y), m_world(w), m_life(3), m_dir(NONE), m_pdir(NONE), m_score(0), m_alive(false)
{
	
}

NachMan::~NachMan()
{
	
}

void NachMan::doSomething()
{
	char ch;
	if (getCharIfAny(ch))
	{
		switch(ch)
		{
			case ARROW_LEFT:	SetNachDirection(WEST);	break;
			case ARROW_RIGHT:	SetNachDirection(EAST);	break;
			case ARROW_UP:		SetNachDirection(NORTH);break;
			case ARROW_DOWN:    SetNachDirection(SOUTH);break;
			default:	break;
		}
	}
	MoveNachManInCurrentDirection();
	EatMazeXY();
}

void NachMan::SetNachDirection(Direction dir)
{
	Maze* cur_maze	= m_world->GetMaze();
	switch(dir)
	{
		case WEST:	
			if(cur_maze->GetGridContents(GetX()-1, GetY()) == WALL || cur_maze->GetGridContents(GetX()-1, GetY()) == CAGEDOOR)
				m_dir	= m_pdir;
			else
				m_dir	= WEST;		
			break;
		case EAST:	
			if(cur_maze->GetGridContents(GetX()+1, GetY()) == WALL || cur_maze->GetGridContents(GetX()+1, GetY()) == CAGEDOOR)
				m_dir	= m_pdir;
			else
				m_dir	= EAST;		
			break;
		case NORTH:	
			if(cur_maze->GetGridContents(GetX(), GetY()-1) == WALL || cur_maze->GetGridContents(GetX(), GetY()-1) == CAGEDOOR)
				m_dir	= m_pdir;
			else
				m_dir	= NORTH;	
			break;
		case SOUTH:	
			if(cur_maze->GetGridContents(GetX(), GetY()+1) == WALL || cur_maze->GetGridContents(GetX(), GetY()+1) == CAGEDOOR)
				m_dir	= m_pdir;
			else
				m_dir	= SOUTH;	
			break;
		default:		break;
	}
	m_pdir	= m_dir;
}

void NachMan::MoveNachManInCurrentDirection()
{
	Maze* cur_maze	= m_world->GetMaze();
	switch(m_dir)
	{
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
			break;
		default:	break;
	}
}

/***************************************************************/
//Monster Implementations
/*
Monster::Monster(World* w, int ActorID, colors mc, int x, int y)
: m_world(w), Actor(ActorID, x, y), m_monColor(mc), m_state(NA), nVulnerableTicks(0)
{

}

	
/***************************************************************/	
//Clyde Implementations
/*
Clyde::Clyde(World* w, int x, int y)
: m_world(w), Monster(w, ITEM_MONSTER4, LIGHTCYAN, NORMAL, x, y), nVulnerableTicks(0), m_pdir(NONE)
{

}

void Clyde::doSomething()
{
	
}

void Clyde::SetToVulnState()
{
	SetDisplayColor(LIGHTBLUE);
	/*if(nVulnerableTicks > 0)
		...
	else
		SetDisplayColor(LIGHTCYAN)
	*/
//}

/***************************************************************/	
//Stinky Implementations
/*
Stinky::Stinky(World* w, int x, int y)
: m_world(w), Monster(w, ITEM_MONSTER2, LIGHTGREEN, NORMAL, x, y), nVulnerableTicks(0), m_pdir(NONE)
{
	
}

void Stinky::doSomething()
{
	
}

void Stinky::SetToVulnState()
{
	SetDisplayColor(LIGHTBLUE);
	/*if(nVulnerableTicks > 0)
		...
	else
		SetDisplayColor(LIGHTGREEN)
	*/
//}

/***************************************************************/	
//Dinky Implementations
/*
Dinky::Dinky(World* w, int x, int y)
: m_world(w), Monster(w, ITEM_MONSTER3, LIGHTMAGENTA, NORMAL, x, y), nVulnerableTicks(0), m_pdir(NONE)
{
	
}

void Dinky::doSomething()
{
	
}

void Dinky::SetToVulnState()
{
	SetDisplayColor(LIGHTBLUE);
	/*if(nVulnerableTicks > 0)
		...
	else
		SetDisplayColor(LIGHTMAGENTA)
	*/	
//}

/***************************************************************/	
//Inky Implementations
/*
Inky::Inky(World* w, int x, int y)
: m_world(w), Monster(w, ITEM_MONSTER1, LIGHTRED, NORMAL, x, y), nVulnerableTicks(0), m_pdir(NONE)
{
	
}

void Inky::doSomething()
{
	
}

void Inky::SetToVulnState()
{
	SetDisplayColor(LIGHTBLUE);
	/*if(nVulnerableTicks > 0)
		...
	else
		SetDisplayColor(LIGHTRED)
	*/	
//}
