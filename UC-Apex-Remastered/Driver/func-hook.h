#pragma once
#include <ntifs.h>
#include <ntdef.h>
#include <windef.h>
#include "globals.h"
#include "structs.h"

enum
{
	INST_GETMODBASE = 1,
	INST_READ,
	INST_WRITE,
	INST_ISRUNNING
};

typedef struct
{
	ULONG srcPID;
	ULONG pID;
	UINT_PTR srcAddr;
	UINT_PTR targetAddr;
	ULONG size;
	PVOID response;
	ULONG instructionID;
	PCWSTR moduleName;
} KERNEL_REQUEST, * PKERNEL_REQUEST;

PVOID GetSystemModuleExport(LPCSTR modName, LPCSTR routineName);
PVOID GetSystemModBase(LPCSTR modName);
void WriteToReadOnly(PVOID dst, PVOID src, SIZE_T size);
void HookFunction(PVOID src, LPCSTR funcName);
NTSTATUS Hook(PVOID param);