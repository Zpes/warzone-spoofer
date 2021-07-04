#include "hook_handler.h"

PDRIVER_DISPATCH hook_handler::original_dispatch = nullptr;

NTSTATUS disk_completion_routine(PDEVICE_OBJECT pDeviceObject, PIRP Irp, PIO_COMPLETION_STRUCT context)
{
	UNREFERENCED_PARAMETER(Irp);
	UNREFERENCED_PARAMETER(pDeviceObject);

	if (context->requestBuffer->SerialNumberOffset > 0 && context->requestBuffer->SerialNumberOffset < context->OutBufferLength)
	{
		char* current_serial_number = ((char*)context->requestBuffer) + context->requestBuffer->SerialNumberOffset;
		char spoofed_serial[14] = "zpes-was-here"; // F4J48B74B-PQ75HU6I5

		memcpy(current_serial_number, spoofed_serial, sizeof(spoofed_serial));
	}

	return STATUS_SUCCESS;
}

NTSTATUS hook_handler::hooked_device_control(PDEVICE_OBJECT pDeviceObject, PIRP Irp)
{
	PIO_STACK_LOCATION stack = IoGetCurrentIrpStackLocation(Irp);
	switch (stack->Parameters.DeviceIoControl.IoControlCode)
	{
	case IOCTL_STORAGE_QUERY_PROPERTY:
	{
		PSTORAGE_PROPERTY_QUERY request_buffer = (PSTORAGE_PROPERTY_QUERY)Irp->AssociatedIrp.SystemBuffer;

		if (!request_buffer->PropertyId == StorageDeviceProperty)
			return hook_handler::original_dispatch(pDeviceObject, Irp);

		stack->Control = 0;
		stack->Control |= SL_INVOKE_ON_SUCCESS;

		PVOID original_context = stack->Context;

		stack->Context = (PVOID)ExAllocatePool(NonPagedPool, sizeof(IO_COMPLETION_STRUCT));

		PIO_COMPLETION_STRUCT spoofed_context = (PIO_COMPLETION_STRUCT)stack->Context;
		spoofed_context->oldCompletionRoutine = stack->CompletionRoutine;
		spoofed_context->oldContext = original_context;
		spoofed_context->signature = 0x75820834;
		spoofed_context->requestBuffer = (PSTORAGE_DEVICE_DESCRIPTOR)request_buffer;
		spoofed_context->OutBufferLength = stack->Parameters.DeviceIoControl.OutputBufferLength;

		stack->CompletionRoutine = (PIO_COMPLETION_ROUTINE)disk_completion_routine;
	}

	default:
		return hook_handler::original_dispatch(pDeviceObject, Irp);
	}
}