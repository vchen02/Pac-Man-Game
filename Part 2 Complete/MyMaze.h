#ifndef _MYMAZE_H_
#define _MYMAZE_H_

#include "Maze.h"
#include <stack>

class MyMaze: public Maze
{
public:
	MyMaze(GraphManager* gm)
		: Maze(gm)
	{
	}
	bool LoadMaze(const std::string &MazeFile);
	bool GetNextCoordinate(int nCurX, int nCurY, int &nNextX, int &nNextY);
	
	class Coord
	{
	public:
		Coord(int rr, int cc) : m_r(rr), m_c(cc) {}
		int r() const { return m_r; }
		int c() const { return m_c; }
	private:
		int m_r;
		int m_c;
	};
	
	
private:
	void createShortestDistance(int sr, int sc, int m_dist[][MAZE_WIDTH]);
	int		m_dist[MAZE_HEIGHT][MAZE_WIDTH];
 
};
#endif	//_MYMAZE_H_