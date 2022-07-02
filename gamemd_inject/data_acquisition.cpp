#include "data_acquisition.h"

DWORD get_game_frame()
{
    return *(LPDWORD)0x00A8ED84;
}
DWORD get_player_num()
{
    return *(LPDWORD)0xA80238 - 2;
}
DWORD get_player_color(int i)
{
    return
        *(LPDWORD)
        (*(LPDWORD)(( *(LPDWORD)0xA8022C) + 0x4 * i) + 0x16054);
}
wchar_t* get_player_name(int i)
{
    return 
        (wchar_t*)
        (*(LPDWORD)((*(LPDWORD)0xA8022C) + 0x4 * i) + 0x1602A);
}
DWORD get_player_id(int i)
{
    return 
        *(LPDWORD)
        (*(LPDWORD)((*(LPDWORD)0xA8022C) + 0x4 * i) + 0x30);
}
DWORD get_player_id(LPDWORD ptr)
{
    return *(LPDWORD)(ptr + 0x30 / 4);
}
DWORD get_player_spectator(int i)
{
    return 
        *(LPDWORD)
        (*(LPDWORD)((*(LPDWORD)0xA8022C) + 0x4 * i) + 0x16058);
}
LPDWORD get_unit_descriptor(LPDWORD* p_unit_class)
{
    LPDWORD p_unit = nullptr;
    if (!p_unit_class) {
        return nullptr;
    }
    __asm {
        pushad
        mov ebx, p_unit_class
        mov ebx, [ebx]
        mov ecx, p_unit_class
        call[ebx + 0x88]
        mov p_unit, eax
        popad
    }
    return p_unit;
}
LPDWORD get_unit_owner(LPDWORD* p_unit_class)
{
    /*
    LPDWORD p_owner_class = nullptr;
    __asm {
        pushad
        mov ecx, p_unit_class
        mov eax, 0x0070F820
        call eax
        mov p_owner_class, eax
        popad
    }
    return p_owner_class;
    */
    return (LPDWORD)(p_unit_class + 0x21C / 4);
}
char* get_unit_name(LPDWORD* p_unit_class) {
    LPDWORD p_unit = get_unit_descriptor(p_unit_class);
    if (p_unit)
        return (char*)(p_unit + 0x24 / 4);
    else
        return nullptr;
}
DWORD get_unit_id(LPDWORD* u_ptr)
{
    return *(LPDWORD)(u_ptr + 0x10 / 4);
}
DWORD get_unit_x(LPDWORD* u_ptr)
{
    return *(LPDWORD)(u_ptr + 0x208 / 4);
}
DWORD get_unit_y(LPDWORD* u_ptr)
{
    return *(LPDWORD)(u_ptr + 0x20C / 4);
}

struct PlayerInfo player_info[8];
queue<vector<UnitInfo>> queue_all_unit;
mutex queue_mtx;

void data_acquisition()
{
    while (true)
    {
        Sleep(10);
        if (get_game_frame() > 0)
        {
            for (int i = 0; i < get_player_num(); ++i)
            {
                player_info[i].color = get_player_color(i);
                wcscpy(player_info[i].name, get_player_name(i));
                player_info[i].id = get_player_id(i);
                player_info[i].spectator = get_player_spectator(i) == 9 ? 1 : 0;
            }
            for (int i = 0; i < get_player_num(); ++i)
            {
                std::cout << "[readGameDataTask] i: " << i << "\n"
                    << "player_info[i].color: " << player_info[i].color << "\n"
                    << "player_info[i].id: " << player_info[i].id << "\n"
                    << "player_info[i].spectator: " << player_info[i].spectator << "\n";
                std::wcout << "player_info[i].name: " << player_info[i].name << "\n";
            }
            break;
        }
    }
    while (true) // 游戏已经开始了
    {
        Sleep(1000);

        DWORD g_unit_num = *(LPDWORD)0x00A8EC88;
        LPDWORD** g_unit_array = *(LPDWORD***)0x00A8EC7C;
        vector<UnitInfo> all_unit;
        for (DWORD i = 0; i < g_unit_num; i++) {
            LPDWORD* p_unit_class = g_unit_array[i];
            LPDWORD p_owner = get_unit_owner(p_unit_class);//getPlayerPtrByUnit
            int p_id = get_player_id(p_owner);
            int unit_id = get_unit_id(p_unit_class);
            char* unit_name(get_unit_name(p_unit_class));
            int unit_x = get_unit_x(p_unit_class);
            int unit_y = get_unit_y(p_unit_class);
            struct UnitInfo tmp
            {
                unit_id,
                unit_name,
                unit_x,
                unit_y,
                p_id,
            };
            all_unit.push_back(tmp);
        }
        queue_mtx.lock();
        queue_all_unit.push(all_unit);
        queue_mtx.unlock();
    }
}

void start_data_acquisition_thread()
{
    OutputDebugStringA("[start_data_acquisition_thread] Start!");
    thread t{ data_acquisition };
    t.detach();
}