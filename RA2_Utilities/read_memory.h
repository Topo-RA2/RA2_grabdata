#pragma once
#include "include.h"
#include "find_game.h"

DWORD readMemory(HANDLE pid, DWORD m);
DWORD readMemory(HANDLE pid, DWORD m, DWORD n);
wchar_t* readMemoryW(HANDLE pid, DWORD m, DWORD n);
DWORD readMemory(HANDLE pid, DWORD m, DWORD n, DWORD o);
DWORD readMemory(HANDLE pid, DWORD m, DWORD n, DWORD o, DWORD p, DWORD q);

DWORD getGameFrame();
DWORD getGameTime();
DWORD getPlayerNum();
DWORD getPlayerId(int i);
wchar_t* getPlayerName(int i);
DWORD getPlayerColor(int i);
DWORD getPlayerSpectator(int i);
DWORD getPlayerFund(int i);
DWORD getPlayerConsume(int i);

