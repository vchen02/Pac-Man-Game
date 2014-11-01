#include "MyWorld.h"
#include "World.h"
#include "GraphManager.h"
#include "Actor.h"
#include "Maze.h"

GameStatus MyWorld::RunLevel()
{
	//Nachman Initializations
	NachMan* Nach		= GetNachMan();
	Maze*	 cur_maze	= GetMaze();
	if (Nach->GetNumLivesLeft() > 0)
	{
		Nach->SetActorState(ALIVE, false);			//This sets NachMan direction to NONE
		Nach->SetX(cur_maze->GetNachManStartX());
		Nach->SetY(cur_maze->GetNachManStartY());
	}
	//Monster Initializations
	Actor* Monst;
	for (int i=0;i<NUM_MONSTERS;i++)
	{
		Monst = GetMonster(i);
		Monst->SetActorState(NORMAL, true);
		Monst->SetX(cur_maze->GetMonsterStartX());
		Monst->SetY(cur_maze->GetMonsterStartY());
	}

	DisplayScreen(true);
	
	while(Nach->GetActorState() == ALIVE &&  cur_maze->GetRemainingFood() > 0)
	{
		Nach->doSomething();		//Allow NachMan to move	
		//If NachMan is still alive, allow monsters to move
		if(Nach->GetActorState() == ALIVE)				{
			for (int j = 0; j <NUM_MONSTERS; j++)	{
				Monst = GetMonster(j);
				Monst->doSomething();				}	}
		DisplayScreen(false);
	}
	
	if (cur_maze->GetRemainingFood() == 0)
		return FINISHED_LEVEL;
	else
		return PLAYER_DIED;
}