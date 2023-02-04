#pragma once

#define WIN32_LEAN_AND_MEAN             // 从 Windows 头文件中排除极少使用的内容
// Windows 头文件
#include <windows.h>
#include <shellapi.h>
#include <tlhelp32.h>
#include <Psapi.h>
#include <shlwapi.h>

#include <map>
#include <io.h>
#include <ctime>
#include <mutex>
#include <queue>
#include <thread>
#include <chrono>
#include <string>
#include <vector>
#include <memory>
#include <tchar.h>
#include <sstream>
#include <fstream>
#include <cstddef>
#include <iostream>
#include <algorithm>
#include <filesystem>
#include <unordered_map>

#include <codecvt> // codecvt_utf8
#include <locale>  // wstring_convert

#pragma warning (disable: 4005)
#include "d3d9.h"
#include "d3d11.h"
#include "d3dx9tex.h"

#pragma comment(lib,"ws2_32.lib")
#pragma comment (lib,"Advapi32.lib")
#pragma comment (lib,"Iphlpapi.lib")
#pragma comment(lib, "Psapi.lib")
#pragma comment(lib, "user32.lib")
#pragma comment(lib, "userenv.lib")
#pragma comment(lib, "bcrypt.lib")

#include "output_log.h"
#include "httplib.h"
#include "json.hpp"
using json = nlohmann::json;
namespace fs = std::filesystem;

#pragma warning(disable:4996)

using std::cout;
using std::queue;
using std::mutex;
using std::thread;
using std::string;
using std::vector;

extern const char* _IN[];
extern const char* _UN[];
extern const char* _PL[];
extern const char* _BL[];

struct GlobalPlayerInfo
{
	int index;
	int color;
	std::string name;
	bool operator < (const GlobalPlayerInfo& other) const {
		return index < other.index;
	}
	bool operator > (const GlobalPlayerInfo& other) const {
		return index > other.index;
	}
};

struct FramePlayerInfo
{
	int index;
	int funds;
	int spent;
	int powerDrain;
	int powerOutput;
	int factories;
};
struct FrameUnitInfo
{
	int x;
	int y;
	int id;
};
struct FrameInfo
{
	int frame;
	int time;

	vector<FramePlayerInfo> vec_fpi;
	vector<FrameUnitInfo> vec_fui;
	
	bool operator < (const FrameInfo& other) const {
		return frame < other.frame;
	}
	bool operator > (const FrameInfo& other) const {
		return frame > other.frame;
	}
};