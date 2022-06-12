#pragma once
#include "include.h"
#include "read_memory.h"

struct PlayerInfo
{
	wchar_t name[40];
	int color;
	int spectator;
	int id;
};

class Game
{
private:
	
public:
	int game_status = 0;
	DWORD game_pid = 0;
	HANDLE game_handle = nullptr;
	int game_id = 0;
	int read_once = 0;

	Game();
	~Game();

	void searchGameTask();
	void readGameDataTask();

};