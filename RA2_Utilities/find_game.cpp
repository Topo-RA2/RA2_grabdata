#include "find_game.h"

Game::Game()
{
	searchGameTask();
    readGameDataTask();
}

Game::~Game()
{

}

void Game::searchGameTask()
{
    cout << "[searchGameTask] enter" << "\n";
	thread t(
		[&]() {
			while (true)
			{
                Sleep(1000);
                try
                {
                    HWND game_window_handle = FindWindowA("Yuris Revenge ", "Yuris Revenge ");
                    if (game_window_handle == nullptr)
                    {
                        if (game_status == 1)
                        {
                            game_pid = 0;
                            game_status = 0;
                            CloseHandle(game_handle);
                            game_handle = nullptr;
                        }
                        continue;
                    }
                    /*
                    { 通过窗体句柄获取进程 ID }
                    GetWindowThreadProcessId(hWindow, dwProcessID);

                    { 通过进程 ID 获取进程句柄 }
                    hProcess := OpenProcess(PROCESS_ALL_ACCESS, False, dwProcessID);
                    */
                    else
                    {
                        if (game_status == 0)//第一次发现进程
                        {
                            //cout << "[searchGameTask] find game & init" << "\n";
                            GetWindowThreadProcessId(game_window_handle, &game_pid);
                            game_handle = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, game_pid);
                            ++game_id;
                            game_status = 1;
                            read_once = 1;
                        }
                    }
                    //std::cout << "[searchGameTask] game_status: " << game_status << '\n';
                }
                catch (...)
                {

                }
                
				
			}
		}
	);
    t.detach();
}
vector<std::unordered_map<const char*, int>> each_player_all_data[8];
struct PlayerInfo player_info[8];
void Game::readGameDataTask()
{
    cout << "[readGameDataTask] enter" << "\n";
    thread t(
        [&]() {
            while (true)
            {
                Sleep(1000);
                try
                {
                    cout << "[readGameDataTask] game_handle: " << game_handle << "\n";
                    if (game_handle != nullptr) // 游戏存在
                    {
                        cout << "game_status: " << game_status << "\n"
                            << "getGameFrame: " << getGameFrame() << "\n";

                        if (1 == game_status && getGameFrame() > 0) // 游戏进行中
                        {
                            cout << "read_once: " << read_once << "\n"
                                << "getPlayerNum: " << getPlayerNum() << "\n";
                            if (1 == read_once)
                            {
                                read_once = 0;
                                for (int i = 0; i < getPlayerNum(); ++i)
                                {
                                    player_info[i].color = getPlayerColor(i);
                                    wcscpy(player_info[i].name, getPlayerName(i));
                                    player_info[i].id = getPlayerId(i);
                                    player_info[i].spectator = getPlayerSpectator(i) == 9 ? 1 : 0;
                                }
                                for (int i = 0; i < getPlayerNum(); ++i)
                                {
                                    cout << "[readGameDataTask] i: " << i << "\n"
                                        << "player_info[i].color: " << player_info[i].color << "\n"
                                        << "player_info[i].id: " << player_info[i].id << "\n"
                                        << "player_info[i].spectator: " << player_info[i].spectator << "\n";
                                    std::wcout << "player_info[i].name: " << player_info[i].name << "\n";
                                }
                            }
                            for (int i = 0; i < getPlayerNum(); ++i)
                            {
                                //cout << "getPlayerSpectator(i): " << getPlayerSpectator(i) << "\n";
                                if(0 == getPlayerSpectator(i))
                                    each_player_all_data[i].push_back(GetEachPlayerData(getPlayerPtr(i)));
                            }
                        }
                    }
                }
                catch (...)
                {

                }

                
            }
        }
    );
    t.detach();
}
