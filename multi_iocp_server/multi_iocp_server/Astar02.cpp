#pragma once
#include "Astar02.h"

CAstar2::CAstar2() {};
CAstar2::~CAstar2() {};


extern MAP g_map[W_HEIGHT][W_WIDTH];

tuple<int, int> CAstar2::GetGoalIndex()
{
	int maxMapSizeRow = 2000;
	int maxMapSizeCol = 2000;

	for (int i = 0; i < maxMapSizeCol; i++)
	{
		for (int j = 0; j < maxMapSizeCol; j++)
		{
			if (map[j][i].type == eEND)
			{
				return make_tuple(i, j);
			}
		}
	}
}

void CAstar2::SetPlayerPos(int x, int y, int px, int py)
{
	openList.clear();
	closeList.clear();
	if (map == nullptr) {
		map = (MAP**)malloc(sizeof(MAP*) * 2000);
		for (int i = 0; i < 2000; i++) {
			map[i] = (MAP*)malloc(sizeof(MAP) * 2000);
		}

	}
	for (int i = 0; i < 2000; ++i) {
		for (int j = 0; j < 2000; ++j) {
			map[j][i].type = g_map[j][i].type;
			if (map[j][i].type == eSAFTY)map[j][i].type = eBLOCKED;
		}
	}
	m_x = x;
	m_y = y;
	map[py][px].type = eEND;

}

Pos CAstar2::Walk()
{
	
	if (workcount == 1)
		return path[1];
	else
		return path[workcount--];
}

Pos CAstar2::FirstWalk()
{


	return path[path.size()-1];
}


void CAstar2::FindPath()
{
		int maxMapSizeRow = 2000;
		int maxMapSizeCol = 2000;


		if (openList.size() == 0)
		{
			cout << "no path exists." << endl;
			return;
		}

		ASNode* openNode = nullptr;

		int smallest_f = 10000;
		for (auto& op : openList)
		{
			if (op->f < smallest_f)
			{
				smallest_f = op->f;
				openNode = op;
			}
		}

		if (openNode != nullptr)
		{
			if (openNode->nodeName == GRID_TYPE::eEND) 
			{
				path.clear();
				vector<Pos>().swap(path);
				while (openNode != nullptr)
				{
					if (openNode->nodeName == eSTART) {
						workcount = path.size() - 1;
						pathclear = true;
						break;
					}

					Pos pos;
					pos.y = openNode->row;
					pos.x = openNode->col;
					path.push_back(pos);

					int rowind = openNode->row;
					int colind = openNode->col;
					map[rowind][colind].type = GRID_TYPE::eOPEN;
					// 여기서 오픈노드 움직임 값 다 받음.
					openNode = openNode->conn;

				}

			}
			else
			{

				int rowInd = openNode->row;
				int colInd = openNode->col;
				ASNode* childNode;
				if (openNode->row - 1 > m_y - 100)
				{
					int childIndRow = openNode->row - 1;
					int childIndCol = openNode->col;

					childNode = GetChildNodes(childIndRow, childIndCol, openNode);
				}

				if (openNode->row + 1 < m_y + 100)
				{
					int childIndRow = openNode->row + 1;
					int childIndCol = openNode->col;

					childNode = GetChildNodes(childIndRow, childIndCol, openNode);
				}

				if (openNode->col + 1 < m_x + 100)
				{
					int childIndRow = openNode->row;
					int childIndCol = openNode->col + 1;

					childNode = GetChildNodes(childIndRow, childIndCol, openNode);
				}

				if (openNode->col - 1 > m_x - 100)
				{
					int childIndRow = openNode->row;
					int childIndCol = openNode->col - 1;

					childNode = GetChildNodes(childIndRow, childIndCol, openNode);
				}

				openList.remove_if([&](ASNode* node)
				{
					if (node->row == rowInd && node->col == colInd)
					{
						return true;
					}
					else
					{
						return false;
					}
				});
				closeList.push_back(openNode);

				FindPath();
			}
		}
}

ASNode* CAstar2::GetChildNodes(int childIndRow, int childIndCol, ASNode* parentNode)
{
	auto it_open = find_if(openList.begin(), openList.end(), [&](ASNode* node)
	{
		if (node->row == childIndRow && node->col == childIndCol)
		{
			return true;
		}
		else
		{
			return false;
		}
	});

	auto it_close = find_if(closeList.begin(), closeList.end(), [&](ASNode* node)
	{
		if (node->row == childIndRow && node->col == childIndCol)
		{
			return true;
		}
		else
		{
			return false;
		}
	});

	if (it_open != openList.end())
	{
		if ((*it_open)->g < parentNode->g + 1)
		{
			(*it_open)->g = parentNode->g + 1;
			parentNode->conn = (*it_open);
			(*it_open)->f = (*it_open)->g + (*it_open)->h;
		}

		return *it_open;
	}
	else if (it_close != closeList.end())
	{

		//exist
		if ((*it_close)->g < parentNode->g + 1)
		{
			(*it_close)->g = parentNode->g + 1;
			parentNode->conn = (*it_close);
			(*it_close)->f = (*it_close)->g + (*it_close)->h;
		}
		return *it_close;
	}
	else
	{
		ASNode* newNode = CreateNodeByIndex(childIndRow, childIndCol, parentNode);

		if (newNode != nullptr)
		{
			openList.push_back(newNode);
		}
		return newNode;
	}

	return nullptr;
}


ASNode* CAstar2::CreateNodeByIndex(int rowIndex, int colIndex, ASNode* parentNode)
{
	GRID_TYPE val = map[rowIndex][colIndex].type;

	if (val == GRID_TYPE::eBLOCKED)
		return nullptr;

	ASNode* node = nullptr;
	if (val == GRID_TYPE::eEND)
	{
		node = new ASNode(rowIndex, colIndex, 'G', GRID_TYPE::eEND);
		node->g = parentNode->g + 1;
		node->h = 0;
		node->f = node->g;
		node->conn = parentNode;
	}
	else
	{
		node = new ASNode(rowIndex, colIndex, val, val);
		node->g = parentNode->g + 1;

		auto inds = GetGoalIndex();
		int goalRowInd = get<0>(inds);
		int goalColInd = get<1>(inds);

		int h = abs(goalRowInd - rowIndex) + abs(goalColInd - colIndex);
		node->h = h;
		node->f = node->g + h;
		node->conn = parentNode;
	}

	return node;
}
