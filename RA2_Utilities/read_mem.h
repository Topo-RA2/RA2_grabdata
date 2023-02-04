#pragma once
#include "include.h"

DWORD readMemory(HANDLE process_handle, DWORD m)
{
    DWORD add_m;
    ReadProcessMemory(process_handle, (void*)m, &add_m, 4, 0);
    return add_m;
}

DWORD readMemory(HANDLE process_handle, DWORD m, DWORD n)
{
    DWORD add_m;
    ReadProcessMemory(process_handle, (void*)m, &add_m, 4, 0);
    m = add_m + n;
    ReadProcessMemory(process_handle, (void*)m, &add_m, 4, 0);
    return add_m;
}

DWORD readMemory(HANDLE process_handle, DWORD m, DWORD n, DWORD o)
{
    DWORD add_m;
    ReadProcessMemory(process_handle, (void*)m, &add_m, 4, 0);
    m = add_m + n;
    ReadProcessMemory(process_handle, (void*)m, &add_m, 4, 0);
    m = add_m + o;
    ReadProcessMemory(process_handle, (void*)m, &add_m, 4, 0);
    return add_m;
}

DWORD readMemory(HANDLE process_handle, DWORD m, DWORD n, DWORD o, DWORD p, DWORD q)
{
    DWORD add_m;
    ReadProcessMemory(process_handle, (void*)m, &add_m, 4, 0);
    m = add_m + n;
    ReadProcessMemory(process_handle, (void*)m, &add_m, 4, 0);
    m = add_m + o;
    ReadProcessMemory(process_handle, (void*)m, &add_m, 4, 0);
    m = add_m + p;
    ReadProcessMemory(process_handle, (void*)m, &add_m, 4, 0);
    m = add_m + q;
    ReadProcessMemory(process_handle, (void*)m, &add_m, 4, 0);
    return add_m;
}