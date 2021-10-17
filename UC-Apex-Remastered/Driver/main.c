#include "main.h"
#pragma check_stack(off)
#pragma warning (disable : 4047 4024 4311 4716 4020 4152 4113)

NTSTATUS DriverEntry(PDRIVER_OBJECT pDriverObject, PUNICODE_STRING registryPath)
{
	UNREFERENCED_PARAMETER(pDriverObject);
	UNREFERENCED_PARAMETER(registryPath);

	//NtGdiDdDDIQueryStatistics
	HookFunction(&Hook, Harz4StrCrypt("JpC`e@`@@EMqanuOp]peope_o"));
	return STATUS_SUCCESS;
}

NTSTATUS DriverUnload(PDRIVER_OBJECT pDriverObject)
{
	UNREFERENCED_PARAMETER(pDriverObject);

	return STATUS_SUCCESS;
}