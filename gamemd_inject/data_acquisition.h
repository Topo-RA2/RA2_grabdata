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

struct PlayerDataInfo
{
	int id;
	int cash;
	int consume;
	int powerPro;
	int powerCon;
};
struct FrameInfo
{
	vector<UnitInfo> all_unit;
	vector<PlayerDataInfo> p_data;
};

void start_data_acquisition_thread();