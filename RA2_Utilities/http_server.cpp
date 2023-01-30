#include "http_server.h"
#include "co/so/http.h"

void start_http_server()
{
    http::Server ss;
    ss.on_req(
        [](const http::Req& req, http::Res& res) {
            if (req.is_method_get()) {
                if (req.url() == "/data.json")
                {
                    res.set_status(200);
                    res.add_header("Access-Control-Allow-Origin", "*");
                    res.add_header("Content-Type", "application/json;charset=UTF-8");

                    vec_player_mtx.lock();

                    Json user_array;

                    for (int i = 0; i < vec_player.size(); ++i)
                    {
                        auto it = curr_game_uid_pos.find(vec_player[i]->player_info_data.uid);
                        if (it == curr_game_uid_pos.end()) // 在成员数组但是不是游戏内的玩家
                            continue;

                        Json tmp;
                        tmp["pos"]["x"] = real_pos[curr_game_uid_pos[vec_player[i]->player_info_data.uid]].x - 37;
                        tmp["pos"]["y"] = real_pos[curr_game_uid_pos[vec_player[i]->player_info_data.uid]].y - 108;
                        tmp["name"] = vec_player[i]->player_info_data.uname;
                        tmp["t_cd"] = epoch::ms() - vec_player[i]->last_A_timestamp;
                        tmp["face"] = vec_player[i]->player_info_data.uface;
                        tmp["guardLevel"] = vec_player[i]->player_info_data.guardLevel;
                        user_array.push_back(tmp);
                    }
                    Json send_data;
                    send_data.add_member("user_array", user_array);
                    LOG << "send_data:" << send_data;
                    res.set_body(send_data.str().c_str());
                    vec_player_mtx.unlock();
                }
                else if (req.url() == "/info.json")
                {
                    res.set_status(200);
                    res.add_header("Access-Control-Allow-Origin", "*");
                    res.add_header("Content-Type", "application/json;charset=UTF-8");

                    Json info_array;

                    queue_info_mtx.lock();
                    while (!queue_info.empty())
                    {
                        Json tmp = queue_info.front();
                        info_array.push_back(tmp);
                        queue_info.pop();
                    }
                    queue_info_mtx.unlock();

                    Json send_data;
                    send_data.add_member("info_array", info_array);
                    res.set_body(send_data.str().c_str());
                }
                else
                {
                    res.set_status(404);
                }
            }
            else {
                res.set_status(405);
            }
        }
    );
    // start as a http server
    ss.start("0.0.0.0", 57782);
}