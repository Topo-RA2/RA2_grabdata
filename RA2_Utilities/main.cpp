#include "include.h"

#include "imgui_main.h"
#include "find_game.h"
#include "http_server.h"

Game game;

int _stdcall WinMain(
    HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    LPSTR lpCmdLine,
    int nCmdShow
)
{
    thread http_thread(start_http_server);
    http_thread.detach();

    thread imgui_thread(showGuiThread);
    imgui_thread.join();
}
