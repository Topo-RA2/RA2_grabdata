#pragma once

#define WIN32_LEAN_AND_MEAN             // 从 Windows 头文件中排除极少使用的内容
// Windows 头文件
#include <windows.h>

#include "co/co.h"
#include "co/json.h"
#include "co/so/tcp.h"
#include "co/thread.h"

#include <io.h>
#include <map>
#include <list>
#include <queue>
#include <mutex>
#include <chrono>
#include <string>
#include <thread>
#include <vector>
#include <memory>
#include <tchar.h>
#include <sstream>
#include <fstream>
#include <cstddef>
#include <iostream>
#include <algorithm>
#include <TlHelp32.h>


#pragma warning(disable:4996)

using std::queue;
using std::mutex;
using std::thread;
using std::vector;