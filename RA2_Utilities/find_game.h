#pragma once
#include "include.h"

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
};