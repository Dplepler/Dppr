#include "Driver.h"

DRIVER_INITIALIZE DriverEntry;
DRIVER_UNLOAD DriverUnload;

UNICODE_STRING deviceName = RTL_CONSTANT_STRING(L"\\Device\\AAAAAmmm");
UNICODE_STRING symbolicLink = RTL_CONSTANT_STRING(L"\\DosDevices\\AAAAAmmm");

NTSTATUS DriverEntry(_In_ PDRIVER_OBJECT DriverObject, _In_ PUNICODE_STRING RegistryPath) {


	NTSTATUS status = STATUS_SUCCESS;
	UNREFERENCED_PARAMETER(RegistryPath);

	PDEVICE_OBJECT deviceObject = NULL;

	for (int i = 0; i < IRP_MJ_MAXIMUM_FUNCTION; i++) {
		DriverObject->MajorFunction[i] = defaultIrpHandler;
	}

	DriverObject->MajorFunction[IRP_MJ_DEVICE_CONTROL] = IrpCallRootkit;


	NTSTATUS ntstatus = STATUS_SUCCESS;
	/////////////////////// THIS SECTION /////////////////////////////////////
	UNICODE_STRING     uniName;
	OBJECT_ATTRIBUTES  objAttr;
	////////////////////////////////\\SystemRoot\\ or C:\WINDOWS / C:|WINNT
	RtlInitUnicodeString(&uniName, L"C:\\proop.txt");  // or L"\\SystemRoot\\example.txt"
	InitializeObjectAttributes(&objAttr, &uniName,
		OBJ_CASE_INSENSITIVE,
		NULL, NULL);
	//////////////////////////////////

	///////////////////////////////////
	//Load the buffer (ie. contents of text file to the console)
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

	status = IoCreateDevice(
		DriverObject,
		0,
		&deviceName,
		FILE_DEVICE_UNKNOWN,
		FILE_DEVICE_SECURE_OPEN,
		FALSE,
		&deviceObject);




	if (!NT_SUCCESS(status)) {
		return status;
	}

	status = IoCreateSymbolicLink(&symbolicLink, &deviceName);

	if (!NT_SUCCESS(status)) {
		IoDeleteDevice(deviceObject);
		return status;
	}

	DriverObject->DriverUnload = DriverUnload;

	return status;
}

VOID DriverUnload(_In_ PDRIVER_OBJECT DriverObject) {

	UNREFERENCED_PARAMETER(DriverObject);

	// Delete our driver device and the associated symbolic link 
	IoDeleteSymbolicLink(&symbolicLink);
	IoDeleteDevice(DriverObject->DeviceObject);

}