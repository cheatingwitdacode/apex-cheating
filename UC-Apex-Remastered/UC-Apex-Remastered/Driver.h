#pragma once
#pragma warning( disable : 4244 )
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <cstdint>
#include <iostream>
#include "XorStr.hpp"
#include <cstring>
#include <stdio.h>
#include <mutex>
#include "globals.h"

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

typedef uint64_t(_stdcall* NtGdiDdDDIQueryStatistics_t)(void*);

class KDriver
{
private:
	uintptr_t currentPID;
	uintptr_t targetPID;
	NtGdiDdDDIQueryStatistics_t NtGdiDdDDIQueryStatistics = (NtGdiDdDDIQueryStatistics_t)0;

public:
	uintptr_t GetModuleBase(uintptr_t pID, PCWSTR moduleName)
	{
		// set private pid variable to the passed pid so we dont need to pass it in the read and write function
		targetPID = pID;
		KERNEL_REQUEST modRequest; // setting up a kernel request struct
		modRequest.instructionID = INST_GETMODBASE;
		modRequest.pID = targetPID;
		modRequest.moduleName = moduleName;
		NtGdiDdDDIQueryStatistics(&modRequest);
		
		uintptr_t base = 0;
		base = reinterpret_cast<uintptr_t>(modRequest.response);
		return base;
	}

	bool Init()
	{
		// loading the library the hooked function is in and getting the address of it
		HMODULE win32uModule = LoadLibrary(xorstr_(L"win32u.dll"));
		if (!win32uModule) return false;
		NtGdiDdDDIQueryStatistics = (NtGdiDdDDIQueryStatistics_t)GetProcAddress(win32uModule, xorstr_("NtGdiDdDDIQueryStatistics"));
		currentPID = GetCurrentProcessId();

		if (!NtGdiDdDDIQueryStatistics || !currentPID) return false;

		return true;
	}

	bool ReadRaw(UINT_PTR readAddress, UINT_PTR targetAddress, ULONG size)
	{
		// setting up a request and calling the hook
		KERNEL_REQUEST rpmRequest;
		rpmRequest.instructionID = INST_READ;
		rpmRequest.srcPID = currentPID;
		rpmRequest.pID = targetPID;
		rpmRequest.srcAddr = readAddress;
		rpmRequest.targetAddr = targetAddress;
		rpmRequest.size = size;
		NtGdiDdDDIQueryStatistics(&rpmRequest);
		return true;
	}
	
	template<class type>
	bool Read(uintptr_t readAddress, type out, size_t length) {
		return ReadRaw(readAddress, (UINT_PTR)&out, length);
	}

	template<class type>
	type rpm(UINT_PTR readAddress)
	{
		type tmp;
		if (ReadRaw(readAddress, (UINT_PTR)&tmp, sizeof(type)))
			return tmp;
		else
			return { NULL };
	}

	template<class type>
	bool wpm(UINT_PTR writeAddress, const type& value)
	{
		// setting up a request and calling the hook
		KERNEL_REQUEST wpmRequest;
		wpmRequest.instructionID = INST_WRITE;
		wpmRequest.srcPID = currentPID;
		wpmRequest.pID = targetPID;
		wpmRequest.srcAddr = (UINT_PTR)&value;
		wpmRequest.targetAddr = writeAddress;
		wpmRequest.size = sizeof(value);
		NtGdiDdDDIQueryStatistics(&wpmRequest);
		return true;
	}

	std::string ReadString(uintptr_t addr, int maxLength = 1000)
	{
		std::string str = "";
		std::vector<char> chars(maxLength);
		ReadRaw(addr, (uintptr_t)chars.data(), maxLength);
		
		// Loop through all the characters untill we find a null byte indicating the end of a string
		for (size_t i = 0; i < chars.size(); i++) {
			if (chars[i] == '\0') break; 
			str.push_back(chars[i]);
		}

		// Prevent strings with only a null byte in it
		if ((int)str[0] == 0 && str.size() == 1) return "";
		return str;
	}

	template<class type>
	bool ReadArray(uintptr_t address, type* array, size_t length)
	{
		return Read(address, &array, sizeof(type) * length);
	}
};