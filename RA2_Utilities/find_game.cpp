#include "find_game.h"
#include "apc_inject.h"
#include "tcp_server.h"

#include <co/flag.h>
#include <co/log.h>

Game::Game()
{
    flag::set_value("cout", "true"); // FLG_b -> true
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

                            PCWSTR pszLibFile = NULL;
                            const wchar_t* dll_path = L"C:\\Users\\49673\\Documents\\GitHub\\RA2_grabdata\\Release\\gamemd_inject.dll";
                            pszLibFile = (wchar_t*)malloc((wcslen(dll_path) + 1) * sizeof(wchar_t));
                            pszLibFile = dll_path;

                            LOG << "APC: " << demoQueueUserAPC(pszLibFile, game_pid);
                            start_tcp_thread();
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
