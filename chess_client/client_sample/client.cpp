#define _CRT_SECURE_NO_WARNINGS
#define SFML_STATIC 1
#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include <iostream>
#include <unordered_map>
#include <chrono>
#include <string>
#include <random>
#include <fstream>
#include <queue>

using namespace std;
using namespace chrono;

#ifdef _DEBUG
#pragma comment (lib, "lib/sfml-graphics-s-d.lib")
#pragma comment (lib, "lib/sfml-window-s-d.lib")
#pragma comment (lib, "lib/sfml-system-s-d.lib")
#pragma comment (lib, "lib/sfml-network-s-d.lib")
#else
#pragma comment (lib, "lib/sfml-graphics-s.lib")
#pragma comment (lib, "lib/sfml-window-s.lib")
#pragma comment (lib, "lib/sfml-system-s.lib")
#pragma comment (lib, "lib/sfml-network-s.lib")
#endif
#pragma comment (lib, "opengl32.lib")
#pragma comment (lib, "winmm.lib")
#pragma comment (lib, "ws2_32.lib")

#include "..\..\multi_iocp_server\multi_iocp_server\protocol.h"
sf::TcpSocket socket;

constexpr auto SCREEN_WIDTH = 20;
constexpr auto SCREEN_HEIGHT = 20;

constexpr auto TILE_WIDTH = 40;
constexpr auto WINDOW_WIDTH = TILE_WIDTH * SCREEN_WIDTH + 10;   // size of window
constexpr auto WINDOW_HEIGHT = TILE_WIDTH * SCREEN_WIDTH + 10;

int g_left_x;
int g_top_y;
int g_myid;
string playerName;

sf::RenderWindow* g_window;
sf::Font g_font;

char g_map[W_HEIGHT][W_WIDTH];
	
constexpr char N_MAP = 0;
constexpr char B_MAP = 3;
constexpr char T_BLANK = 4;
constexpr char T_ROCK = 5;
constexpr char T_HOUSE = 6;
constexpr char T_GRASS = 7;


char town_map[15][15]
{
'5','5','5','5','5','5','0','0','0','5','5','5','5','5','5',
'5','4','4','4','5','5','0','0','0','5','4','4','4','5','5',
'5','4','6','4','5','5','0','0','0','5','4','6','4','5','5',
'5','4','4','4','5','5','0','0','0','5','4','4','4','5','5',
'5','5','5','5','5','5','0','0','0','5','5','5','5','5','5',
'5','5','5','5','5','5','0','0','0','5','5','5','5','5','5',
'0','0','0','0','0','0','0','0','0','0','0','0','0','0','0',
'0','0','0','0','0','0','0','0','0','0','0','0','0','0','0',
'0','0','0','0','0','0','0','0','0','0','0','0','0','0','0',
'5','5','5','5','5','5','0','0','0','5','5','5','5','5','5',
'5','4','4','4','5','5','0','0','0','5','4','4','4','5','5',
'5','4','6','4','5','5','0','0','0','5','4','6','4','5','5',
'5','4','4','4','5','5','0','0','0','5','4','4','4','5','5',
'5','5','5','5','5','5','0','0','0','5','5','5','5','5','5',
'5','5','5','5','5','5','0','0','0','5','5','5','5','5','5'
};

void init_map()
{
	char data;
	FILE* fp = fopen("mapdata_1.txt", "rb");

	int count = 0;
	while (fscanf(fp, "%c", &data) != EOF) {
		//printf("%c", data);
		switch (data)
		{
		case '0':
			g_map[count / 2000][count % 2000] = N_MAP;
			count++;
			break;
		case '3':
			g_map[count / 2000][count % 2000] = B_MAP;
			count++;
			break;
		default:
			break;
		}
	}

	for (int i = 0; i < 15; i++) {
		for (int j = 0; j < 15; j++) {
			if(town_map[j][i] == '6')
			g_map[500 + j][500 + i] = T_HOUSE;
			if (town_map[j][i] == '4')
				g_map[500 + j][500 + i] = T_BLANK;
			if (town_map[j][i] == '5')
				g_map[500 + j][500 + i] = T_ROCK;
			if (town_map[j][i] == '0')
				g_map[500 + j][500 + i] = T_GRASS;
 		}
	}
}
sf::Texture* board;
sf::Texture* pieces;
sf::Texture* monster;
sf::Texture* mapTile;
sf::Texture* attack;
sf::Texture* healItem;
sf::Texture* monsterhit;
sf::Texture* townRockTile;
sf::Texture* townmapHouse;
sf::Texture* townGrassTile;
sf::Texture* otherPlayer;


