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
	thread t(
		[&]() {
			while (true)
			{
                try
                {
                    DWORD aProcesses[1024], cbNeeded;
                    if (!EnumProcesses(aProcesses, sizeof(aProcesses), &cbNeeded)) {
                        //throw -1;
                    }
                    unsigned int cProcesses = cbNeeded / sizeof(DWORD);
                    for (unsigned int i = 0; i < cProcesses; ++i)//����ÿ�����̵����ƺͽ��̱�ʶ��
                    {
                        DWORD processID = aProcesses[i];
                        if (processID == 0)
                            continue;
                        char buff[255];
                        HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, processID);
                        HMODULE hMod;
                        DWORD cbNeeded;
                        if (EnumProcessModules(hProcess, &hMod, sizeof(hMod), &cbNeeded))
                            GetModuleBaseNameA(hProcess, hMod, (LPSTR)&buff, DWORD(sizeof(buff) / sizeof(char)));
                        string s(buff);
                        if (s == "gamemd-spawn.exe" || s == "gamemd.exe") {
                            
                            game_pid = processID;
                            if (game_status != 1)//��һ�η��ֽ���
                            {
                                game_status = 1;
                                game_handle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, game_pid);
                            }
                            break;
                        }
                        if (i == cProcesses - 1 && game_status == 1)//���˸��黹��û�ҵ�
                        {
                            game_pid = 0;
                            game_status = 0;
                            CloseHandle(game_handle);
                            game_handle = nullptr;
                            //throw 0;
                        }
                        CloseHandle(hProcess);
                    }
                }
                catch (...)
                {

                }
                
				Sleep(1000);
			}
		}
	);
    t.detach();
}

void Game::readGameDataTask()
{
    thread t(
        [&]() {
            while (true)
            {
                try
                {
                    if (game_handle != nullptr) // ��Ϸ����
                    {

                    }
                }
                catch (...)
                {

                }

                Sleep(1000);
            }
        }
    );
    t.detach();
}
