#include "utility.h"

NTSTATUS utility::find_driver_object(PDRIVER_OBJECT* DriverObject, PUNICODE_STRING DriverName)
{
	HANDLE handle{};
	OBJECT_ATTRIBUTES attributes{};
	UNICODE_STRING directory_name{};
	PVOID directory{};
	BOOLEAN success = FALSE;

	RtlInitUnicodeString(&directory_name, L"\\Driver");
	InitializeObjectAttributes(&attributes, &directory_name, OBJ_CASE_INSENSITIVE, NULL, NULL);

	NTSTATUS status = ZwOpenDirectoryObject(&handle, DIRECTORY_ALL_ACCESS, &attributes);

	if (!NT_SUCCESS(status))
		return status;

	status = ObReferenceObjectByHandle(handle, DIRECTORY_ALL_ACCESS, nullptr, KernelMode, &directory, nullptr);

	if (!NT_SUCCESS(status))
	{
		ZwClose(handle);
		return status;
	}

	POBJECT_DIRECTORY directory_object = POBJECT_DIRECTORY(directory);

	ExAcquirePushLockExclusiveEx(&directory_object->Lock, 0);

	for (POBJECT_DIRECTORY_ENTRY entry : directory_object->HashBuckets)
	{
		if (!entry)
			continue;

		if (success)
			break;

		while (entry && entry->Object)
		{
			PDRIVER_OBJECT driver = PDRIVER_OBJECT(entry->Object);

			if (RtlCompareUnicodeString(&driver->DriverName, DriverName, FALSE) == 0)
			{
				*DriverObject = driver;
				success = TRUE;
			}
			entry = entry->ChainLink;
		}
	}

	ExReleasePushLockExclusiveEx(&directory_object->Lock, 0);

	ObDereferenceObject(directory);
	ZwClose(handle);

	return success == TRUE ? STATUS_SUCCESS : STATUS_NOT_FOUND;
}