#pragma once
#include <ntifs.h>
#include <ntddk.h>
#include "defines.h"
#include "undocumented.h"
#include "hook_handler.h"
#include "utility.h"

namespace driver
{
	NTSTATUS driver_entry();
	NTSTATUS init_hook();
}