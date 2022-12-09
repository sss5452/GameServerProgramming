#define _CRT_SECURE_NO_WARNINGS
#pragma once
#include <iostream>
#include <array>
#include <WS2tcpip.h>
#include <MSWSock.h>
#include <sqlext.h>  
#include <thread>
#include <vector>
#include <mutex>
#include <unordered_set>
#include <queue>
#include <chrono>
#include <map>

#include "Astar02.h"
//#include "protocol.h"

constexpr int RANGE = 8;
constexpr int NAME_LEN = 20;
#pragma comment(lib, "WS2_32.lib")
#pragma comment(lib, "MSWSock.lib")
using namespace std;

SQLHENV henv;
SQLHDBC hdbc;
SQLHSTMT hstmt = 0;
SQLRETURN retcode;
MAP g_map[W_HEIGHT][W_WIDTH];
constexpr char N_MAP = 0;
constexpr char B_MAP = 3;
map<int, vector<int>> party;


GRID_TYPE town_map[15][15]
{
   GRID_TYPE::eBLOCKED ,   GRID_TYPE::eBLOCKED ,   GRID_TYPE::eBLOCKED ,   GRID_TYPE::eBLOCKED ,   GRID_TYPE::eBLOCKED ,   GRID_TYPE::eBLOCKED ,  GRID_TYPE::eSAFTY ,  GRID_TYPE::eSAFTY ,  GRID_TYPE::eSAFTY ,   GRID_TYPE::eBLOCKED ,   GRID_TYPE::eBLOCKED ,   GRID_TYPE::eBLOCKED ,   GRID_TYPE::eBLOCKED ,   GRID_TYPE::eBLOCKED ,   GRID_TYPE::eBLOCKED ,
   GRID_TYPE::eBLOCKED ,   GRID_TYPE::eBLOCKED ,   GRID_TYPE::eBLOCKED ,   GRID_TYPE::eBLOCKED ,   GRID_TYPE::eBLOCKED ,   GRID_TYPE::eBLOCKED ,  GRID_TYPE::eSAFTY ,  GRID_TYPE::eSAFTY ,  GRID_TYPE::eSAFTY ,   GRID_TYPE::eBLOCKED ,   GRID_TYPE::eBLOCKED ,   GRID_TYPE::eBLOCKED ,   GRID_TYPE::eBLOCKED ,   GRID_TYPE::eBLOCKED ,   GRID_TYPE::eBLOCKED ,
   GRID_TYPE::eBLOCKED ,   GRID_TYPE::eBLOCKED ,   GRID_TYPE::eBLOCKED ,   GRID_TYPE::eBLOCKED ,   GRID_TYPE::eBLOCKED ,   GRID_TYPE::eBLOCKED ,  GRID_TYPE::eSAFTY ,  GRID_TYPE::eSAFTY ,  GRID_TYPE::eSAFTY ,   GRID_TYPE::eBLOCKED ,   GRID_TYPE::eBLOCKED ,   GRID_TYPE::eBLOCKED ,   GRID_TYPE::eBLOCKED ,   GRID_TYPE::eBLOCKED ,   GRID_TYPE::eBLOCKED ,
   GRID_TYPE::eBLOCKED ,   GRID_TYPE::eBLOCKED ,   GRID_TYPE::eBLOCKED ,   GRID_TYPE::eBLOCKED ,   GRID_TYPE::eBLOCKED ,   GRID_TYPE::eBLOCKED ,  GRID_TYPE::eSAFTY ,  GRID_TYPE::eSAFTY ,  GRID_TYPE::eSAFTY ,   GRID_TYPE::eBLOCKED ,   GRID_TYPE::eBLOCKED ,   GRID_TYPE::eBLOCKED ,   GRID_TYPE::eBLOCKED ,   GRID_TYPE::eBLOCKED ,   GRID_TYPE::eBLOCKED ,
   GRID_TYPE::eBLOCKED ,   GRID_TYPE::eBLOCKED ,   GRID_TYPE::eBLOCKED ,   GRID_TYPE::eBLOCKED ,   GRID_TYPE::eBLOCKED ,   GRID_TYPE::eBLOCKED ,  GRID_TYPE::eSAFTY ,  GRID_TYPE::eSAFTY ,  GRID_TYPE::eSAFTY ,   GRID_TYPE::eBLOCKED ,   GRID_TYPE::eBLOCKED ,   GRID_TYPE::eBLOCKED ,   GRID_TYPE::eBLOCKED ,   GRID_TYPE::eBLOCKED ,   GRID_TYPE::eBLOCKED ,
   GRID_TYPE::eBLOCKED ,   GRID_TYPE::eBLOCKED ,   GRID_TYPE::eBLOCKED ,   GRID_TYPE::eBLOCKED ,   GRID_TYPE::eBLOCKED ,   GRID_TYPE::eBLOCKED ,  GRID_TYPE::eSAFTY ,  GRID_TYPE::eSAFTY ,  GRID_TYPE::eSAFTY ,   GRID_TYPE::eBLOCKED ,   GRID_TYPE::eBLOCKED ,   GRID_TYPE::eBLOCKED ,   GRID_TYPE::eBLOCKED ,   GRID_TYPE::eBLOCKED ,   GRID_TYPE::eBLOCKED ,
  GRID_TYPE::eSAFTY ,  GRID_TYPE::eSAFTY ,  GRID_TYPE::eSAFTY ,  GRID_TYPE::eSAFTY ,  GRID_TYPE::eSAFTY ,  GRID_TYPE::eSAFTY ,  GRID_TYPE::eSAFTY ,  GRID_TYPE::eSAFTY ,  GRID_TYPE::eSAFTY ,  GRID_TYPE::eSAFTY ,  GRID_TYPE::eSAFTY ,  GRID_TYPE::eSAFTY ,  GRID_TYPE::eSAFTY ,  GRID_TYPE::eSAFTY ,  GRID_TYPE::eSAFTY ,
  GRID_TYPE::eSAFTY ,  GRID_TYPE::eSAFTY ,  GRID_TYPE::eSAFTY ,  GRID_TYPE::eSAFTY ,  GRID_TYPE::eSAFTY ,  GRID_TYPE::eSAFTY ,  GRID_TYPE::eSAFTY ,  GRID_TYPE::eSAFTY ,  GRID_TYPE::eSAFTY ,  GRID_TYPE::eSAFTY ,  GRID_TYPE::eSAFTY ,  GRID_TYPE::eSAFTY ,  GRID_TYPE::eSAFTY ,  GRID_TYPE::eSAFTY ,  GRID_TYPE::eSAFTY ,
  GRID_TYPE::eSAFTY ,  GRID_TYPE::eSAFTY ,  GRID_TYPE::eSAFTY ,  GRID_TYPE::eSAFTY ,  GRID_TYPE::eSAFTY ,  GRID_TYPE::eSAFTY ,  GRID_TYPE::eSAFTY ,  GRID_TYPE::eSAFTY ,  GRID_TYPE::eSAFTY ,  GRID_TYPE::eSAFTY ,  GRID_TYPE::eSAFTY ,  GRID_TYPE::eSAFTY ,  GRID_TYPE::eSAFTY ,  GRID_TYPE::eSAFTY ,  GRID_TYPE::eSAFTY ,
   GRID_TYPE::eBLOCKED ,   GRID_TYPE::eBLOCKED ,   GRID_TYPE::eBLOCKED ,   GRID_TYPE::eBLOCKED ,   GRID_TYPE::eBLOCKED ,   GRID_TYPE::eBLOCKED ,  GRID_TYPE::eSAFTY ,  GRID_TYPE::eSAFTY ,  GRID_TYPE::eSAFTY ,   GRID_TYPE::eBLOCKED ,   GRID_TYPE::eBLOCKED ,   GRID_TYPE::eBLOCKED ,   GRID_TYPE::eBLOCKED ,   GRID_TYPE::eBLOCKED ,   GRID_TYPE::eBLOCKED ,
   GRID_TYPE::eBLOCKED ,   GRID_TYPE::eBLOCKED ,   GRID_TYPE::eBLOCKED ,   GRID_TYPE::eBLOCKED ,   GRID_TYPE::eBLOCKED ,   GRID_TYPE::eBLOCKED ,  GRID_TYPE::eSAFTY ,  GRID_TYPE::eSAFTY ,  GRID_TYPE::eSAFTY ,   GRID_TYPE::eBLOCKED ,   GRID_TYPE::eBLOCKED ,   GRID_TYPE::eBLOCKED ,   GRID_TYPE::eBLOCKED ,   GRID_TYPE::eBLOCKED ,   GRID_TYPE::eBLOCKED ,
   GRID_TYPE::eBLOCKED ,   GRID_TYPE::eBLOCKED ,   GRID_TYPE::eBLOCKED ,   GRID_TYPE::eBLOCKED ,   GRID_TYPE::eBLOCKED ,   GRID_TYPE::eBLOCKED ,  GRID_TYPE::eSAFTY ,  GRID_TYPE::eSAFTY ,  GRID_TYPE::eSAFTY ,   GRID_TYPE::eBLOCKED ,   GRID_TYPE::eBLOCKED ,   GRID_TYPE::eBLOCKED ,   GRID_TYPE::eBLOCKED ,   GRID_TYPE::eBLOCKED ,   GRID_TYPE::eBLOCKED ,
   GRID_TYPE::eBLOCKED ,   GRID_TYPE::eBLOCKED ,   GRID_TYPE::eBLOCKED ,   GRID_TYPE::eBLOCKED ,   GRID_TYPE::eBLOCKED ,   GRID_TYPE::eBLOCKED ,  GRID_TYPE::eSAFTY ,  GRID_TYPE::eSAFTY ,  GRID_TYPE::eSAFTY ,   GRID_TYPE::eBLOCKED ,   GRID_TYPE::eBLOCKED ,   GRID_TYPE::eBLOCKED ,   GRID_TYPE::eBLOCKED ,   GRID_TYPE::eBLOCKED ,   GRID_TYPE::eBLOCKED ,
   GRID_TYPE::eBLOCKED ,   GRID_TYPE::eBLOCKED ,   GRID_TYPE::eBLOCKED ,   GRID_TYPE::eBLOCKED ,   GRID_TYPE::eBLOCKED ,   GRID_TYPE::eBLOCKED ,  GRID_TYPE::eSAFTY ,  GRID_TYPE::eSAFTY ,  GRID_TYPE::eSAFTY ,   GRID_TYPE::eBLOCKED ,   GRID_TYPE::eBLOCKED ,   GRID_TYPE::eBLOCKED ,   GRID_TYPE::eBLOCKED ,   GRID_TYPE::eBLOCKED ,   GRID_TYPE::eBLOCKED ,
   GRID_TYPE::eBLOCKED ,   GRID_TYPE::eBLOCKED ,   GRID_TYPE::eBLOCKED ,   GRID_TYPE::eBLOCKED ,   GRID_TYPE::eBLOCKED ,   GRID_TYPE::eBLOCKED ,  GRID_TYPE::eSAFTY ,  GRID_TYPE::eSAFTY ,  GRID_TYPE::eSAFTY ,   GRID_TYPE::eBLOCKED ,   GRID_TYPE::eBLOCKED ,   GRID_TYPE::eBLOCKED ,   GRID_TYPE::eBLOCKED ,   GRID_TYPE::eBLOCKED ,   GRID_TYPE::eBLOCKED
};

