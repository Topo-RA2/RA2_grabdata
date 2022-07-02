// 目标是注入到游戏内，开启TCP接口&获取游戏的单位数据然后发出来。
// 这样启动的游戏不需要做启动判断，直接读内存就完事儿了。
#include "include.h"
#include "tcp_client.h"
#include "data_acquisition.h"
#include "co/god.h"

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    {
        OutputDebugStringA("DLL_PROCESS_ATTACH");
        // 🙏
        god::bless_no_bugs();
        start_tcp_thread();
        start_data_acquisition_thread();
        break;
    }
    case DLL_THREAD_ATTACH:
    {
        OutputDebugStringA("DLL_THREAD_ATTACH");
        break;
    }
    case DLL_THREAD_DETACH:
    {
        break;
    }
    case DLL_PROCESS_DETACH:
    {
        break;
    }
    }
    return TRUE;
}