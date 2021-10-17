#pragma once
#include <ntifs.h>

#define DbgMsg( ... ) DbgPrintEx(0, 0, __VA_ARGS__)

extern BOOLEAN isReady;
extern BOOLEAN isDone;
extern BOOLEAN trigger;