enum COMP_TYPE { OP_ACCEPT, OP_RECV, OP_SEND, EV_MOVE, EV_HEAL, EV_ATTACK, OP_RANDOM_MOVE, OP_PLAYER_HEAL, OP_RESPAWN };

//enum EVENT_TYPE { OP_ACCEPT, OP_RECV, OP_SEND, EV_MOVE, EV_HEAL, EV_ATTACK, OP_RANDOM_MOVE, OP_PLAYER_HEAL};
struct TIMER_EVENT {
	int object_id;
	COMP_TYPE ev;
	chrono::system_clock::time_point act_time;
	int target_id;

	constexpr bool operator < (const TIMER_EVENT& _Left) const
	{
		return (act_time > _Left.act_time);
	}

};

priority_queue<TIMER_EVENT> timer_queue;
mutex timer_lock;

//enum COMP_TYPE { OP_ACCEPT, OP_RECV, OP_SEND , EV_MOVE, EV_HEAL, EV_ATTACK, OP_RANDOM_MOVE, OP_PLAYER_HEAL};
class OVER_EXP {
public:
	WSAOVERLAPPED _over;
	WSABUF _wsabuf;
	char _send_buf[BUF_SIZE];
	COMP_TYPE _comp_type;
	OVER_EXP()
	{
		_wsabuf.len = BUF_SIZE;
		_wsabuf.buf = _send_buf;
		_comp_type = OP_RECV;
		ZeroMemory(&_over, sizeof(_over));
	}
	OVER_EXP(char* packet)
	{
		_wsabuf.len = packet[0];
		_wsabuf.buf = _send_buf;
		ZeroMemory(&_over, sizeof(_over));
		_comp_type = OP_SEND;
		memcpy(_send_buf, packet, packet[0]);
	}
};

enum SESSION_STATE { ST_FREE, ST_ACCEPTED, ST_INGAME};
class SESSION {
	OVER_EXP _recv_over;

public:
	mutex	_sl;
	SESSION_STATE _s_state;
	int _id;
	SOCKET _socket;
	short	x, y;
	char	_name[NAME_SIZE];
	int level = 1;
	int hp = 100;
	int exp = 0;
	int quset_item;
	int heal_item;
	int skill_level;
	bool isSleep;