struct worldChat {
	sf::Text chat;
	chrono::system_clock::time_point chat_end_time;
	void set_chat(const char str[])
	{
		chat.setFont(g_font);
		chat.setString(str);
		chat.setFillColor(sf::Color(0, 0, 0));
		chat.setCharacterSize(20);
		chat.setStyle(sf::Text::Bold);
		chat_end_time = chrono::system_clock::now() + chrono::seconds(5);
	}
	void draw(int x, int y)
	{
		chat.setPosition(sf::Vector2f(x, y));
		g_window->draw(chat);
	}

};

vector<worldChat> worldSystemChat;

class OBJECT {
private:
	bool m_showing;
	sf::Sprite m_sprite;
	sf::Text m_name;
	sf::Text m_chat;
	sf::Text m_levl_info;
	sf::Text m_system_chat;
	sf::Sprite m_attack;
	sf::Sprite m_healItem;
	sf::Text m_healItemText;
	sf::Text m_pos;
	sf::Sprite m_hit;
	chrono::system_clock::time_point m_mess_end_time;
	chrono::system_clock::time_point m_attack_end_time;
	chrono::system_clock::time_point m_system_end_time;



	//hp
	sf::Text fonthp;
	sf::RectangleShape maxhpbox;
	sf::RectangleShape curhpbox;
	//exp
	sf::RectangleShape maxexpbox;
	sf::RectangleShape curexpbox;
	//systembox
	sf::RectangleShape systembox;

	//
	sf::RectangleShape worldsystembox;
public:
	chrono::system_clock::time_point m_move_time;
	chrono::system_clock::time_point m_attack_time;
	chrono::system_clock::time_point m_hit_end_time;

	int m_x, m_y;
	int hp;
	int exp , level;
	bool isAttack;
	bool isPlayer;
	bool isMe;
	bool isParty;
	int global_x;
	int global_y;
	bool togglePartyMode = false;
	int partyId = -1;
	bool toggleRequset = false;
	int requsetId;
	int HealItem;
	bool skill_home;
	short m_type;
	OBJECT(sf::Texture& t, int x, int y, int x2, int y2, bool me,bool otherplayer) {
		m_showing = false;
		isMe = me;
		isPlayer = otherplayer;
		m_sprite.setTexture(t);
		m_sprite.setTextureRect(sf::IntRect(x, y, x2, y2));
		set_name("NONAME");
		m_mess_end_time = chrono::system_clock::now();
		m_attack.setTexture(*attack);
		m_attack.setTextureRect(sf::IntRect(0, 0, 125, 150));
		m_healItem.setTexture(*healItem);
		m_healItem.setTextureRect(sf::IntRect(0, 0, 32, 32));
		m_hit.setTexture(*monsterhit);
		m_hit.setTextureRect(sf::IntRect(0, 0, 32, 32));

		m_healItemText.setFont(g_font);
		m_healItemText.setFillColor(sf::Color(0, 0, 0));
		m_healItemText.setCharacterSize(30);
		m_healItemText.setStyle(sf::Text::Bold);

		m_pos.setFont(g_font);
		m_pos.setFillColor(sf::Color(0, 0, 0));
		m_pos.setCharacterSize(25);
		m_pos.setStyle(sf::Text::Bold);
		m_pos.setPosition(sf::Vector2f(10, 25));

	}
	OBJECT() {
		m_showing = false;
	}
	void show()
	{
		m_showing = true;
	}
	void hide()
	{
		m_showing = false;
	}
	void setParty(int i)
	{
		partyId = i;
	}

	void a_move(int x, int y) {
		m_sprite.setPosition((float)x, (float)y);
	}

	void a_draw() {
		g_window->draw(m_sprite);
	}

	void p_attack()
	{
		float rx = (m_x - g_left_x) * 40 -25 ;
		float ry = (m_y - g_top_y) * 40 - 40 ;
		m_attack.setPosition(rx, ry);
		m_attack_end_time = chrono::system_clock::now() + chrono::milliseconds(200);
	}

