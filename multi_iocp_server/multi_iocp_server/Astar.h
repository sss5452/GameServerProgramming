#pragma once
#include <list>

struct _NODE
{
	int ix;
	int iy;
	_NODE* pParent;

	double	F;	//G+H
	double	G;	//�����
	int		H;	//�������� �Ÿ�


	_NODE(int x, int y, _NODE* parent)
	{
		ix = x;
		iy = y;
		pParent = parent;
	}
};

class CAStar
{

public:
	int iendX;
	int iendY;
	int istartX;
	int istartY;
	_NODE* endNode;
public:
	std::list<_NODE*> openList;
	std::list<_NODE*> closeList;


public:
	CAStar();
	~CAStar();


	//����
	void setG(_NODE* _node);
	//�밢��
	void setG_dia(_NODE* _node);
	bool compareG(int** mapData, _NODE* _node, bool isDia, int dir);

	void setH(_NODE* _node);
	void setF(_NODE* _node);

	void setEndPos(int x, int y);
	void setStartPos(int x, int y);
	bool returnPos(int* x, int* y); //���ݽ����̸� true

	bool searchLoad(int** mapData, int _startX, int _startY, int _endX, int _endY);
	//void searchLoad(HWND hWnd);

	void pathDraw();
	//void pathDraw(HWND hWnd);

	void setEndNodeNULL();
	bool isEndNode();

};