	unordered_set <int> view_list;
	mutex	vl;
	int target = -1;
	bool istarget;
	vector<tuple<int, int>> AstarPath;
	CAstar2 AstarHelper;
	int currentPath;

	int partyId = -1;
	short m_type;
	//int m_map[200][200];
	chrono::system_clock::time_point next_move_time;
	int		_prev_remain;
public:
	void damaged(int attackdamage) {
		hp -= attackdamage;
	}
	SESSION()
	{
		_id = -1;
		_socket = 0;
		x = rand() % W_WIDTH;
		y = rand() % W_HEIGHT;
		_name[0] = 0;
		_s_state = ST_FREE;
		_prev_remain = 0;
		next_move_time = chrono::system_clock::now() + chrono::seconds(1);
	}

	~SESSION() {}

	void do_recv()
	{
		DWORD recv_flag = 0;
		memset(&_recv_over._over, 0, sizeof(_recv_over._over));
		_recv_over._wsabuf.len = BUF_SIZE - _prev_remain;
		_recv_over._wsabuf.buf = _recv_over._send_buf + _prev_remain;
		WSARecv(_socket, &_recv_over._wsabuf, 1, 0, &recv_flag,
			&_recv_over._over, 0);
	}

	void do_send(void* packet)
	{
		OVER_EXP* sdata = new OVER_EXP{ reinterpret_cast<char*>(packet) };
		WSASend(_socket, &sdata->_wsabuf, 1, 0, 0, &sdata->_over, 0);
	}
	void send_login_ok_packet()
	{
		SC_LOGIN_OK_PACKET p;
		p.id = _id;
		p.size = sizeof(SC_LOGIN_OK_PACKET);
		p.type = SC_LOGIN_OK;
		p.x = x;
		p.y = y;
		p.hp = hp;
		p.exp = exp;
		p.level = level;
		p.h_item = heal_item;
		//strcpy_s(p.name, _name);

		do_send(&p);
	}
	void send_login_fail_packet()
	{
		SC_LOGIN_FAIL_PACKET p;
		//p.id = _id;
		p.size = sizeof(SC_LOGIN_FAIL_PACKET);
		p.type = SC_LOGIN_FAIL;

		do_send(&p);
	}

	void send_stat_change_packet(int c_id);
	void send_move_packet(int c_id, int client_time);
	void send_add_object(int c_id);
	void send_remove_object(int c_id);
	void send_chat_packet(int c_id , char c_mess[] , char chat_type);
};

array<SESSION, MAX_USER + NUM_NPC> clients;
HANDLE g_h_iocp;
SOCKET g_s_socket;

void add_timer(int obj_id, int act_time, COMP_TYPE e_type)
{
	timer_lock.lock();

	using namespace chrono;
	TIMER_EVENT ev;
	ev.act_time = system_clock::now() + milliseconds(act_time);
	ev.object_id = obj_id;
	ev.ev = e_type;
	//ev.target_id = target_id;
	timer_queue.push(ev);
	timer_lock.unlock();


}

int distance(int a, int b)
{
	return abs(clients[a].x - clients[b].x) + abs(clients[a].y - clients[b].y);
}

int monsterattackdistance(int a, int b)
{
	return abs(clients[a].x - clients[b].x) + abs(clients[a].y - clients[b].y);
}

void SESSION::send_move_packet(int c_id, int client_time)
{
	if (c_id < 0) {
		cout << "ROOR2";
	}
	SC_MOVE_OBJECT_PACKET p;
	p.id = c_id;
	p.size = sizeof(SC_MOVE_OBJECT_PACKET);
	p.type = SC_MOVE_OBJECT;
	p.x = clients[c_id].x;
	p.y = clients[c_id].y;
	p.client_time = client_time;
	do_send(&p);
}

void SESSION::send_add_object(int c_id)
{
	if (c_id < 0) {
		cout << "ROOR";
	}
	SC_ADD_OBJECT_PACKET p;
	p.id = c_id;
	p.size = sizeof(SC_ADD_OBJECT_PACKET);
	p.type = SC_ADD_OBJECT;
	p.x = clients[c_id].x;
	p.y = clients[c_id].y;
	p.hp = clients[c_id].hp;
	p.level = clients[c_id].level;
	p.race = clients[c_id].m_type;
	strcpy_s(p.name, clients[c_id]._name);
	do_send(&p);
}

void SESSION::send_remove_object(int c_id)
{
	SC_REMOVE_OBJECT_PACKET p;
	p.id = c_id;
	p.size = sizeof(SC_REMOVE_OBJECT_PACKET);
	p.type = SC_REMOVE_OBJECT;
	do_send(&p);
}

void SESSION::send_stat_change_packet(int c_id)
{
	SC_STAT_CHANGE_PACKET p;
	p.size = sizeof(p);
	p.type = SC_STAT_CHANGE;
	p.id = clients[c_id]._id;
	p.hp = clients[c_id].hp;
	p.level = clients[c_id].level;
	p.exp = clients[c_id].exp;
	p.heal_item = clients[c_id].heal_item;
	do_send(&p);

}

void SESSION::send_chat_packet(int c_id, char c_mess[] , char chat_type)
{
	SC_CHAT_PACKET packet;
	packet.size = sizeof(SC_CHAT_PACKET);
	packet.type = SC_CHAT;
	packet.id = c_id;
	packet.chat_type = chat_type;
	strcpy(packet.mess, c_mess);
	do_send(&packet);
//	cout << "Chat send" << endl;

}
void move_npc(int n_id);
void disconnect(int c_id);
int get_new_client_id()
{
	for (int i = 0; i < MAX_USER; ++i) {
		clients[i]._sl.lock();
		if (clients[i]._s_state == ST_FREE) {
			clients[i]._s_state = ST_ACCEPTED;
			clients[i]._sl.unlock();
			return i;
		}
		clients[i]._sl.unlock();
	}
	return -1;
}


SQLWCHAR szUser_LoginID[NAME_LEN];
SQLINTEGER  szUser_x, szUser_y;
SQLLEN cbLoginID = 0;
SQLLEN cbName = 0, cbID = 0, cbX, cbY = 0, cbHP, cbEXP, cbLEVEL;
SQLINTEGER  szUser_lv, szUser_exp;
SQLINTEGER  szUser_h_item, szUser_q_item;
SQLINTEGER  szUser_hp, szUser_skill_lv;
SQLLEN cb0 = 0,cb1= 0;
SQLLEN cb2 =0, cb4 = 0;
SQLLEN cb3= 0, cb5 = 0;

