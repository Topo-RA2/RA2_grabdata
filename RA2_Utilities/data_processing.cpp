#include "data_processing.h"
#include "find_game.h"

extern Game game;
mutex vec_game_data_mtx;
vector<Json> vec_game_data;

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
	for (int i = 0; i < vec_game_data.size(); ++i)
	{
		int id = vec_game_data[i]["id"].get_int();
		cash[id].push_back(vec_game_data[i]["cash"].get_int());
		consume[id].push_back(vec_game_data[i]["consume"].get_int());
		powerPro[id].push_back(vec_game_data[i]["powerPro"].get_int());
		powerCon[id].push_back(vec_game_data[i]["powerCon"].get_int());
	}
	vec_game_data_mtx.unlock();
}

void handle_replay_data()
{

}

