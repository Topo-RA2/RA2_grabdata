#pragma once
#include "include.h"
#include "find_game.h"

DWORD readMemory(HANDLE handle, DWORD m);
DWORD readMemory(HANDLE handle, DWORD m, DWORD n);
wchar_t* readMemoryW(HANDLE handle, DWORD m, DWORD n, DWORD o);
DWORD readMemory(HANDLE handle, DWORD m, DWORD n, DWORD o);
DWORD readMemory(HANDLE handle, DWORD m, DWORD n, DWORD o, DWORD p, DWORD q);

DWORD getGameFrame();
DWORD getGameTime();
DWORD getPlayerNum();
DWORD getPlayerPtr(int i);
DWORD getPlayerId(int i);
wchar_t* getPlayerName(int i);
DWORD getPlayerColor(int i);
DWORD getPlayerSpectator(int i);
DWORD getPlayerFund(int i);
DWORD getPlayerConsume(int i);
std::unordered_map<const char*, int> GetEachPlayerData(DWORD p_player);