bool CheckAdminLogin(CS_LOGIN_PACKET packet, int client_id)
{
	
	string temp(packet.name);
	if (temp.find("DUMMY") != string::npos) {
		strcpy_s(clients[client_id]._name, packet.name);
		clients[client_id].x = rand() % 2000;
		clients[client_id].y = rand() % 2000;
		return true;
	}
	wstring wtemp;
	wtemp.assign(temp.begin(), temp.end());

	wstring order = L"EXECUTE sp_accept_login ";
	order += wtemp;

	retcode = SQLAllocHandle(SQL_HANDLE_STMT, hdbc, &hstmt);
	retcode = SQLExecDirect(hstmt, (SQLWCHAR*)order.c_str(), SQL_NTS);
	if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO) {
		retcode = SQLBindCol(hstmt, 1, SQL_C_WCHAR, szUser_LoginID, NAME_LEN, &cbLoginID);
		retcode = SQLBindCol(hstmt, 2, SQL_C_LONG, &szUser_x, 100, &cbX);
		retcode = SQLBindCol(hstmt, 3, SQL_C_LONG, &szUser_y, 100, &cbY);
		retcode = SQLBindCol(hstmt, 4, SQL_C_LONG, &szUser_lv, 100, &cb0);
		retcode = SQLBindCol(hstmt, 5, SQL_C_LONG, &szUser_exp, 100, &cb1);
		retcode = SQLBindCol(hstmt, 6, SQL_C_LONG, &szUser_q_item, 100, &cb2);
		retcode = SQLBindCol(hstmt, 7, SQL_C_LONG, &szUser_h_item, 100, &cb3);
		retcode = SQLBindCol(hstmt, 8, SQL_C_LONG, &szUser_skill_lv, 100, &cb4);
		retcode = SQLBindCol(hstmt, 9, SQL_C_LONG, &szUser_hp, 100, &cb5);

		for (int i = 0; ; i++) {
			retcode = SQLFetch(hstmt);
			if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO) {
				if (retcode == SQL_ERROR || retcode == SQL_SUCCESS_WITH_INFO) {
					//HandleDiagnosticRecord(hstmt, SQL_HANDLE_STMT, retcode);
				}
				//검색 완료( 존재 )
				if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO)
				{
					printf("ID: %S  ", szUser_LoginID);
					printf("X: %d  ", szUser_x);
					printf("Y: %d  ", szUser_y);
					printf("lv: %d  ", szUser_lv);
					printf("exp: %d  ", szUser_exp);
					printf("q_item: %d  ", szUser_q_item);
					printf("h_item: %d  ", szUser_h_item);
					printf("skill_lv: %d  ", szUser_skill_lv);
					printf("hp: %d  ", szUser_hp);

					printf("로그인 성공.\n");
					clients[client_id].x = szUser_x;
					clients[client_id].y = szUser_y;
					clients[client_id].level = szUser_lv;
					clients[client_id].exp = szUser_exp;
					clients[client_id].quset_item = szUser_q_item;
					clients[client_id].heal_item = szUser_h_item;
					clients[client_id].skill_level = szUser_skill_lv;
					clients[client_id].hp = szUser_hp;

					strcpy_s(clients[client_id]._name, packet.name);

					return true;
				}
				// 검색 실패 ( 없음 )
				else {
					//printf("%S 로그인 정보가 잘못 되었습니다.\n", admin_id);
					return false;
				}
			}
		}
	}
	else {
		std::cout << "DB Order Error\n";
		return false;

	}
}

void SaveClientPos(int client_id)
{
	string temp(clients[client_id]._name);
	wstring wtemp;
	wtemp.assign(temp.begin(), temp.end());

	wstring order = L"EXECUTE sp_save_userinfo ";
	order += wtemp;
	order += L", ";
	order += to_wstring(clients[client_id].x);
	order += L", ";
	order += to_wstring(clients[client_id].y);
	order += L", ";
	order += to_wstring(clients[client_id].level);
	order += L", ";
	order += to_wstring(clients[client_id].exp);
	order += L", ";
	order += to_wstring(clients[client_id].skill_level);
	order += L", ";
	order += to_wstring(clients[client_id].heal_item);
	order += L", ";
	order += to_wstring(clients[client_id].quset_item);
	order += L", ";
	order += to_wstring(clients[client_id].hp);


	retcode = SQLAllocHandle(SQL_HANDLE_STMT, hdbc, &hstmt);
	retcode = SQLExecDirect(hstmt, (SQLWCHAR*)order.c_str(), SQL_NTS);
	if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO) {
		std::cout << "POS 저장 완료" << endl;
	}
	else {
		std::cout << "DB Order Error\n";
	}
}

void setAstar(int npc_id, int targetid)
{
	ASNode* startNode = new ASNode(166, 233, 'S', GRID_TYPE::eSTART);
	vector < tuple<int, int>> a;
	//clients[npc_id].AstarHelper.PlayerPos(233, 166, 227, 165);
	clients[npc_id].AstarHelper.openList.push_back(startNode);
	clients[npc_id].AstarHelper.FindPath();
	cout << npc_id << "PATHFINDEND" << endl;

}


