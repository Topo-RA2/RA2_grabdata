#include "include.h"

#include "imgui_main.h"
#include "find_game.h"
#include "http_server.h"

Game game;

int main()
{
    thread http_thread(start_http_server);
    http_thread.detach();

    thread imgui_thread(showGuiThread);
    imgui_thread.join();
}