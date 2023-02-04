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
                        if (game_status == 1) // ��Ϸ�رգ��򿪽������
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
                    { ͨ����������ȡ���� ID }
                    GetWindowThreadProcessId(hWindow, dwProcessID);

                    { ͨ������ ID ��ȡ���̾�� }
                    hProcess := OpenProcess(PROCESS_ALL_ACCESS, False, dwProcessID);
                    */
                    else
                    {
                        if (game_status == 0)//��һ�η��ֽ��̣��رս������
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
