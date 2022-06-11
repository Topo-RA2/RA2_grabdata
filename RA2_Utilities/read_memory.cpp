#include "read_memory.h"

extern Game game;

DWORD readMemory(HANDLE handle, DWORD m)
{
    DWORD add_m;
    ReadProcessMemory(handle, (void*)m, &add_m, 4, 0);
    return add_m;
}

DWORD readMemory(HANDLE handle, DWORD m, DWORD n)
{
    DWORD add_m;
    ReadProcessMemory(handle, (void*)m, &add_m, 4, 0);
    m = add_m + n;
    ReadProcessMemory(handle, (void*)m, &add_m, 4, 0);
    return add_m;
}

DWORD readMemory(HANDLE handle, DWORD m, DWORD n, DWORD o)
{
    DWORD add_m;
    ReadProcessMemory(handle, (void*)m, &add_m, 4, 0);
    m = add_m + n;
    ReadProcessMemory(handle, (void*)m, &add_m, 4, 0);
    m = add_m + o;
    ReadProcessMemory(handle, (void*)m, &add_m, 4, 0);
    return add_m;
}
wchar_t* readMemoryW(HANDLE handle, DWORD m, DWORD n, DWORD o)
{
    DWORD add_m;
    ReadProcessMemory(handle, (void*)m, &add_m, 4, 0);
    m = add_m + n;
    ReadProcessMemory(handle, (void*)m, &add_m, 4, 0);
    m = add_m + o;
    wchar_t* ans = nullptr;
    ReadProcessMemory(handle, (void*)m, ans, 40, 0);
    return ans;
}

DWORD readMemory(HANDLE handle, DWORD m, DWORD n, DWORD o, DWORD p, DWORD q)
{
    DWORD add_m;
    ReadProcessMemory(handle, (void*)m, &add_m, 4, 0);
    m = add_m + n;
    ReadProcessMemory(handle, (void*)m, &add_m, 4, 0);
    m = add_m + o;
    ReadProcessMemory(handle, (void*)m, &add_m, 4, 0);
    m = add_m + p;
    ReadProcessMemory(handle, (void*)m, &add_m, 4, 0);
    m = add_m + q;
    ReadProcessMemory(handle, (void*)m, &add_m, 4, 0);
    return add_m;
}

DWORD getGameFrame()
{
    if (game.game_handle == nullptr)
        return 0;
    else
        return readMemory(game.game_handle, 0xA8ED84);
}

DWORD getGameTime()
{
    if (game.game_handle == nullptr)
        return 0;
    else
        return readMemory(game.game_handle, 0xABCD4C);
}

DWORD getPlayerNum()
{
    if (game.game_handle == nullptr)
        return 0;
    else
        return readMemory(game.game_handle, 0xA8DA84);
}

DWORD getPlayerId(int i)
{
    if (game.game_handle == nullptr)
        return 0;
    else
        return readMemory(game.game_handle, 0xA8022C, 0x4 * i, 0x30);
}

wchar_t* getPlayerName(int i)
{
    if (game.game_handle == nullptr)
        return 0;
    else
        return readMemoryW(game.game_handle, 0xA8022C, 0x4 * i, 0x1602A);
}

DWORD getPlayerColor(int i)
{
    if (game.game_handle == nullptr)
        return 0;
    else
        return readMemory(game.game_handle, 0xA8022C, 0x4 * i, 0x16054);
}

DWORD getPlayerSpectator(int i)
{
    if (game.game_handle == nullptr)
        return 0;
    else
        return readMemory(game.game_handle, 0xA8022C, 0x4 * i, 0x1605C);
}

DWORD getPlayerFund(int i)
{
    if (game.game_handle == nullptr)
        return 0;
    else
        return readMemory(game.game_handle, 0xA8022C, 0x4 * i, 0x30C);
}

DWORD getPlayerConsume(int i)
{
    if (game.game_handle == nullptr)
        return 0;
    else
        return readMemory(game.game_handle, 0xA8022C, 0x4 * i, 0x2DC);
}