void do_attack(int user_id)
{
	clients[user_id].vl.lock();
	auto vl = clients[user_id].view_list;
	clients[user_id].vl.unlock();
	for (auto npc : vl)
	{
		if (clients[npc]._id < MAX_USER)
		{
			//char mess[100];
			//sprintf_s(mess, "AttacDamge - %d" , clients[user_id].level *10);
			//clients[npc].send_chat_packet(user_id,mess);
		}
		else {
			if ((clients[npc].x == clients[user_id].x && clients[npc].y == clients[user_id].y - 1) ||
				(clients[npc].x == clients[user_id].x && clients[npc].y == clients[user_id].y + 1) ||
				(clients[npc].x == clients[user_id].x - 1 && clients[npc].y == clients[user_id].y) ||
				(clients[npc].x == clients[user_id].x + 1 && clients[npc].y == clients[user_id].y))
			{
				clients[npc]._sl.lock();
				clients[npc].damaged(clients[user_id].level * 2);
				clients[npc].target = user_id;
				if (clients[npc].m_type == MONSTER_TYPE_PEACE) {
					clients[npc].isSleep = false;
					add_timer(npc, 500, OP_RANDOM_MOVE);
				}
				clients[npc]._sl.unlock();

				if (clients[npc].hp <= 0) {

					clients[npc].target = -1;
					clients[npc].isSleep= true;

					clients[npc]._sl.lock();
					clients[npc]._s_state = ST_FREE;

					while (true)
					{
						int x = rand() % W_WIDTH;
						int y = rand() % W_HEIGHT;

						if (g_map[y][x].type == eBLANK)
						{
							clients[npc].x = x;
							clients[npc].y = y;
							break;
						}
					}
					add_timer(npc, 3000, OP_RESPAWN);
					clients[npc]._sl.unlock();

					for (int i = 0; i < MAX_USER; ++i)
					{
						
						clients[i]._sl.lock();
						if (clients[i]._s_state != ST_FREE) {
							
							clients[i].send_remove_object(npc);

						}
						clients[i]._sl.unlock();

					}


					int exp;
					if (clients[user_id].partyId != -1) {
						exp = 5;
						clients[user_id].exp += exp;
						clients[clients[user_id].partyId].exp += exp;
					}
					else {
						exp = 10;
						clients[user_id].exp += exp;
					}
					if (clients[user_id].exp >= 100) {
						clients[user_id].level++;
						clients[user_id].exp = 0;
						char mess[50];
						sprintf_s(mess, "Player Level Up %d -> %d" , clients[user_id].level -1, clients[user_id].level);
						clients[user_id].send_chat_packet(user_id, mess, CHAT_TYPE_SYSTEM);
					}
					else {
						char mess[50];
						sprintf_s(mess, "Player Get %d EXP", exp);
						clients[user_id].send_chat_packet(user_id, mess, CHAT_TYPE_SYSTEM);
						if (clients[user_id].partyId != -1) {
							char mess[50];
							sprintf_s(mess, "Player Get %d EXP", exp);
							clients[clients[user_id].partyId].send_chat_packet(clients[user_id].partyId, mess, CHAT_TYPE_SYSTEM);
						}
					}
					if (rand() % 4 == 0) {
						clients[user_id].heal_item += 2;
						char mess[50];
						sprintf_s(mess, "Player Get Heal Item!");
						clients[user_id].send_chat_packet(user_id, mess, CHAT_TYPE_SYSTEM);
					}
					clients[user_id].send_stat_change_packet(user_id);

					for (int i = 0; i < MAX_USER; ++i)
					{
						if (clients[i]._s_state == ST_INGAME) {
							clients[i].send_stat_change_packet(user_id);
						}
					}
				}
				else {
					char mess[50];
					sprintf_s(mess, "Player %d Damaged to Monster by Basic Attack", clients[user_id].level * 2);
					clients[user_id].send_chat_packet(user_id, mess, CHAT_TYPE_SYSTEM);
				}


				for (int i = 0; i < MAX_USER; ++i)
				{
					if (clients[i]._s_state == ST_INGAME) {
						clients[i]._sl.lock();
						clients[i].send_stat_change_packet(npc);
						clients[i]._sl.unlock();

						//send_chat_packet(i, user_id, mess, 1);
					}
					else {
						//clients[i].send_remove_object(npc);
					}

				}


				//clients[user_id].send_stat_change_packet(user_id);


				//clients[npc].hp -= 2 * clients[user_id].level;

				//if (g_clients[npc].target == nullptr)
				//{
				//	g_clients[npc].target = &g_clients[user_id];
				//	if (g_clients[npc].npcMoveType != NPC_RANDOM_MOVE)
				//		random_move_npc(npc);
				//}

				//if (g_clients[npc].hp < 0) g_clients[npc].hp = 0;
				//isNPCDie(user_id, npc);

				//char mess[100];
				//sprintf_s(mess, "%s -> attack -> %s (-%d).", g_clients[user_id].m_name, g_clients[npc].m_name, g_clients[user_id].level * 2);


			}
		}
	}
}



