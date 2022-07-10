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
#include <unordered_map>

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

#include "co/json.h"

#pragma warning(disable:4996)

using std::cout;
using std::queue;
using std::mutex;
using std::thread;
using std::string;
using std::vector;
