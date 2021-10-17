#pragma once
#include "globals.h"
#include "func-hook.h"
#include "Util.h"

NTSTATUS DriverEntry(PDRIVER_OBJECT pDriverObject, PUNICODE_STRING registryPath);
NTSTATUS DriverUnload(PDRIVER_OBJECT pDriverObject);