void process_packet(int c_id, char* packet)
{
	switch (packet[1]) {
	case CS_LOGIN: {
		CS_LOGIN_PACKET* p = reinterpret_cast<CS_LOGIN_PACKET*>(packet);
		if (CheckAdminLogin(*p, c_id)){
			clients[c_id]._sl.lock();
			if (clients[c_id]._s_state == ST_FREE) {
				clients[c_id]._sl.unlock();
				break;
			}

			if (clients[c_id]._s_state == ST_INGAME) {
				clients[c_id]._sl.unlock();
				disconnect(c_id);
				break;
			}

			//strcpy_s(clients[c_id]._name, p->name);
			clients[c_id]._s_state = ST_INGAME;
			clients[c_id].send_login_ok_packet();
			clients[c_id]._sl.unlock();
			if (clients[c_id].hp < 100)
				add_timer(c_id, 5000, OP_PLAYER_HEAL);
			for (int i = 0; i < MAX_USER; ++i) {
				auto& pl = clients[i];
				if (pl._id == c_id) continue;
				pl._sl.lock();
				if (ST_INGAME != pl._s_state) {
					pl._sl.unlock();
					continue;
				}

				if (RANGE >= distance(c_id, pl._id)) {
					pl.vl.lock();
					pl.view_list.insert(c_id);
					pl.vl.unlock();
					pl.send_add_object(c_id);
				}
				
				pl._sl.unlock();
			}
			for (auto& pl : clients) {
				if (pl._id == c_id) continue;
				lock_guard<mutex> aa{ pl._sl };
				if (ST_INGAME != pl._s_state) continue;

				if (RANGE >= distance(pl._id, c_id)) {
					clients[c_id].vl.lock();
					clients[c_id].view_list.insert(pl._id);
					clients[c_id].vl.unlock();
					clients[c_id].send_add_object(pl._id);
				}
			}
		}
		else {
			clients[c_id].send_login_fail_packet();

		}
		break;
	}
	case CS_ATTACK:
	{
		CS_ATTACK_PACKET* p = reinterpret_cast<CS_ATTACK_PACKET*>(packet);
		do_attack(c_id);
		
		break;
	}
	case CS_ITEM:
	{
		CS_ITEM_PACKET* p = reinterpret_cast<CS_ITEM_PACKET*>(packet);
		char mess[50];
		if (clients[c_id].hp >= 100) {
			sprintf_s(mess, "Player hp is already full");
			clients[c_id].send_chat_packet(c_id, mess, CHAT_TYPE_SYSTEM);
		}
		else {
			clients[c_id].hp += 30;
			if (clients[c_id].hp >= 100) clients[c_id].hp = 100;
			sprintf_s(mess, "Player use potion!! Remain potion - %d ", --clients[c_id].heal_item);
			clients[c_id].send_chat_packet(c_id, mess, CHAT_TYPE_SYSTEM);
			
			clients[c_id].send_stat_change_packet(c_id);
			clients[clients[c_id].partyId].send_stat_change_packet(c_id);

			clients[c_id].vl.lock();
			auto vl = clients[c_id].view_list;
			clients[c_id].vl.unlock();
			for (auto p : vl)
			{
				clients[p].send_stat_change_packet(c_id);
			}
		}
		break;
	}
	case CS_CHAT:
	{
		CS_CHAT_PACKET* p = reinterpret_cast<CS_CHAT_PACKET*>(packet);
		if (p->chat_type == CHAT_TYPE_REQUEST_PARTY) {
			cout << "CHAT pr recv" << endl;

			char temp[50];
			sprintf_s(temp, "Party Request by - %s \n		Press Y/N " , clients[c_id]._name);
			clients[p->target_id]._sl.lock();
			//clients[c_id].send_stat_change_packet(c_id);
			clients[p->target_id].send_chat_packet(c_id, temp, CHAT_TYPE_REQUEST_PARTY);
			clients[p->target_id]._sl.unlock();

		}
		else if (p->chat_type == CHAT_TYPE_PARTY_YES) {
			clients[c_id].partyId = clients[p->target_id]._id;
			clients[p->target_id].partyId = clients[c_id]._id;
			char temp[50];
			sprintf_s(temp, "new PartyPlayer - %s ", clients[c_id]._name);
			clients[c_id].send_chat_packet(p->target_id, temp, CHAT_TYPE_PARTY_YES);
			sprintf_s(temp, "new PartyPlayer - %s ", clients[p->target_id]._name);
			clients[p->target_id].send_chat_packet(c_id, temp, CHAT_TYPE_PARTY_YES);

		}
		break;
	}
	case CS_MOVE: {
		
		CS_MOVE_PACKET* p = reinterpret_cast<CS_MOVE_PACKET*>(packet);
		short x = clients[c_id].x;
		short y = clients[c_id].y;	
		clients[c_id].vl.lock();
		auto old_vl = clients[c_id].view_list;
		clients[c_id].vl.unlock();

		switch (p->direction) { 
		case 0: if (y > 0  && g_map[y-1][x].type != eBLOCKED) y--; break;
		case 1: if (y < W_HEIGHT - 1 && g_map[y+1][x].type != eBLOCKED) y++; break;
		case 2: if (x > 0 && g_map[y][x-1].type != eBLOCKED) x--; break;
		case 3: if (x < W_WIDTH - 1 && g_map[y][x+1].type != eBLOCKED) x++; break;
		}
		clients[c_id].x = x;
		clients[c_id].y = y;

		unordered_set <int> new_vl;
		for (auto& obj : clients) {
			if (obj._id == c_id) continue;
			if (obj._s_state != ST_INGAME) continue;
			if (distance(c_id, obj._id) > RANGE) {

				continue;
			}
			new_vl.insert(obj._id);
			if (obj._id > MAX_USER && (c_id ==0 || c_id == 1)) {
				if (obj.isSleep && obj.m_type == MONSTER_TYPE_AGRO) {
					obj.isSleep = false;
					obj.target = c_id;
					add_timer(obj._id, 1000, OP_RANDOM_MOVE);
				}
			}
		}

		clients[c_id].send_move_packet(c_id, p->client_time);
		for (auto obj : new_vl) {
			clients[c_id].vl.lock();
			if (0 == clients[c_id].view_list.count(obj)) {
				clients[c_id].view_list.insert(obj);
				clients[c_id].vl.unlock();
				clients[c_id].send_add_object(obj);						
			}
			else {
				clients[c_id].vl.unlock();
				clients[c_id].send_move_packet(obj, 0);
			}
			if (obj < MAX_USER) {
				clients[obj].vl.lock();
				if (0 == clients[obj].view_list.count(c_id)) {
					clients[obj].view_list.insert(c_id);
					clients[obj].vl.unlock();
					clients[obj].send_add_object(c_id);
				}
				else {
					clients[obj].vl.unlock();
					clients[obj].send_move_packet(c_id, 0);
				}
			}
		}

		for (auto obj : old_vl) {
			if (0 != new_vl.count(obj) ) continue;
				
			clients[c_id].vl.lock();
			if (0 != clients[c_id].view_list.count(obj)) {
				clients[c_id].view_list.erase(obj);
				clients[c_id].vl.unlock();
				clients[c_id].send_remove_object(obj);
				if (clients[obj].target == c_id) {
					clients[obj].target = -1;
				}
			}
			else {
				clients[c_id].vl.unlock();
			}
			if (obj < MAX_USER) {
				clients[obj].vl.lock();
				if (0 != clients[obj].view_list.count(c_id)) {
					clients[obj].view_list.erase(c_id);
					clients[obj].vl.unlock();
					clients[obj].send_remove_object(c_id);
				}
				else {
					clients[obj].vl.unlock();
				}
			}
		}



		break;
	}
	}
}

void disconnect(int c_id)
{
	cout << "Disconnect" << endl;
	SaveClientPos(c_id);

	clients[c_id]._sl.lock();
	if (clients[c_id]._s_state == ST_FREE) {
		clients[c_id]._sl.unlock();
		return;
	}
	closesocket(clients[c_id]._socket);
	clients[c_id]._s_state = ST_FREE;
	clients[c_id]._sl.unlock();

	for (auto& pl : clients) {
		if (pl._id == c_id) continue;
		if (pl._id >= MAX_USER) continue;
		pl._sl.lock();
		if (pl._s_state != ST_INGAME) {
			pl._sl.unlock();
			continue;
		}
		pl.vl.lock();
		if (0 != pl.view_list.count(c_id)) {
			pl.view_list.erase(c_id);
			pl.vl.unlock();
			SC_REMOVE_OBJECT_PACKET p;
			p.id = c_id;
			p.size = sizeof(p);
			p.type = SC_REMOVE_OBJECT;
			pl.do_send(&p);
		}
		else
			pl.vl.unlock();
		pl._sl.unlock();
	}
}


