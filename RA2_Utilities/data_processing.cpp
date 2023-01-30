#include "data_processing.h"
#include "find_game.h"

extern Game game;
mutex vec_game_data_mtx;
vector<Json> vec_game_data;

vector<int> curr_frame;
vector<int> cash[8];
vector<int> consume[8];
vector<int> powerPro[8];
vector<int> powerCon[8];

void insert_new_data(Json x)
{
	vec_game_data_mtx.lock();
	vec_game_data.push_back(x);
	vec_game_data_mtx.unlock();
}

void save_queue_game_data()
{
	vec_game_data_mtx.lock();

	vec_game_data_mtx.unlock();
}

void load_queue_game_data()
{
	vec_game_data_mtx.lock();

	vec_game_data_mtx.unlock();
}

void reset_queue_game_data()
{
	vec_game_data_mtx.lock();
	vector<Json>().swap(vec_game_data);
	vec_game_data_mtx.unlock();
}

void handle_game_data()
{
	vec_game_data_mtx.lock();
	for (int i = 0; i < 8; ++i)
	{
		vector<int>().swap(curr_frame);
		vector<int>().swap(cash[i]);
		vector<int>().swap(consume[i]);
		vector<int>().swap(powerPro[i]);
		vector<int>().swap(powerCon[i]);
	}
	for (int i = 0; i < vec_game_data.size(); ++i)
	{
		curr_frame.push_back(vec_game_data[i]["curr_frame"].get_int());
		int id = vec_game_data[i]["player_data"]["id"].get_int();
		cash[id].push_back(vec_game_data[i]["player_data"]["cash"].get_int());
		consume[id].push_back(vec_game_data[i]["player_data"]["consume"].get_int());
		powerPro[id].push_back(vec_game_data[i]["player_data"]["powerPro"].get_int());
		powerCon[id].push_back(vec_game_data[i]["player_data"]["powerCon"].get_int());
	}
	vec_game_data_mtx.unlock();
}

void handle_replay_data()
{

}

