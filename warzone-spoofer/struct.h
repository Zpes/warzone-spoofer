#pragma once
#include <windef.h>
#include <ntifs.h>
#include <ntdddisk.h>

typedef struct _IO_COMPLETION_STRUCT
{
	PVOID	oldContext;
	PIO_COMPLETION_ROUTINE	oldCompletionRoutine;
	PSTORAGE_DEVICE_DESCRIPTOR	requestBuffer;
	DWORD	OutBufferLength;
	DWORD	signature;

}IO_COMPLETION_STRUCT, * PIO_COMPLETION_STRUCT;