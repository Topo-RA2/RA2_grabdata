#include "include.h"

#include "imgui_main.h"
#include "find_game.h"

Game game;

int main()
{
    thread imgui_thread(showGuiThread);
    imgui_thread.join();

    
}