	void move(int x, int y) {
		char temp[15];
		sprintf_s(temp, "[X-%d,Y-%d]", x , y);
		m_pos.setString(temp);
		m_x = x;
		m_y = y;
	}
	void draw() {
		if (false == m_showing) return;
		float rx = (m_x - g_left_x) * 40 ;
		float ry = (m_y - g_top_y) * 40  ;
		global_x = rx;
		global_y = ry;
		m_sprite.setPosition(rx, ry);
		g_window->draw(m_sprite);

		if (m_mess_end_time < chrono::system_clock::now()) {
		
			m_name.setPosition(rx - 10, ry - 20);
			g_window->draw(m_name);
		}
		else {
			m_chat.setPosition(rx - 10, ry - 20);
			g_window->draw(m_chat);
		}

		if (m_attack_end_time > chrono::system_clock::now()) {
			g_window->draw(m_attack);
		}

		if (m_hit_end_time > chrono::system_clock::now()) {
			g_window->draw(m_hit);
		}
		if (!isMe) {

		//hp


			if (isPlayer == false) {
				maxhpbox.setPosition(sf::Vector2f(rx - 20, ry - 40));
				maxhpbox.setOutlineThickness(3.0f);
				maxhpbox.setOutlineColor(sf::Color::White);
				maxhpbox.setFillColor(sf::Color(0, 0, 0, 100));
				maxhpbox.setSize(sf::Vector2f(100, 10));
				g_window->draw(maxhpbox);
				curhpbox.setPosition(sf::Vector2f(rx - 20, ry - 40));
				curhpbox.setFillColor(sf::Color(255, 0, 0, 100));
				if (hp < 0) hp = 0;
				curhpbox.setSize(sf::Vector2f(hp, 10));

				g_window->draw(curhpbox);
			}
			else if(isParty) {
				maxhpbox.setPosition(sf::Vector2f(rx - 20, ry - 40));
				maxhpbox.setOutlineThickness(3.0f);
				maxhpbox.setOutlineColor(sf::Color::White);
				maxhpbox.setFillColor(sf::Color(0, 0, 0, 100));
				maxhpbox.setSize(sf::Vector2f(100, 10));
				g_window->draw(maxhpbox);
				curhpbox.setPosition(sf::Vector2f(rx - 20, ry - 40));
				curhpbox.setFillColor(sf::Color(255, 255, 0, 100));
				if (hp < 0) hp = 0;
				curhpbox.setSize(sf::Vector2f(hp, 10));

				g_window->draw(curhpbox);
			}

		}
		fonthp.setPosition(rx + 10, ry - 50);
		if (isMe) {
			g_window->draw(m_pos);
			maxhpbox.setPosition(sf::Vector2f(WINDOW_HEIGHT / 2 - 100, 30));
			maxhpbox.setOutlineThickness(3.0f);
			maxhpbox.setOutlineColor(sf::Color::White);
			maxhpbox.setFillColor(sf::Color(0, 0, 0, 100));
			maxhpbox.setSize(sf::Vector2f(300, 20));
			g_window->draw(maxhpbox);
			curhpbox.setPosition(sf::Vector2f(WINDOW_HEIGHT / 2 - 100, 30));
			curhpbox.setFillColor(sf::Color(255, 0, 0, 100));
			curhpbox.setSize(sf::Vector2f(hp*3, 20));
			g_window->draw(curhpbox);
			//level
			m_levl_info.setPosition(sf::Vector2f(WINDOW_HEIGHT / 2 - 300, WINDOW_WIDTH - 60));
			g_window->draw(m_levl_info);
			//exp
			maxexpbox.setPosition(sf::Vector2f(WINDOW_HEIGHT / 2 - 200, WINDOW_WIDTH - 50));
			maxexpbox.setOutlineThickness(3.0f);
			maxexpbox.setOutlineColor(sf::Color::White);
			maxexpbox.setFillColor(sf::Color(0, 0, 0, 100));
			maxexpbox.setSize(sf::Vector2f(500, 20));
			g_window->draw(maxexpbox);

			curexpbox.setPosition(sf::Vector2f(WINDOW_HEIGHT / 2 - 200, WINDOW_WIDTH - 50));
			curexpbox.setFillColor(sf::Color(10, 255, 255, 150));
			curexpbox.setSize(sf::Vector2f(exp * 5, 20));
			g_window->draw(curexpbox);

			m_healItem.setPosition(sf::Vector2f(WINDOW_HEIGHT - 130, 1));
			g_window->draw(m_healItem);
			char temp[10];
			m_healItemText.setPosition(sf::Vector2f(WINDOW_HEIGHT - 100, 1));
			sprintf_s(temp, "x%d", HealItem);
			m_healItemText.setString(temp);
			g_window->draw(m_healItemText);

			
			if (m_system_end_time > chrono::system_clock::now())
			{
				m_system_chat.setPosition(sf::Vector2f(WINDOW_HEIGHT / 2 - 200, 200));
				systembox.setPosition(sf::Vector2f(WINDOW_HEIGHT / 2 - 200, 200));
				systembox.setOutlineThickness(3.0f);
				systembox.setOutlineColor(sf::Color::Black);
				systembox.setFillColor(sf::Color(0, 0, 0, 50));
				systembox.setSize(sf::Vector2f(500, 100));
				g_window->draw(systembox);
				g_window->draw(m_system_chat);

			}

			//월드 시스템
			if (worldSystemChat.size() > 0)
			{
				worldsystembox.setPosition(sf::Vector2f(10, WINDOW_HEIGHT - 250));
				worldsystembox.setOutlineThickness(1.0f);
				worldsystembox.setOutlineColor(sf::Color::Black);
				worldsystembox.setFillColor(sf::Color(0, 0, 0, 50));
				worldsystembox.setSize(sf::Vector2f(600, 150));
				g_window->draw(worldsystembox);
				for (int i = 0; i < worldSystemChat.size(); ++i) {
					if (worldSystemChat[i].chat_end_time > chrono::system_clock::now())
						worldSystemChat[i].draw(10, WINDOW_HEIGHT - 150  - (i *20));
					else {
						worldSystemChat.erase(worldSystemChat.begin() + i);
					}
				}

			}


		}
	//	g_window->draw(fonthp);


	}
	void set_world_chat(const char srt[])
	{

	}
	void set_postext(int x, int y)
	{
		char temp[15];
		sprintf_s(temp, "[X-%d,Y-%d]", x, y);
		m_pos.setString(temp);
	}
	void set_hit()
	{
		m_hit.setPosition(global_x+ 5,global_y +5);
		m_hit_end_time = chrono::system_clock::now() + chrono::milliseconds(200);

	}
	void set_system_chat(const char str[])
	{
		m_system_chat.setFont(g_font);
		m_system_chat.setString(str);
		m_system_chat.setFillColor(sf::Color(0, 0, 0));
		m_system_chat.setCharacterSize(30);
		m_system_chat.setStyle(sf::Text::Bold);
		m_system_end_time = chrono::system_clock::now() + chrono::seconds(5);

	}
	void set_level(const char str[])
	{
		m_levl_info.setFont(g_font);
		m_levl_info.setString(str);
		m_levl_info.setFillColor(sf::Color(0, 0, 255));
		m_levl_info.setCharacterSize(30);
		m_levl_info.setStyle(sf::Text::Bold);
	}
	void set_name(const char str[]) {
		m_name.setFont(g_font);
		m_name.setString(str);
		m_name.setFillColor(sf::Color(255, 255, 0));
		m_name.setCharacterSize(20);
		m_name.setStyle(sf::Text::Bold);
	}
	void set_chat(const char str[]) {
		m_chat.setFont(g_font);
		m_chat.setString(str);
		m_chat.setFillColor(sf::Color(255, 255, 255));
		m_chat.setStyle(sf::Text::Bold);
		m_mess_end_time = chrono::system_clock::now() + chrono::seconds(3);
	}
};