void do_worker()
{
	while (true) {
		DWORD num_bytes;
		ULONG_PTR key;
		WSAOVERLAPPED* over = nullptr;
		BOOL ret = GetQueuedCompletionStatus(g_h_iocp, &num_bytes, &key, &over, INFINITE);
		OVER_EXP* ex_over = reinterpret_cast<OVER_EXP*>(over);
		int client_id = static_cast<int>(key);
		if (FALSE == ret) {
			if (ex_over->_comp_type == OP_ACCEPT) cout << "Accept Error";
			else {
				cout << "GQCS Error on client[" << key << "]\n";
				disconnect(static_cast<int>(key));
				if (ex_over->_comp_type == OP_SEND) delete ex_over;
				continue;
			}
		}

		switch (ex_over->_comp_type) {
		case OP_ACCEPT: {
			SOCKET c_socket = reinterpret_cast<SOCKET>(ex_over->_wsabuf.buf);
			int client_id = get_new_client_id();
			if (client_id != -1) {
				clients[client_id].x = 0;
				clients[client_id].y = 0;
				clients[client_id]._id = client_id;
				clients[client_id]._name[0] = 0;
				clients[client_id]._prev_remain = 0;
				clients[client_id]._socket = c_socket;
				CreateIoCompletionPort(reinterpret_cast<HANDLE>(c_socket),
					g_h_iocp, client_id, 0);
				clients[client_id].do_recv();
				c_socket = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
			}
			else {
				cout << "Max user exceeded.\n";
			}
			ZeroMemory(&ex_over->_over, sizeof(ex_over->_over));
			ex_over->_wsabuf.buf = reinterpret_cast<CHAR*>(c_socket);
			int addr_size = sizeof(SOCKADDR_IN);
			AcceptEx(g_s_socket, c_socket, ex_over->_send_buf, 0, addr_size + 16, addr_size + 16, 0, &ex_over->_over);
			break;
		}
		case OP_RECV: {
			if (0 == num_bytes) disconnect(client_id);
			int remain_data = num_bytes + clients[key]._prev_remain;
			char* p = ex_over->_send_buf;
			while (remain_data > 0) {
				int packet_size = p[0];
				if (packet_size <= remain_data) {
					process_packet(static_cast<int>(key), p);
					p = p + packet_size;
					remain_data = remain_data - packet_size;
				}
				else break;
			}
			clients[key]._prev_remain = remain_data;
			if (remain_data > 0) {
				memcpy(ex_over->_send_buf, p, remain_data);
			}
			clients[key].do_recv();
			break;
		}
		case OP_SEND:{
			if (0 == num_bytes) disconnect(client_id);
			delete ex_over;
			break;
		}
		case OP_RANDOM_MOVE:{
			move_npc(client_id);
			break;
		}
		case OP_RESPAWN: {
			clients[client_id].hp = 100;
			clients[client_id]._s_state = ST_INGAME;
			break;
		}

		case OP_PLAYER_HEAL:{
			cout << key << clients[key].hp << endl;
			clients[key].hp += 10;
			if (clients[key].hp >= 100)
				clients[key].hp = 100;
			else if (clients[key].hp < 100)
				add_timer(key, 5000, OP_PLAYER_HEAL);
			clients[key].send_stat_change_packet(key);
			break;
		}
		}


	}
}

void move_npc(int npc_id)
{
	short x = clients[npc_id].x;
	short y = clients[npc_id].y;
	unordered_set<int> old_vl;
	for (int i = 0; i < MAX_USER; ++i) {
		if (clients[i]._s_state != ST_INGAME) {
			continue;
		}
		if (distance(npc_id, i) <= RANGE) old_vl.insert(i);
	}
	
	if (clients[npc_id].target != -1 && (distance(npc_id, clients[npc_id].target) <= 1))
	{
		int target = clients[npc_id].target;
		clients[target].hp -= 10;
		if (clients[target].hp <= 0) {
			clients[target].hp = 80;
			clients[target].exp = 0;
			clients[target].x = 506;
			clients[target].y = 506;
			clients[target].send_move_packet(target,0);
			clients[target].send_stat_change_packet(target);
			char mess[50];
			sprintf_s(mess, "Player Dead - Respawn");
			clients[target].send_chat_packet(target, mess, CHAT_TYPE_SYSTEM);

		}
		else {
			clients[target].send_stat_change_packet(target);
			char mess[50];
			sprintf_s(mess, "Damged by Monster - %s", clients[npc_id]._name);
			clients[target].send_chat_packet(npc_id, mess, CHAT_TYPE_SYSTEM);
		}
	}
	else if (clients[npc_id].target != -1) {
		int px = clients[clients[npc_id].target].x;
		int py = clients[clients[npc_id].target].y;

		if (g_map[py][px].type == eSAFTY)
		{
			clients[npc_id].target = -1;
		}
		else {
			ASNode* startNode = new ASNode(y, x, 'S', GRID_TYPE::eSTART);
			clients[npc_id].AstarHelper.SetPlayerPos(x, y, clients[clients[npc_id].target].x, clients[clients[npc_id].target].y);
			clients[npc_id].AstarHelper.openList.push_back(startNode);
			clients[npc_id].AstarHelper.FindPath();
			Pos pos = clients[npc_id].AstarHelper.Walk();
			x = pos.x;
			y = pos.y;
		}
		//if (distance(npc_id, clients[npc_id].target) >= RANGE) {
		//	cout << npc_id << "OUT of Range" << endl;
		//	clients[npc_id].isSleep;
		//	clients[npc_id].target = -1;

		//}
	}
	else {

		switch (rand() % 4) {
		case 0: if (y > 0 && g_map[y - 1][x].type == eBLANK) y--; break;
		case 1: if (y < W_HEIGHT - 1 && g_map[y + 1][x].type == eBLANK) y++; break;
		case 2: if (x > 0 && g_map[y][x - 1].type == eBLANK) x--; break;
		case 3: if (x < W_WIDTH - 1 && g_map[y][x + 1].type == eBLANK) x++; break;
		}

	}

	
	clients[npc_id].x = x;
	clients[npc_id].y = y;

	unordered_set<int> new_vl;
	for (int i = 0; i < MAX_USER; ++i) {
		if (clients[i]._s_state != ST_INGAME) continue;
		if (distance(npc_id, i) <= RANGE) {
			new_vl.insert(i);
				if (distance(npc_id, i) <= 1)
				{
					clients[i].hp -= 5;
					//for (int j = 0; j < MAX_USER; j++) {
					//	if (clients[j]._s_state != ST_INGAME) continue;
							clients[i].send_stat_change_packet(i);
					//}
					char mess[50];
					sprintf_s(mess, "Damged by Monster - %s", clients[npc_id]._name);
					clients[i].send_chat_packet(npc_id, mess, CHAT_TYPE_SYSTEM);
					//clients[i]._sl.unlock();

				}
		}
	}

	for (auto p_id : new_vl) {
			clients[p_id].vl.lock();
			if (0 == clients[p_id].view_list.count(npc_id)) {
				clients[p_id].view_list.insert(npc_id);
				clients[p_id].vl.unlock();
				clients[p_id].send_add_object(npc_id);
			}
			else {
				clients[p_id].vl.unlock();
				clients[p_id].send_move_packet(npc_id, 0);
			}
	}
	for (auto p_id : old_vl) {
		if (0 == new_vl.count(p_id)) {
			clients[p_id].vl.lock();
			if (clients[p_id].view_list.count(npc_id) == 1) {
				clients[p_id].view_list.erase(npc_id);
				clients[p_id].vl.unlock();
				clients[p_id].send_remove_object(npc_id);
			}
			else
				clients[p_id].vl.unlock();
		}
	}
	if(!clients[npc_id].isSleep)add_timer(npc_id, 1000, OP_RANDOM_MOVE);

}

void do_ai_ver_1()
{
	for (;;) {
		auto start_t = chrono::system_clock::now();
		for (int i = 0; i < NUM_NPC; ++i) {
			int npc_id = i + MAX_USER;
			if (start_t > clients[npc_id].next_move_time) {
				move_npc(npc_id);
				clients[npc_id].next_move_time = start_t + chrono::seconds(1);
			}
		}
	}
}

