#pragma once
#include "include.h"

class Game
{
private:
	
public:
	int game_status = 0;
	DWORD game_pid = 0;
	HANDLE game_handle = nullptr;

	Game();
	~Game();

	void searchGameTask();
};