OBJECT avatar;
unordered_map<int, OBJECT> players;
unordered_map<int, OBJECT> npcs;
//OBJECT players[MAX_USER];
// OBJECT npcs[NUM_NPC];

OBJECT nomal_tile;
OBJECT obstacle_tile;
OBJECT town_house;
OBJECT rock_tile;
OBJECT grass_tile;



void client_initialize()
{
	board = new sf::Texture;
	pieces = new sf::Texture;
	monster = new sf::Texture;
	mapTile = new sf::Texture;
	attack = new sf::Texture;
	healItem = new sf::Texture;
	monsterhit = new sf::Texture;
	townmapHouse = new sf::Texture;
	townRockTile = new sf::Texture;
	townGrassTile = new sf::Texture;
	otherPlayer = new sf::Texture;
	mapTile->loadFromFile("res/mapTile.bmp");
	board->loadFromFile("chessmap.bmp");
	otherPlayer->loadFromFile("res/george_other.png");
	pieces->loadFromFile("res/george.png");
	monster->loadFromFile("res/pacman.png");
	attack->loadFromFile("res/Classic_01.png");
	healItem->loadFromFile("res/heal_item32.png");
	monsterhit->loadFromFile("res/monster_hit.png");
	townmapHouse->loadFromFile("res/house.png");
	townRockTile->loadFromFile("res/rock.png");
	townGrassTile->loadFromFile("res/grass.png");

	if (false == g_font.loadFromFile("cour.ttf")) {
		cout << "Font Loading Error!\n";
		exit(-1);
	}
	nomal_tile = OBJECT{ *mapTile, 0, 0, TILE_WIDTH, TILE_WIDTH,false ,false};
	obstacle_tile = OBJECT{ *mapTile, 65, 0, TILE_WIDTH, TILE_WIDTH ,false,false };
	town_house = OBJECT{ *townmapHouse ,0,0,120,120,false,false };
	rock_tile = OBJECT{ *townRockTile ,0,0,40,40,false,false };
	grass_tile = OBJECT{ *townGrassTile ,0,0,40,40,false,false };

	avatar = OBJECT{ *pieces, 0, 0, 48, 48 ,true,true };
	avatar.move(4, 4);

	//for (auto& pl : players) {
	//	pl = OBJECT{ *pieces, 64, 0, 64, 64 };
	//}
	//for (auto& pl : npcs) {
	//	pl = OBJECT{ *pieces, 0, 0, 64, 64 };
	//}
}

