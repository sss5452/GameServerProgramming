#pragma once
#include <iostream>
#include <list>
#include <algorithm>
#include <tuple>
#include <vector>
#include "protocol.h"


using namespace std;

struct ASNode {
	ASNode* conn;
	int row, col; //index
	int g, h, f;
	GRID_TYPE nodeName;
	char value;

	ASNode(int _x, int _y, char _v, GRID_TYPE _i)
	{
		row = _x;
		col = _y;
		value = _v;
		nodeName = _i;
		g = 0;
		h = 0;
		f = 0;
	}
};

struct Pos {
	int x;
	int y;
	bool clear;
};

static char uniqueName = 'a';
class CAstar2
{
public:
	list<ASNode*> openList;
	list<ASNode*> closeList;
	//MAP map[200][200];
	MAP **map;
	void SetPlayerPos(int x, int y, int px, int py);
	ASNode* GetChildNodes(int childIndRow, int childIndCol, ASNode* parentNode);
	ASNode* CreateNodeByIndex(int rowIndex, int colIndex, ASNode* parentNode);
	tuple<int, int> GetGoalIndex();
	void FindPath();
	Pos Walk();
	Pos FirstWalk();
	vector<Pos> path;
	int workcount;
	int m_x;
	int m_y;
	bool pathclear = false;

public:
	CAstar2();
	~CAstar2();
};

