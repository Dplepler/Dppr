#ifndef DRIVER_H
#define DRIVER_H

//#include <wdm.h>
//#include <ntddk.h>
#include <ntifs.h>

#include <windows.h>


#include <stdio.h>
#include <tchar.h>
#include <ctype.h>
#include <stdlib.h>

//#include <psapi.h>
#include <time.h>
//#include <tlhelp32.h>
#include <string.h>
//#include <winioctl.h>

#define IRP_ROOTKIT_CODE 0x815

#define PROCAMOUNT 3

DRIVER_INITIALIZE DriverEntry;
VOID DriverUnload(_In_ PDRIVER_OBJECT DriverObject);

NTSTATUS hidep();
ULONG find_eproc_pid();

NTSTATUS defaultIrpHandler(_In_ PDEVICE_OBJECT DeviceObject, _In_ PIRP IrpMessage);
NTSTATUS IrpCallRootkit(_In_ PDEVICE_OBJECT DeviceObject, _In_ PIRP Irp);

#endif