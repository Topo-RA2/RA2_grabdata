#include "find_game.h"
#include "process_data.h"
#include "get_data.h"

extern int showUIState;

Game::Game()
{
	searchGameTask();
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
                Sleep(2000);
                try
                {
                    HWND game_window_handle = FindWindowA("Yuris Revenge ", "Yuris Revenge ");
                    if (game_window_handle == nullptr)
                    {
                        if (game_status == 1) // 游戏关闭，打开结算界面
                        {
                            game_pid = 0;
                            game_status = 0;
                            CloseHandle(game_handle);
                            game_handle = nullptr;

                            handle_game_data();
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
                        if (game_status == 0)//第一次发现进程，关闭结算界面
                        {
                            //cout << "[searchGameTask] find game & init" << "\n";
                            GetWindowThreadProcessId(game_window_handle, &game_pid);
                            game_handle = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, game_pid);
                            game_status = 1;
                            showUIState = 0;
                            OutputDebugStringA("Start");

                            thread data_thread(get_game_data);
                            data_thread.detach();
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
