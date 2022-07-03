#include "apc_inject.h"
#include "co/co.h"

DWORD demoQueueUserAPC(PCWSTR pszLibFile, DWORD dwProcessId)
{
	int cb = (lstrlenW(pszLibFile) + 1) * sizeof(wchar_t);
	LOG << "cb: " << cb;

	HANDLE hProcess = 
		OpenProcess( PROCESS_VM_OPERATION | PROCESS_VM_WRITE, FALSE, dwProcessId);
	if (hProcess == NULL)
	{
		LOG << "[-] Error: Could not open process for PID " << dwProcessId;
		return(1);
	}

	LPVOID pszLibFileRemote = (PWSTR)VirtualAllocEx(hProcess, NULL, cb, MEM_COMMIT, PAGE_READWRITE);
	if (pszLibFileRemote == NULL)
	{
		LOG << "[-] Error: Could not allocate memory inside PID " << dwProcessId;
		return(1);
	}

	LPVOID pfnThreadRtn = (LPVOID)GetProcAddress(GetModuleHandle(TEXT("Kernel32")), "LoadLibraryW");
	if (pfnThreadRtn == NULL)
	{
		LOG << "[-] Error: Could not find LoadLibraryA function inside kernel32.dll library";
		return(1);
	}

	DWORD n = WriteProcessMemory(hProcess, pszLibFileRemote, (PVOID)pszLibFile, cb, NULL);
	if (n == 0)
	{
		LOG << "[-] Error: Could not write any bytes into the PID (%d) address space" << dwProcessId;
		return(1);
	}

	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);
	if (hSnapshot == INVALID_HANDLE_VALUE)
	{
		LOG << "[-] Error: Unable to get thread information";
		return(1);
	}

	DWORD threadId = 0;
	THREADENTRY32 threadEntry;
	threadEntry.dwSize = sizeof(THREADENTRY32);

	BOOL bResult = Thread32First(hSnapshot, &threadEntry);
	int i = 0;
	while (bResult)
	{
		//LOG << i++;
		bResult = Thread32Next(hSnapshot, &threadEntry);
		if (bResult)
		{
			if (threadEntry.th32OwnerProcessID == dwProcessId)
			{
				threadId = threadEntry.th32ThreadID;

				LOG << "[+] Using thread: " << threadId;
				HANDLE hThread = OpenThread(THREAD_SET_CONTEXT, FALSE, threadId);
				LOG << "hThread: " << hThread;
				if (hThread == NULL)
				{
					LOG << "if tree";
					LOG << "[-] Error: Can't open thread. Continuing to try other threads...";
				}
				else
				{
					LOG << "else tree";
					DWORD dwResult = QueueUserAPC((PAPCFUNC)pfnThreadRtn, hThread, (ULONG_PTR)pszLibFileRemote);
					LOG << "dwResult: " << dwResult;
					if (!dwResult)
					{
						LOG << "[-] Error: Couldn't call QueueUserAPC on thread> Continuing to try othrt threads...";
					}
					else
					{
						LOG << "[+] Success: DLL injected via CreateRemoteThread().";
					}
					CloseHandle(hThread);
				}
			}
		}
	}

	if (!threadId)
		LOG << "[-] Error: No threads found in thr target process";

	CloseHandle(hSnapshot);
	CloseHandle(hProcess);

	return 0;
}