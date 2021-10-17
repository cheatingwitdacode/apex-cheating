#pragma once
#include <ntifs.h>

#pragma warning (disable : 4214 4201)

typedef struct _LDR_DATA_TABLE_ENTRY
{
	LIST_ENTRY InLoadOrderLinks;
	LIST_ENTRY InMemoryOrderLinks;
	LIST_ENTRY InInitializationOrderLinks;
	PVOID DllBase;
	PVOID EntryPoint;
	ULONG SizeOfImage;
	UNICODE_STRING FullDllName;
	UNICODE_STRING BaseDllName;
	// ...
} LDR_DATA_TABLE_ENTRY, * PLDR_DATA_TABLE_ENTRY;

typedef struct _PEB_LDR_DATA
{
	unsigned int Length;
	int Initialized;
	void* SSHandle;
	LIST_ENTRY InLoadOrderLinks;
	LIST_ENTRY InMemoryOrderLinks;
	LIST_ENTRY InInitializationOrderLinks;
	// ...
} PEB_LDR_DATA, * PPEB_LDR_DATA;

typedef struct _PEB64
{
	unsigned char InheritedAddressSpace;	// 0x0000 
	unsigned char ReadImageFileExecOptions;	// 0x0001 
	unsigned char BeingDebugged;			// 0x0002 
	unsigned char BitField;					// 0x0003 
	unsigned char pad_0x0004[0x4];			// 0x0004
	PVOID Mutant;							// 0x0008 
	PVOID ImageBaseAddress;					// 0x0010 
	PPEB_LDR_DATA Ldr;						// 0x0018
	// ...
} PEB64, * PPEB64;

typedef struct _RTL_PROCESS_MODULE_INFORMATION
{
	HANDLE Section;
	PVOID MappedBase;
	PVOID ImageBase;
	ULONG ImageSize;
	ULONG Flags;
	USHORT LoadOrderIndex;
	USHORT InitOrderIndex;
	USHORT LoadCount;
	USHORT OffsetToFileName;
	UCHAR  FullPathName[MAXIMUM_FILENAME_LENGTH];
} RTL_PROCESS_MODULE_INFORMATION, * PRTL_PROCESS_MODULE_INFORMATION;

typedef struct _RTL_PROCESS_MODULES
{
	ULONG NumberOfModules;
	RTL_PROCESS_MODULE_INFORMATION Modules[1];
} RTL_PROCESS_MODULES, * PRTL_PROCESS_MODULES;

typedef enum _SYSTEM_INFORMATION_CLASS
{
	SystemModuleInformation = 0xb,
} SYSTEM_INFORMATION_CLASS;