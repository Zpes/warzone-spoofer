#pragma once
#include "defines.h"
#include "hook_handler.h"

namespace driver
{
	NTSTATUS driver_entry();
	NTSTATUS init_hook();
}