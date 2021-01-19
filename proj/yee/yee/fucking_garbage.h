#pragma once
#include <Windows.h>
#include <Psapi.h>
#include <string>

const char* refine_poynter(const char* a) {
	const char* last = a;
	for (;*a;a++)if (*a == '\\' || *a == '/')last = a+1;
	return last;
}

HMODULE GetModule(const char* modname, HANDLE process)
{
	HMODULE hMods[1024];
	DWORD cbNeeded;
	unsigned int i;

	if (EnumProcessModules(process, hMods, sizeof(hMods), &cbNeeded))
	{
		for (i = 0; i < (cbNeeded / sizeof(HMODULE)); i++)
		{
			TCHAR szModName[MAX_PATH];
			if (GetModuleFileNameEx(process, hMods[i], szModName, sizeof(szModName) / sizeof(TCHAR)))
			{
				if (!strcmp(refine_poynter(szModName), modname))
				{
					return hMods[i];
				}
			}
		}
	}
	return nullptr;
}