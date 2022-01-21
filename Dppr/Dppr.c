#include "Utils.h"

#define ARG L"monitorproc"

#define SERVICE L"Whatahek"
//#define DNAME   L"AAAtest"
#define DEVICE L"\\\\.\\Whatahek"
#define DRIVER L"C:\\Windows\\System32\\drivers\\DPDriver.sys"

HANDLE install_driver();
BOOL load_driver(SC_HANDLE svcHandle);

//void monitor();

//VOID CALLBACK WaitOrTimerCallback(
//	_In_  PVOID lpParameter,
//	_In_  BOOLEAN TimerOrWaitFired
//)
//{
//	LPWSTR procn = (LPWSTR)LocalAlloc(LMEM_ZEROINIT, MAX_PATH * 2);
//	GetModuleFileName(NULL, procn, MAX_PATH * 2);
//	ShellExecuteW(NULL, NULL, procn, ARG, NULL, SW_SHOWDEFAULT);
//	LocalFree(procn);
//
//	monitor();
//}


int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow) {

	char pid[32] = { 0 };
	sprintf_s(pid, 32, "%lu", GetCurrentProcessId());

	popup(pid, pid);

	HANDLE device = install_driver();

	NTSTATUS status;
	ULONG bytesReturned;

	BOOLEAN result = 
		DeviceIoControl
	(
		device,
		0x815,
		pid,
		strlen(pid) + 1,
		&status,
		32,
		&bytesReturned,
		NULL
	);

	if (!result) {
		char b[100];
		sprintf_s(b, "%lu", GetLastError());
		popup(b, b);
	}
	else {
		popup("Interesting", "Hmm");
	}



	CloseHandle(device);
	
	
	//int argc;

	//popup("Process Started..", "Hi");

	//LPWSTR* argv = CommandLineToArgvW(GetCommandLineW(), &argc);

	//if (argc == 1 || lstrcmpW(argv[1], ARG)) {

	//	
	//	//CreateThread(NULL, NULL, &blink, NULL, 0, NULL);

	//	LPWSTR procn = (LPWSTR)LocalAlloc(LMEM_ZEROINIT, MAX_PATH * 2);
	//	GetModuleFileName(NULL, procn, MAX_PATH * 2);

	//	ShellExecuteW(NULL, NULL, procn, ARG, NULL, SW_SHOWDEFAULT);
	//	
	//	Sleep(100000);
	//	popup("lol", "Still using this computer?\n");
	//	
	//}
	//else {

	//	//argv[0] = (LPWSTR)L"Blabla";

	//}

	//monitor();

	//while (popup("lol", "Still using this computer?\n") == IDYES) {}
	//HANDLE lightsT = CreateThread(NULL, NULL, &FlashLEDs, NULL, 0, NULL);


	//createNote(".\\note.txt", "WOowoowowooOOWoWOo", TRUE);


	//makeSearch("https://www.youtube.com/watch?v=T5y_OcKDadQ");

	//freezeCursor(15);



	while (TRUE) Sleep(10000);
	return 0;
}

BOOL load_driver(SC_HANDLE svcHandle) {

	// Attempt to start the service
	if (!StartService(svcHandle, 0, NULL)) {

		// Check if error was due to the driver already running
		if (GetLastError() == ERROR_SERVICE_ALREADY_RUNNING) {

			popup("Service running", "GRRR");
			return TRUE;

		}
		else {
			printError();
			popup("I was here", "Such amazing information");
			return FALSE;
		}
	}

	return TRUE;
}


HANDLE install_driver() {

	SC_HANDLE hSCManager = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	SC_HANDLE hService = OpenService(hSCManager, SERVICE, SERVICE_ALL_ACCESS);

	HANDLE device = NULL;

	/* Installing service (if it doesn't exist yet..) */
	if (!hService && GetLastError() == ERROR_SERVICE_DOES_NOT_EXIST) {

		hService = CreateService
		(
			hSCManager,
			SERVICE,
			SERVICE,
			SC_MANAGER_ALL_ACCESS,
			SERVICE_KERNEL_DRIVER,
			SERVICE_DEMAND_START,
			SERVICE_ERROR_IGNORE,
			DRIVER,
			NULL, NULL, NULL, NULL, NULL

		);

		if (!hService) {
			popup("Could not create service", "Oop");
			printError();
		}
		else {
			popup("Hi", "Else statenebt");
		}

		

	}
	else if (!hService) {

		if (load_driver(hService)) {

			popup("loaded driver", "YEAH!");

		}
		else {
			popup("Didn't load driver", "NO");
		}
	}

	popup("Error Incoming!", "Scary");

	device = CreateFileW
	(
		DEVICE,
		GENERIC_READ | GENERIC_WRITE,
		0,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL
	);

	if (device == INVALID_HANDLE_VALUE) {
		popup("badbad", "bad");
	}

	device == INVALID_HANDLE_VALUE && !load_driver(hService) ? NULL : device;
	

	CloseServiceHandle(hService);
	CloseServiceHandle(hSCManager);

	return hService;
}




//void monitor() {
//
//	HANDLE snap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);	// Get all current running processes
//
//	LPWSTR iprocn = (LPWSTR)LocalAlloc(LMEM_ZEROINIT, 512);			// Get the current process's name
//	GetProcessImageFileNameW(GetCurrentProcess(), iprocn, 512);
//	iprocn = getImagNameW(iprocn);
//
//	HANDLE cproc;
//	char* cprocn = NULL;
//
//	PROCESSENTRY32 entry;
//	entry.dwSize = sizeof(entry);
//
//	Process32First(snap, &entry);
//
//	HANDLE nh;
//
//	do {
//
//		if (!lstrcmpW(iprocn, entry.szExeFile) && entry.th32ProcessID != GetCurrentProcessId()) {
//			cproc = OpenProcess(PROCESS_ALL_ACCESS, FALSE, entry.th32ProcessID);
//			RegisterWaitForSingleObject(&nh, cproc, WaitOrTimerCallback, NULL, INFINITE, WT_EXECUTEONLYONCE);
//			break;
//		}
//
//	} while (Process32Next(snap, &entry));
//
//
//	CloseHandle(snap);		// Snapshot will change each time a process is being created, make sure it is updated
//}



