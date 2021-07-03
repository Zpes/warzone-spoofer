#pragma once
#include <ntifs.h>
#include "undocumented.h"

namespace utility
{
	NTSTATUS find_driver_object(PDRIVER_OBJECT* DriverObject, PUNICODE_STRING DriverName);
}