void client_finish()
{
	delete board;
	delete pieces;
}

void ProcessPacket(char* ptr)
{
	static bool first_time = true;
	switch (ptr[1])
	{
	case SC_LOGIN_OK:
	{
		SC_LOGIN_OK_PACKET* packet = reinterpret_cast<SC_LOGIN_OK_PACKET*>(ptr);
		g_myid = packet->id;
		avatar.m_x = packet->x;
		avatar.m_y = packet->y;
		string a_name;
		a_name += playerName;
		avatar.set_name(a_name.c_str());
		avatar.hp = packet->hp;
		avatar.level = packet->level;
		avatar.exp = packet->exp;
		avatar.HealItem = packet->h_item;
		g_left_x = packet->x - 9;
		g_top_y = packet->y - 9;
		avatar.set_postext(packet->x, packet->y);
		char str[20];
		sprintf(str, "LV.%d", packet->level);
		avatar.set_level(str);

		//sprintf(str, "EXP ");
		//avatar.set_exp(str);

		//sprintf(str, "%d/100", packet->hp);
		//avatar.set_hp(str);
		avatar.show();
		break;
	}
	case SC_LOGIN_FAIL:
	{
		cout << "LOGIN_FAIL - 일치하는 아이디가 없습니다." << endl;
		
		break;
	}

	case SC_ADD_OBJECT:
	{
		SC_ADD_OBJECT_PACKET* my_packet = reinterpret_cast<SC_ADD_OBJECT_PACKET*>(ptr);
		int id = my_packet->id;
		//cout << my_packet->id << endl;
		if (id < MAX_USER) {
			if (0 != players.count(id)) {
				cout << "Player " << id << " already exists.\n";
				break;
			}
			players[id] = OBJECT{ *otherPlayer, 0, 0, 48, 48 ,false, true};
			players[id].move(my_packet->x, my_packet->y);
			players[id].hp = my_packet->hp;
			players[id].level = my_packet->level;
			if (id == avatar.partyId) players[id].isParty = true;
			//players[id].setParty();
			//players[id].exp = my_packet->exp;

			players[id].set_name(my_packet->name);
			players[id].show();
		}
		else {
			if (0 != npcs.count(id)) {
				cout << "NPC " << id << " already exists.\n";
				break;
			}
			npcs[id].m_type = my_packet->race;
			if(npcs[id].m_type == MONSTER_TYPE_PEACE)
				npcs[id] = OBJECT{ *monster, 64, 0, 32, 32 ,false,false};
			else if (npcs[id].m_type == MONSTER_TYPE_AGRO)
				npcs[id] = OBJECT{ *monster, 64, 64, 32, 32 ,false,false };
			npcs[id].move(my_packet->x, my_packet->y);
			npcs[id].set_name(my_packet->name);
			npcs[id].hp = my_packet->hp;
			cout << npcs[id].hp << endl;
			npcs[id].show();
		}
		break;
	}
	case SC_MOVE_OBJECT:
	{
		SC_MOVE_OBJECT_PACKET* my_packet = reinterpret_cast<SC_MOVE_OBJECT_PACKET*>(ptr);
		int other_id = my_packet->id;
		if (other_id == g_myid) {
			avatar.move(my_packet->x, my_packet->y);
			g_left_x = my_packet->x - 9;
			g_top_y = my_packet->y - 9;
		}
		else if (other_id < MAX_USER) {
			if (0 == players.count(other_id)) {
				cout << "Player " << other_id << " does not exists.\n";
				break;
			}
			players[other_id].move(my_packet->x, my_packet->y);
		}
		else {
			if (0 == npcs.count(other_id)) {
				cout << "NPC " << other_id << " does not exists.\n";
				break;
			}
			npcs[other_id].move(my_packet->x, my_packet->y);
		}
		break;
	}

	case SC_REMOVE_OBJECT:
	{
		SC_REMOVE_OBJECT_PACKET* my_packet = reinterpret_cast<SC_REMOVE_OBJECT_PACKET*>(ptr);
		int other_id = my_packet->id;
		if (other_id == g_myid) {
			avatar.hide();
		}
		else if (other_id < MAX_USER) {
			if (0 == players.count(other_id)) {
				cout << "Player " << other_id << " does not exists.\n";
				break;
			}
			players[other_id].hide();
			players.erase(other_id);
		}
		else {
			if (0 == npcs.count(other_id)) {
				cout << "NPC " << other_id << " does not exists.\n";
				break;
			}
			npcs[other_id].hide();
			npcs.erase(other_id);
		}
		break;
	}

	case SC_STAT_CHANGE:
	{
		SC_STAT_CHANGE_PACKET* my_packet = reinterpret_cast<SC_STAT_CHANGE_PACKET*>(ptr);
		int o_id = my_packet->id;
		if (o_id == g_myid) {
			avatar.hp = my_packet->hp;
			avatar.level = my_packet->level;
			avatar.exp = my_packet->exp;
			avatar.HealItem = my_packet->heal_item;

		}
		else if (o_id < MAX_USER) {
			if (0 == players.count(o_id)) {
				cout << "Player " << o_id << " does not exists.\n";
				break;
			}
			players[o_id].hp = my_packet->hp;
			players[o_id].level = my_packet->level;
			players[o_id].exp = my_packet->exp;


		}
		else {
			if (0 == npcs.count(o_id)) {
				cout << "NPC " << o_id << " does not exists.\n";
				break;
			}
			npcs[o_id].hp = my_packet->hp;
			npcs[o_id].level = my_packet->level;
			npcs[o_id].exp = my_packet->exp;
		}
		break;
	}
	case SC_CHAT:
	{
		SC_CHAT_PACKET* my_packet = reinterpret_cast<SC_CHAT_PACKET*>(ptr);
		switch (my_packet->chat_type)
		{
			case CHAT_TYPE_REQUEST_PARTY:
			{
				avatar.set_system_chat(my_packet->mess);
				avatar.toggleRequset = true;
				avatar.requsetId = my_packet->id;
				break;
			}
			case CHAT_TYPE_PARTY_YES:
			{
				avatar.setParty(my_packet->id);
				players[my_packet->id].isParty = true;
				break;
			}
			case CHAT_TYPE_SYSTEM:
			{
				worldChat temp;
				temp.set_chat(my_packet->mess);
				if (worldSystemChat.size() > 5) {
					worldSystemChat.erase(worldSystemChat.begin());
				}
				worldSystemChat.push_back(temp);

				if (my_packet->id != g_myid)
				{
					avatar.set_hit();
				}
				break;
			}
			break;
		}
		//int o_id = my_packet->id;

		//if (o_id < MAX_USER) {
		//	if (0 == players.count(o_id)) {
		//		cout << "Player " << o_id << " does not exists.\n";
		//		break;
		//	}
		//	players[o_id].set_chat(my_packet->mess);

		//}
		//else {
		//	if (0 == npcs.count(o_id)) {
		//		cout << "NPC " << o_id << " does not exists.\n";
		//		break;
		//	}

		//}
		break;
	}

	default:
		printf("Unknown PACKET type [%d]\n", ptr[1]);
	}
}

