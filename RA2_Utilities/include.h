#pragma once

#define WIN32_LEAN_AND_MEAN             // 从 Windows 头文件中排除极少使用的内容
// Windows 头文件
#include <windows.h>
#include <shellapi.h>
#include <tlhelp32.h>
#include <Psapi.h>
#include <shlwapi.h>

#include <io.h>
#include <map>
#include <thread>
#include <chrono>
#include <string>
#include <vector>
#include <memory>
#include <sstream>
#include <fstream>
#include <cstddef>
#include <iostream>
#include <algorithm>

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


#pragma warning(disable:4996)

using std::thread;
using std::string;
