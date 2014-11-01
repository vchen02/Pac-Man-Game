#include "MyMaze.h"
#include "GraphManager.h"
#include <stack>
#include <iostream>
using namespace std;

bool MyMaze::LoadMaze(const std::string &MazeFile)
{
	if(Maze::LoadMaze(MazeFile) )
	{
		createShortestDistance(GetMonsterStartY(), GetMonsterStartX(), m_dist);
		return true;
	}
	else 
		return false;
}

bool MyMaze::GetNextCoordinate(int nCurX, int nCurY, int &nNextX, int &nNextY) 
{
	int current	= m_dist[nCurY][nCurX];
	
	if (current == 0)
		return false;
	else 
	{
		if ( m_dist[nCurY-1][nCurX] == current-1)	//North
		{
			nNextY--;
			return true;
		}
		if ( m_dist[nCurY][nCurX+1] == current-1)	//East
		{
			nNextX++;
			return true;
		}
		if ( m_dist[nCurY+1][nCurX] == current-1)	//South
		{
			nNextY++;
			return true;
		}
		if ( m_dist[nCurY][nCurX-1] == current-1)	//West
		{
			nNextX--;
			return true;
		}
	}
	return false;
}

void MyMaze::createShortestDistance(int sr, int sc, int m_dist[][MAZE_WIDTH])
{
	//Set every element of the m_distance array to 99.
	for (int row = 0; row < MAZE_HEIGHT; row ++)	{
		for (int col = 0; col < MAZE_WIDTH; col ++)
			m_dist[row][col]	= 99;
	}
	
	//Set position (sr,sc) of the m_distance array to 0.
	m_dist [sr][sc]	= 0;
	
	//Push the starting coordinate (sr,sc) onto the coordinate stack.
	stack<Coord> coordStack;
	coordStack.push(Coord(sr, sc));
	
	while (!coordStack.empty())
	{
		Coord cur	= coordStack.top();		//uses Copy Contructor of Coord class
		coordStack.pop();
		
		/*
		 m_dist [cur.r()-1][cur.c()]			//above (NORTH) of current location
		 m_dist [cur.r()+1][cur.c()]			//below (SOUTH) of current location
		 m_dist [cur.r()][cur.c()-1]			//left  (WEST) of current location
		 m_dist [cur.r()][cur.c()+1]			//right (EAST) of current location
		 */
		
		int m	=	m_dist [cur.r()-1][cur.c()];	//finds the smallest value between the 4 directions
		if (m_dist [cur.r()+1][cur.c()] < m)	   //directions centered at m_dist [cur.r()][cur.c()]
			m	= m_dist [cur.r()+1][cur.c()];
		if (m_dist [cur.r()][cur.c()-1] < m)
			m	= m_dist [cur.r()][cur.c()-1];
		if (m_dist [cur.r()][cur.c()+1] < m)
			m	= m_dist [cur.r()][cur.c()+1];
		
		if (m_dist [cur.r()][cur.c()] > m+1)	//reached a number on m_distance array thats smaller than current path
			m_dist [cur.r()][cur.c()]	= m+1;	//prevent overwriting previous path on m_distance array
		
		const int v	=	m_dist [cur.r()][cur.c()];	//holds current value on m_distance array
		//Move North?
		if (GetGridContents(cur.c(), cur.r()-1) != WALL && m_dist [cur.r()-1][cur.c()] > v+1)
		{	
			m_dist [cur.r()-1][cur.c()]	= v+1;
			coordStack.push(Coord(cur.r()-1, cur.c()));
		}
		//Move East?
		if (GetGridContents(cur.c()+1, cur.r()) != WALL && m_dist [cur.r()][cur.c()+1] > v+1)
		{
			m_dist [cur.r()][cur.c()+1]	= v+1;
			coordStack.push(Coord(cur.r(), cur.c()+1));
		}
		//Move South?
		if (GetGridContents(cur.c(), cur.r()+1) != WALL && m_dist [cur.r()+1][cur.c()] > v+1)
		{
			m_dist [cur.r()+1][cur.c()]	= v+1;
			coordStack.push(Coord(cur.r()+1, cur.c()));
		}
		//Move West?
		if (GetGridContents(cur.c()-1, cur.r()) != WALL && m_dist [cur.r()][cur.c()-1] > v+1)
		{
			m_dist [cur.r()][cur.c()-1]	= v+1;
			coordStack.push(Coord(cur.r(), cur.c()-1));
		}
	}
}