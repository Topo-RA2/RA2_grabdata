#include "process_data.h"
#include "find_game.h"

extern Game game;
extern int thread_num;

vector<fs::path> json_list;
fs::path new_json_file;

vector<FrameInfo> all_frame;
vector<GlobalPlayerInfo> all_player_info;

int showUIState = 0;

void find_json_file()
{
    int new_time_file = -1;
    for (const fs::directory_entry& entry : fs::directory_iterator(fs::current_path()))
    {
        if (entry.is_regular_file() and entry.path().extension() == ".json")
        {
            json_list.push_back(entry.path());
            
            if (new_time_file < fs::last_write_time(entry.path()).time_since_epoch().count())
            {
                new_json_file = entry.path();
                new_time_file = fs::last_write_time(entry.path()).time_since_epoch().count();
            }
        }
    }
}

void handle_game_data()
{
    /*thread t
    {
        [&]()
        {
            
        }
    };
    t.detach();*/

    while (thread_num != 0)
    {
        Sleep(1);
    }
    find_json_file();
    if (!json_list.empty())
    {
        std::ifstream ifs(new_json_file, std::ios::in);
        std::istreambuf_iterator<char> beg(ifs), end;
        string strdata(beg, end);
        ifs.close();
        auto a_game_data = json::parse(strdata);
        BaseLog::Output(a_game_data.dump());

        GlobalPlayerInfo tmpGlobalPlayerInfo;

        for (auto& item : a_game_data.items())
        {
            if (item.key() == "all_player_info")
            {
                for (auto& api_item : item.value()) // []
                {
                    for (auto& attr : api_item.items()) // {}
                    {
                        if (attr.key() == "index")
                        {
                            tmpGlobalPlayerInfo.index = attr.value();
                        }
                        else if (attr.key() == "color")
                        {
                            tmpGlobalPlayerInfo.color = attr.value();
                        }
                        else if (attr.key() == "name")
                        {
                            //tmpGlobalPlayerInfo.name = attr.value();
                        }
                    }
                    all_player_info.push_back(tmpGlobalPlayerInfo);
                }
            }
            else if (item.key() == "data")
            {
                for (auto& data_item : item.value()) // []
                {
                    FrameInfo tmpFrameInfo;
                    for (auto& data_attr : data_item.items()) // {}
                    {
                        if (data_attr.key() == "frame")
                        {
                            tmpFrameInfo.frame = data_attr.value();
                        }
                        else if (data_attr.key() == "time")
                        {
                            tmpFrameInfo.time = data_attr.value();
                        }
                        else if (data_attr.key() == "frame_player_info")
                        {
                            for (auto& fpi_item : data_attr.value()) // []
                            {
                                FramePlayerInfo tmpFramePlayerInfo{};
                                for (auto& fpi_attr : fpi_item.items()) // {}
                                {
                                    if (fpi_attr.key() == "index")
                                    {
                                        tmpFramePlayerInfo.index = fpi_attr.value();
                                    }
                                    else if (fpi_attr.key() == "factories")
                                    {
                                        tmpFramePlayerInfo.factories = fpi_attr.value();
                                    }
                                    else if (fpi_attr.key() == "funds")
                                    {
                                        tmpFramePlayerInfo.funds = fpi_attr.value();
                                    }
                                    else if (fpi_attr.key() == "spent")
                                    {
                                        tmpFramePlayerInfo.spent = fpi_attr.value();
                                    }
                                    else if (fpi_attr.key() == "powerOutput")
                                    {
                                        tmpFramePlayerInfo.powerOutput = fpi_attr.value();
                                    }
                                    else if (fpi_attr.key() == "powerDrain")
                                    {
                                        tmpFramePlayerInfo.powerDrain = fpi_attr.value();
                                    }
                                }
                                tmpFrameInfo.vec_fpi.push_back(tmpFramePlayerInfo);
                            }
                        }
                        else if (data_attr.key() == "frame_unit_info")
                        {
                            for (auto& fui_item : data_attr.value()) // []
                            {
                                FrameUnitInfo tmpFrameUnitInfo{};
                                for (auto& fpi_attr : fui_item.items()) // {}
                                {
                                    if (fpi_attr.key() == "ID")
                                    {
                                        //tmpFrameUnitInfo.id = fpi_attr.value();
                                    }
                                    else if (fpi_attr.key() == "X")
                                    {
                                        tmpFrameUnitInfo.x = fpi_attr.value();
                                    }
                                    else if (fpi_attr.key() == "Y")
                                    {
                                        tmpFrameUnitInfo.y = fpi_attr.value();
                                    }

                                }
                                tmpFrameInfo.vec_fui.push_back(tmpFrameUnitInfo);
                            }
                        }
                    }
                    all_frame.push_back(tmpFrameInfo);
                }
            }
        }

        std::sort(all_frame.begin(), all_frame.end(), std::less<FrameInfo>());
        std::sort(all_player_info.begin(), all_player_info.end(), std::less<GlobalPlayerInfo>());

        showUIState = 1;
    }
    else
    {
        showUIState = 999;
    }
}
