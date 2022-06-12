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
    wchar_t ans[40];
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
        return -1;
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
        return readMemory(game.game_handle, 0xA80238) - 2;
}

DWORD getPlayerPtr(int i)
{
    if (game.game_handle == nullptr)
        return 0;
    else
        return readMemory(game.game_handle, 0xA8022C, 0x4 * i);
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
        return readMemory(game.game_handle, 0xA8022C, 0x4 * i, 0x16058);//0x1605C不是很准
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

std::unordered_map<DWORD, int> GetEachPlayerObjData(DWORD p_player, DWORD offset)
{
    DWORD list = readMemory(game.game_handle, p_player + offset);
    DWORD list_num = readMemory(game.game_handle, p_player + offset + 4);//列表的长度
    //cout << "list_num: " << list_num << "\n";
    std::unordered_map<DWORD, int> exist_num_vec;
    if (list_num == 0) {
        return exist_num_vec;
    }
    for (DWORD i = 0; i < list_num; i++) {
        DWORD t = readMemory(game.game_handle, list + 4 * i);
        if (t > 0) {
            //std::cout << "i:" << i << "\t list[i]: " << (int)t << '\n';
            exist_num_vec.emplace(i, (int)t);//{ id: num }
        }
    }
    return exist_num_vec;
}

extern const char* _IN[];
extern const char* _UN[];
extern const char* _PL[];
extern const char* _BL[];
DWORD b_offset = 0x5554, c_offset = 0x5568, i_offset = 0x557C, p_offset = 0x5590;

std::unordered_map<const char*, int> GetEachPlayerData(DWORD p_player)
{
    cout << "DWORD p_player: " << p_player << '\n';
    std::unordered_map<DWORD, int> b = GetEachPlayerObjData(p_player, b_offset);//building
    std::unordered_map<DWORD, int> c = GetEachPlayerObjData(p_player, c_offset);//car
    std::unordered_map<DWORD, int> i = GetEachPlayerObjData(p_player, i_offset);//infantry
    std::unordered_map<DWORD, int> p = GetEachPlayerObjData(p_player, p_offset);//plane

    std::unordered_map<const char*, int> p_player_all_unit;
    for (auto& each_unit : b)
        p_player_all_unit.emplace(_BL[each_unit.first], each_unit.second);
    /*for (auto& each_unit : c)
        p_player_all_unit.emplace(_UN[each_unit.first], each_unit.second);
    for (auto& each_unit : i)
        p_player_all_unit.emplace(_IN[each_unit.first], each_unit.second);
    for (auto& each_unit : p)
        p_player_all_unit.emplace(_PL[each_unit.first], each_unit.second);*/
    return p_player_all_unit;
}
