#pragma once
#include "include.h"

struct PlayerInfo
{
	wchar_t name[40];
	int color;
	int spectator;
	int id;
};
struct UnitInfo
{
	int id;
	char* name;
	int x;
	int y;
	int p_id;
};

void start_data_acquisition_thread();