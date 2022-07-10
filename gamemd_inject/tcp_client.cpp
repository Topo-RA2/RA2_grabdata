#include "tcp_client.h"
#include "data_acquisition.h"

tcp::Client c("127.0.0.1", 7788);
extern queue<FrameInfo> queue_each_frame;
extern mutex queue_mtx;

void send_data()
{
    while (true)
    {
        Sleep(100);
        if (c.connected())
        {
            while (!queue_each_frame.empty())
            {
                queue_mtx.lock();
                struct FrameInfo each_frame_info = queue_each_frame.front();
                queue_each_frame.pop();
                queue_mtx.unlock();

                vector<UnitInfo> vec_unit_info = each_frame_info.all_unit;
                vector<PlayerDataInfo> vec_player_info = each_frame_info.p_data;

                Json send_tmp;
                for (int i = 0; i < vec_unit_info.size(); ++i)
                {
                    Json each_tmp;
                    each_tmp["id"] = vec_unit_info[i].id;
                    each_tmp["name"] = vec_unit_info[i].name;
                    each_tmp["x"] = vec_unit_info[i].x;
                    each_tmp["y"] = vec_unit_info[i].y;
                    each_tmp["p_id"] = vec_unit_info[i].p_id;
                    
                    send_tmp["unit_data"].push_back(each_tmp.str());
                }
                for (int i = 0; i < vec_player_info.size(); ++i)
                {
                    Json each_tmp;
                    each_tmp["id"] = vec_player_info[i].id;
                    each_tmp["cash"] = vec_player_info[i].cash;
                    each_tmp["consume"] = vec_player_info[i].consume;
                    each_tmp["powerPro"] = vec_player_info[i].powerPro;
                    each_tmp["powerCon"] = vec_player_info[i].powerCon;

                    send_tmp["player_data"].push_back(each_tmp.str());
                }

                LOG << "send_tmp: " << send_tmp;
                int r = c.send(send_tmp.str().c_str(), send_tmp.str().size());
                if (r <= 0)
                {
                    LOG << "client send error: " << c.strerror();
                    break;
                }
            }
            
        }
        else
        {
            if (!c.connect(2000))
            {
                LOG << "connect failed: " << c.strerror();
                continue;
            }
        }
    }
}

void tcp_thread()
{
    go(send_data);
}

void start_tcp_thread()
{
    OutputDebugStringA("[start_tcp_thread] Client Start!");
    thread t{ tcp_thread };
    t.detach();
}