void process_data(char* net_buf, size_t io_byte)
{
	char* ptr = net_buf;
	static size_t in_packet_size = 0;
	static size_t saved_packet_size = 0;
	static char packet_buffer[BUF_SIZE];

	while (0 != io_byte) {
		if (0 == in_packet_size) in_packet_size = ptr[0];
		if (io_byte + saved_packet_size >= in_packet_size) {
			memcpy(packet_buffer + saved_packet_size, ptr, in_packet_size - saved_packet_size);
			ProcessPacket(packet_buffer);
			ptr += in_packet_size - saved_packet_size;
			io_byte -= in_packet_size - saved_packet_size;
			in_packet_size = 0;
			saved_packet_size = 0;
		}
		else {
			memcpy(packet_buffer + saved_packet_size, ptr, io_byte);
			saved_packet_size += io_byte;
			io_byte = 0;
		}
	}
}

void client_main()
{
	char net_buf[BUF_SIZE];
	size_t	received;
	auto recv_result = socket.receive(net_buf, BUF_SIZE, received);
	if (recv_result == sf::Socket::Error)
	{
		wcout << L"Recv 에러!";
		while (true);
	}
	if (recv_result == sf::Socket::Disconnected) {
		wcout << L"서버 접속 종료\n";
		client_finish();
		exit(0);

	}
	if (recv_result != sf::Socket::NotReady)
		if (received > 0) process_data(net_buf, received);

	for (int i = 0; i < SCREEN_WIDTH; ++i) {
		for (int j = 0; j < SCREEN_HEIGHT; ++j)
		{
			int tile_x = i + g_left_x;
			int tile_y = j + g_top_y;
			if ((tile_x < 0) || (tile_y < 0)) continue;

			if (g_map[tile_y][tile_x] == N_MAP)
			{
				nomal_tile.a_move(TILE_WIDTH * i, TILE_WIDTH * j);
				nomal_tile.a_draw();
			}
			else if (g_map[tile_y][tile_x] == B_MAP)
			{
				obstacle_tile.a_move(TILE_WIDTH * i, TILE_WIDTH * j);
				obstacle_tile.a_draw();
			}
			else if(g_map[tile_y][tile_x] == T_HOUSE){
				town_house.a_move(TILE_WIDTH * (i-1), TILE_WIDTH * (j-1));
				town_house.a_draw();
			}
			else if (g_map[tile_y][tile_x] == T_ROCK) {
				rock_tile.a_move(TILE_WIDTH * (i), TILE_WIDTH * (j));
				rock_tile.a_draw();
			}
			else if (g_map[tile_y][tile_x] == T_GRASS) {
				grass_tile.a_move(TILE_WIDTH * (i), TILE_WIDTH * (j));
				grass_tile.a_draw();
			}
		}
	}

	
	avatar.draw();

	for (auto& pl : players) pl.second.draw();
	for (auto& pl : npcs) pl.second.draw();
}

