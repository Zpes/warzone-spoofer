#include "hook_handler.h"

PDRIVER_DISPATCH hook_handler::original_dispatch = nullptr;

NTSTATUS hook_handler::hooked_device_control(PDEVICE_OBJECT pDeviceObject, PIRP Irp)
{
	switch (IoGetCurrentIrpStackLocation(Irp)->Parameters.DeviceIoControl.IoControlCode)
	{
	case IOCTL_STORAGE_QUERY_PROPERTY:
		if (!((PSTORAGE_PROPERTY_QUERY)Irp->AssociatedIrp.SystemBuffer)->PropertyId == StorageDeviceProperty)
			return hook_handler::original_dispatch(pDeviceObject, Irp);
		// PSTORAGE_DEVICE_DESCRIPTOR

	default:
		print_success_message("hook -> %p", IoGetCurrentIrpStackLocation(Irp)->Parameters.DeviceIoControl.IoControlCode);
		return hook_handler::original_dispatch(pDeviceObject, Irp);
	}
}
