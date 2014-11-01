#include "MyWorld.h"
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
		Nach->SetNachManState(true);		//This sets NachMan direction to NONE
		Nach->SetX(cur_maze->GetNachManStartX());
		Nach->SetY(cur_maze->GetNachManStartY());
	}
	//Monster Initializations
	
	DisplayScreen(true);

	while(Nach->GetNachManState() &&  cur_maze->GetRemainingFood() > 0)
	{
		Nach->doSomething();		//Allow NachMan to move
			
									//If NachMan is still alive, allow monsters to move
		DisplayScreen(false);
	}
	
	if (cur_maze->GetRemainingFood() == 0)
		return FINISHED_LEVEL;
	else
		return PLAYER_DIED;
}
