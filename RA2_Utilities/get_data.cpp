#include "get_data.h"
#include "read_mem.h"
#include "find_game.h"

extern Game game;
int thread_num = 0;
int player_init[11] = { 0 };

std::string to_utf8(std::wstring& wide_string)
{
	static std::wstring_convert<std::codecvt_utf8<wchar_t>> utf8_conv;
	return utf8_conv.to_bytes(wide_string);
}

void get_game_data()
{
	if (thread_num == 0)
	{
		++thread_num;
	}
	else
	{
		return;
	}

	json curr_game;
	json total_frame_data = json::array();
	while (true)
	{
		Sleep(100);
		if (1 == game.game_status)
		{
			int frame = readMemory(game.game_handle, 0xA8ED84);
			int time = readMemory(game.game_handle, 0xABCD4C);
			if (frame > 0)
			{
				json curr_frame;
				json all_player_info = json::array();
				json frame_player_info = json::array();
				json frame_unit_info = json::array();
				curr_frame.emplace("frame", frame);
				curr_frame.emplace("time", time);
				
				int player_num = readMemory(game.game_handle, 0xA80238);
				int player_vector = readMemory(game.game_handle, 0xA8022C);

				for (int i = 0; i < player_num; ++i)
				{
					int each_player_vector_pointer = readMemory(game.game_handle, player_vector + i * 4);
					int each_player_vector_pointer_index = each_player_vector_pointer + 0x30;
					int each_player_vector_pointer_isHuman = each_player_vector_pointer + 0x1EC;
					int each_player_vector_pointer_name  = each_player_vector_pointer + 0x1602A;
					int each_player_vector_pointer_color = each_player_vector_pointer + 0x16054;
					int each_player_vector_pointer_powerOutput = each_player_vector_pointer + 0x53A4;
					int each_player_vector_pointer_powerDrain = each_player_vector_pointer + 0x53A8;
					int each_player_vector_pointer_funds = each_player_vector_pointer + 0x30C;
					int each_player_vector_pointer_spent = each_player_vector_pointer + 0x2DC;
					int each_player_vector_pointer_factories = each_player_vector_pointer + 0x5380;

					int each_player_vector_pointer_ActiveBuildingTypes = readMemory(game.game_handle, each_player_vector_pointer, 0x5554);
					int each_player_vector_pointer_ActiveUnitTypes = readMemory(game.game_handle, each_player_vector_pointer, 0x5568);
					int each_player_vector_pointer_ActiveInfantryTypes = readMemory(game.game_handle, each_player_vector_pointer, 0x557c);
					int each_player_vector_pointer_ActiveAircraftTypes = readMemory(game.game_handle, each_player_vector_pointer, 0x5590);

					int player_index = 0;
					bool player_isHuman = 0;
					wchar_t player_name[21] = { 0 };
					std::wstring wstring_player_name;
					int player_color = 0;
					int player_powerOutput = 0;
					int player_powerDrain = 0;
					int player_funds = 0;
					int player_spent = 0;
					int player_factories = 0;

					ReadProcessMemory(game.game_handle, (LPCVOID)(each_player_vector_pointer_index), &player_index, sizeof(player_index), 0);
					ReadProcessMemory(game.game_handle, (LPCVOID)(each_player_vector_pointer_isHuman), &player_isHuman, sizeof(player_isHuman), 0);
					if (1 == player_isHuman)
					{
						json curr_player;

						ReadProcessMemory(game.game_handle, (LPCVOID)(each_player_vector_pointer_name), player_name, sizeof(player_name), 0);
						ReadProcessMemory(game.game_handle, (LPCVOID)(each_player_vector_pointer_color), &player_color, sizeof(player_color), 0);
						ReadProcessMemory(game.game_handle, (LPCVOID)(each_player_vector_pointer_powerOutput), &player_powerOutput, sizeof(player_powerOutput), 0);
						ReadProcessMemory(game.game_handle, (LPCVOID)(each_player_vector_pointer_powerDrain), &player_powerDrain, sizeof(player_powerDrain), 0);
						ReadProcessMemory(game.game_handle, (LPCVOID)(each_player_vector_pointer_funds), &player_funds, sizeof(player_funds), 0);
						ReadProcessMemory(game.game_handle, (LPCVOID)(each_player_vector_pointer_spent), &player_spent, sizeof(player_spent), 0);
						ReadProcessMemory(game.game_handle, (LPCVOID)(each_player_vector_pointer_factories), &player_factories, sizeof(player_factories), 0);

						wstring_player_name = std::wstring(player_name);
						/*BaseLog::Output(std::to_string(player_index).c_str());
						BaseLog::Output(std::to_string(player_isHuman).c_str());
						BaseLog::Output(player_name);
						BaseLog::Output(std::to_string(player_color).c_str());
						BaseLog::Output(std::to_string(player_powerOutput).c_str());
						BaseLog::Output(std::to_string(player_powerDrain).c_str());
						BaseLog::Output(std::to_string(player_funds).c_str());
						BaseLog::Output(std::to_string(player_spent).c_str());
						BaseLog::Output(std::to_string(player_factories).c_str());*/

						if (0 == player_init[player_index])
						{
							json curr_player_info;
							curr_player_info.emplace("index", player_index);
							curr_player_info.emplace("name", to_utf8(wstring_player_name));
							curr_player_info.emplace("color", player_color);
							all_player_info.emplace_back(curr_player_info);
							player_init[10] = 1;
						}
						curr_player.emplace("index", player_index);
						curr_player.emplace("powerOutput", player_powerOutput);
						curr_player.emplace("powerDrain", player_powerDrain);
						curr_player.emplace("factories", player_factories);
						curr_player.emplace("funds", player_funds);
						curr_player.emplace("spent", player_spent);
						frame_player_info.emplace_back(curr_player);
					}
				}
				curr_frame.emplace("frame_player_info", frame_player_info);
				if (1 == player_init[10] && !all_player_info.empty())
				{
					curr_game.emplace("all_player_info", all_player_info);
				}

				int unit_num = readMemory(game.game_handle, 0x8B4108 + 0x10);
				int unit_vector = readMemory(game.game_handle, 0x8B4108 + 4);
				for (int i = 0; i < unit_num; ++i)
				{
					int each_unit_vector_pointer = readMemory(game.game_handle, unit_vector + i * 4);
					if (0 == each_unit_vector_pointer)
						continue;

					int each_unit_vector_pointer_UniqueID = each_unit_vector_pointer + 0x0;
					int each_unit_vector_pointer_Owner = each_unit_vector_pointer + 0x21C;
					int each_unit_vector_pointer_X = each_unit_vector_pointer + 0x208;
					int each_unit_vector_pointer_Y = each_unit_vector_pointer + 0x20C;
					int each_unit_vector_pointer_TypesClass = each_unit_vector_pointer + 0x6C4;

					int unit_UniqueID = 0;
					int unit_Owner = 0;
					int unit_OwnerID = 0;
					
					bool player_isHuman = 1;
					int unit_X = 0;
					int unit_Y = 0;
					int TypesClass_pointer = 0;
					
					char unit_ID[24];

					ReadProcessMemory(game.game_handle, (LPCVOID)(each_unit_vector_pointer_Owner), &unit_Owner, sizeof(unit_Owner), 0);
					ReadProcessMemory(game.game_handle, (LPCVOID)(unit_Owner + 0x1EC), &player_isHuman, sizeof(player_isHuman), 0);
					if (1 == player_isHuman)
					{
						ReadProcessMemory(game.game_handle, (LPCVOID)(each_unit_vector_pointer_UniqueID), &unit_UniqueID, sizeof(unit_UniqueID), 0);
						
						ReadProcessMemory(game.game_handle, (LPCVOID)(unit_Owner + 0x30), &unit_OwnerID, sizeof(unit_OwnerID), 0);
						ReadProcessMemory(game.game_handle, (LPCVOID)(each_unit_vector_pointer_X), &unit_X, sizeof(unit_X), 0);
						ReadProcessMemory(game.game_handle, (LPCVOID)(each_unit_vector_pointer_Y), &unit_Y, sizeof(unit_Y), 0);

						ReadProcessMemory(game.game_handle, (LPCVOID)(each_unit_vector_pointer_TypesClass), &TypesClass_pointer, sizeof(TypesClass_pointer), 0);
						ReadProcessMemory(game.game_handle, (LPCVOID)(TypesClass_pointer + 0x24), unit_ID, sizeof(unit_ID), 0);
						
						json curr_unit;
						//curr_unit.emplace("unitUniqueID", unit_UniqueID);
						curr_unit.emplace("ID", unit_ID);
						curr_unit.emplace("X", unit_X);
						curr_unit.emplace("Y", unit_Y);
						frame_unit_info.emplace_back(curr_unit);
					}
				}
				curr_frame.emplace("frame_unit_info", frame_unit_info);
				//BaseLog::Output(frame_unit_info.dump());
				total_frame_data.emplace_back(curr_frame);
				
			}
		}
		else
		{
			curr_game.emplace("data", total_frame_data);
			BaseLog::JsonOutput(curr_game.dump());
			--thread_num;
			break;
		}
	}
}