void do_ai_ver_heat_beat()
{
	cout << "!";
	for (;;) {
		auto start_t = chrono::system_clock::now();
		for (int i = 0; i < NUM_NPC; ++i) {
			int npc_id = i + MAX_USER;
			move_npc(npc_id);
		}
		auto end_t = chrono::system_clock::now();
		auto ai_t = end_t - start_t;
		cout << "AI time : " << chrono::duration_cast<chrono::milliseconds>(ai_t).count();
		cout << "ms\n";
		this_thread::sleep_until(start_t + chrono::seconds(1));
	}
}

void initialize_npc()
{
	for (int i = 0; i < NUM_NPC + MAX_USER; ++i) {
		clients[i]._id = i;
		clients[i]._s_state = ST_FREE;
	}
	for (int i = 0; i < NUM_NPC; ++i) {
		int npc_id = i + MAX_USER;
		clients[npc_id]._s_state = ST_INGAME;
		clients[npc_id].hp = 100;
		clients[npc_id].isSleep = true;
		sprintf_s(clients[npc_id]._name, "M-%d", npc_id);
		if (rand() % 2 == 0)clients[npc_id].m_type = MONSTER_TYPE_AGRO;
		else clients[npc_id].m_type = MONSTER_TYPE_PEACE;
		while (true)
		{
			int x = rand() % 2000;
			int y = rand() % 2000;

			if (g_map[y][x].type == eBLANK)
			{
				clients[npc_id].x = x;
				clients[npc_id].y = y;
				break;
			}
		}
		//add_timer(npc_id, 1000,OP_RANDOM_MOVE); //그냥 움직이는게 아니라 플레이어가 움직였을때 시야에 들어오면 깨워야 함
	}
	std::cout << "NPC Init" << std::endl;

}

void do_timer()
{
	while (true)
	{
		this_thread::sleep_for(1ms); //Sleep(1);
		while (true)
		{
			timer_lock.lock();
			if (timer_queue.empty() == true)
			{
				timer_lock.unlock();
				break;
			}
			if (timer_queue.top().act_time > chrono::system_clock::now())
			{
				timer_lock.unlock();
				break;
			}
			TIMER_EVENT event = timer_queue.top();
			timer_queue.pop();
			timer_lock.unlock();

			switch (event.ev)
			{
			case OP_PLAYER_HEAL:
			{	
				OVER_EXP* over = new OVER_EXP();
				over->_comp_type = event.ev;
				PostQueuedCompletionStatus(g_h_iocp, 1, event.object_id, &over->_over);
				break;
			}
			case OP_RANDOM_MOVE:
			{
				OVER_EXP* over = new OVER_EXP();
				over->_comp_type = event.ev;
				PostQueuedCompletionStatus(g_h_iocp, 1, event.object_id, &over->_over);
				break;
			}
			case OP_RESPAWN:
			{
				OVER_EXP* over = new OVER_EXP();
				over->_comp_type = event.ev;
				PostQueuedCompletionStatus(g_h_iocp, 1, event.object_id, &over->_over);
				break;
			}
			}
		}
	}
}

void InitDataBase() {

	SQLWCHAR szUser_Name[NAME_LEN];
	SQLINTEGER dUser_id, dUser_x, dUser_y, dUser_LEVEL, dUser_EXP, dUser_HP;

	setlocale(LC_ALL, "korean");

	SQLLEN cbName = 0, cbID = 0, cbX, cbY = 0, cbHP, cbEXP, cbLEVEL;

	// Allocate environment handle  
	retcode = SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &henv);

	// Set the ODBC version environment attribute  
	if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO) {
		retcode = SQLSetEnvAttr(henv, SQL_ATTR_ODBC_VERSION, (SQLPOINTER*)SQL_OV_ODBC3, 0);

		// Allocate connection handle  
		if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO) {
			retcode = SQLAllocHandle(SQL_HANDLE_DBC, henv, &hdbc);

			// Set login timeout to 5 seconds  
			if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO) {
				SQLSetConnectAttr(hdbc, SQL_LOGIN_TIMEOUT, (SQLPOINTER)5, 0);

				// Connect to data source  
				retcode = SQLConnect(hdbc, (SQLWCHAR*)L"termproject_db", SQL_NTS, (SQLWCHAR*)NULL, 0, NULL, 0);

				// Allocate statement handle  
				if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO) {
					retcode = SQLAllocHandle(SQL_HANDLE_STMT, hdbc, &hstmt);

					printf("ODBC Connect OK \n");
				}
			}
		}
	}

}

void InitMapData()
{
	char data;
	FILE* fp = fopen("mapdata_1.txt", "rb");

	int count = 0;
	while (fscanf(fp, "%c", &data) != EOF) {
		//printf("%c", data);
		switch (data)
		{
		case '0':
			g_map[count / 2000][count % 2000].type = eBLANK;
			count++;
			break;
		case '3':
			g_map[count / 2000][count % 2000].type = eBLOCKED;
			count++;
			break;
		default:
			break;
		}
	}
	for (int i = 0; i < 15; i++) {
		for (int j = 0; j < 15; j++) {
			g_map[500 + j][500 + i].type = town_map[j][i];
		}
	}

	

	std::cout << "Map Init" << std::endl;

}

int main()
{
	InitDataBase();
	InitMapData();
	initialize_npc();
	

	WSADATA WSAData;
	WSAStartup(MAKEWORD(2, 2), &WSAData);
	g_s_socket = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
	SOCKADDR_IN server_addr;
	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(PORT_NUM);
	server_addr.sin_addr.S_un.S_addr = INADDR_ANY;
	bind(g_s_socket, reinterpret_cast<sockaddr*>(&server_addr), sizeof(server_addr));
	listen(g_s_socket, SOMAXCONN);
	SOCKADDR_IN cl_addr;
	int addr_size = sizeof(cl_addr);
	int client_id = 0;

	g_h_iocp = CreateIoCompletionPort(INVALID_HANDLE_VALUE, 0, 0, 0);
	CreateIoCompletionPort(reinterpret_cast<HANDLE>(g_s_socket), g_h_iocp, 9999, 0);
	SOCKET c_socket = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
	OVER_EXP a_over;
	a_over._comp_type = OP_ACCEPT;
	a_over._wsabuf.buf = reinterpret_cast<CHAR *>(c_socket);
	AcceptEx(g_s_socket, c_socket, a_over._send_buf, 0, addr_size + 16, addr_size + 16, 0, &a_over._over);

	
	vector <thread> worker_threads;
	for (int i = 0; i < 6; ++i)
		worker_threads.emplace_back(do_worker);
	thread timer_thread{ do_timer };
	//thread ai_thread{ do_ai_ver_heat_beat };
	//ai_thread.join();
	for (auto& th : worker_threads)
		th.join();
	timer_thread.join();
	closesocket(g_s_socket);
	WSACleanup();
}


