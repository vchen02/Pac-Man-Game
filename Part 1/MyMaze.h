#ifndef _MYMAZE_H_
#define _MYMAZE_H_

#include "Maze.h"

class MyMaze: public Maze
{
  public:

	MyMaze(GraphManager* gm)
		: Maze(gm)
	{
	}

  // You may add additional code
	virtual bool GetNextCoordinate(int nCurX, int nCurY, int &nNextX, int &nNextY)
	{
		return false;
	}
};
#endif	//_MYMAZE_H_