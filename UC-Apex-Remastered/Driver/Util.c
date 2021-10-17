#include "Util.h"

#define KEY 4

const char* Harz4StrCrypt(char str[])
{
	for (int i = 0; i < strlen(str); i++)
		str[i] += KEY;

	return str;
}

const wchar_t* Harz4StrCryptW(wchar_t str[])
{
	for (int i = 0; i < wcslen(str); i++)
		str[i] += KEY;

	return str;
}