void send_packet(void *packet)
{
	unsigned char *p = reinterpret_cast<unsigned char *>(packet);
	size_t sent = 0;
	socket.send(packet, p[0], sent);
}


int main()
{
	//ofstream is("data.txt");
	//std::random_device rd;
	//mt19937 gen(rd());
	//uniform_int_distribution<int> dis(0, 4);
	//for (int i = 0; i < 2000; ++i) {
	//	for (int j = 0; j < 2000; ++j)
	//	{
	//		is << dis(gen);
	//	}
	//	is << "\n";
	//}
	//is.close();

	//cout << "생성완료" << endl;


	init_map();

	
	cout << "Input User ID: ";
	cin >> playerName;
	wcout.imbue(locale("korean"));
	sf::Socket::Status status = socket.connect("127.0.0.1", PORT_NUM);
	socket.setBlocking(false);
	
	CS_LOGIN_PACKET p;
	p.size = sizeof(CS_LOGIN_PACKET);
	p.type = CS_LOGIN;
	strcpy_s(p.name, playerName.c_str());
	send_packet(&p);

	if (status != sf::Socket::Done) {
		wcout << L"서버와 연결할 수 없습니다.\n";
		while (true);
	}

	client_initialize();

	sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "2D CLIENT");
	g_window = &window;


	while (window.isOpen())
	{

		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
			if (event.type == sf::Event::MouseButtonPressed)
			{
				int x =event.mouseButton.x;
				int y = event.mouseButton.y;
				if (avatar.togglePartyMode) {

					for (auto& pl : players) {
						if (pl.second.global_x - 50 < x && pl.second.global_x + 50 > x &&
							pl.second.global_y - 20 < y && pl.second.global_y + 50 > y)
						{
							char temp[20];
							sprintf_s(temp, "Party %d",pl.first);
							CS_CHAT_PACKET chatpacket;
							chatpacket.size = sizeof(p);
							chatpacket.type = CS_CHAT;
							chatpacket.chat_type = CHAT_TYPE_REQUEST_PARTY;
							chatpacket.target_id = pl.first;
							strcpy_s(chatpacket.mess, temp);
							send_packet(&chatpacket);
							cout << "Send Party" << pl.first << endl;
							avatar.togglePartyMode = false;
							break;
						}
					}
				}
			}
			if (event.type == sf::Event::KeyPressed) {
				int direction = -1;
				switch (event.key.code) {
				case sf::Keyboard::Left:
					direction = 2;
					break;
				case sf::Keyboard::Right:
					direction = 3;
					break;
				case sf::Keyboard::Up:
					direction = 0;
					break;
				case sf::Keyboard::Down:
					direction = 1;
					break;
				case sf::Keyboard::Escape:
					window.close();
					break;
				case sf::Keyboard::A:
				{
					if (avatar.m_attack_time <= chrono::system_clock::now())
					{
						avatar.p_attack();
						CS_ATTACK_PACKET p;
						p.size = sizeof(p);
						p.type = CS_ATTACK;
						send_packet(&p);
						cout << "attack" << endl;
						avatar.m_attack_time = chrono::system_clock::now() + chrono::seconds(1);

					}
				}
					break;
				case sf::Keyboard::P: {
					avatar.togglePartyMode = true;
					break;
				}
				case sf::Keyboard::Y: {
					
					if (avatar.toggleRequset)
					{
						CS_CHAT_PACKET chatpacket;
						chatpacket.size = sizeof(p);
						chatpacket.type = CS_CHAT;
						chatpacket.chat_type = CHAT_TYPE_PARTY_YES;
						chatpacket.target_id = avatar.requsetId;
						send_packet(&chatpacket);
						avatar.toggleRequset = false;
					}
					break;
				}
				case sf::Keyboard::N: {
					if (avatar.toggleRequset)
					{
						CS_CHAT_PACKET chatpacket;
						chatpacket.size = sizeof(p);
						chatpacket.type = CS_CHAT;
						chatpacket.chat_type = CHAT_TYPE_PARTY_NO;
						chatpacket.target_id = avatar.requsetId;
						send_packet(&chatpacket);
						avatar.toggleRequset = false;
					}
					break;
				}

				case sf::Keyboard::Z: {
						CS_ITEM_PACKET itempacket;
						itempacket.size = sizeof(p);
						itempacket.type = CS_ITEM;
						send_packet(&itempacket);
					break;
				}
				}
				if (-1 != direction) {
					if (avatar.m_move_time <= chrono::system_clock::now())
					{
						CS_MOVE_PACKET p;
						p.size = sizeof(p);
						p.type = CS_MOVE;
						p.direction = direction;
						send_packet(&p);
						avatar.m_move_time = chrono::system_clock::now() + chrono::milliseconds(500);
					}

				}

			}
		}

		window.clear();
		client_main();
		window.display();
	}
	client_finish();

	return 0;
}

void send_attack_packet()
{
	CS_ATTACK_PACKET p;
	p.size = sizeof(p);
	p.type = CS_ATTACK;
	send_packet(&p);
}