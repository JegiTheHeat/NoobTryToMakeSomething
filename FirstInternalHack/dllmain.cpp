// dllmain.cpp : Defines the entry point for the DLL application.
#include "stdafx.h"
#include <iostream>
#include "mem.h"

DWORD tilt = 0x00C84FE0;
DWORD strafe = 0x00C84FDC;

DWORD WINAPI HackThread(HMODULE hModule)
{
	//Create Console
	AllocConsole();
	FILE* f;
	freopen_s(&f, "CONOUT$", "w", stdout);

	std::cout << "Jegi said Hi!\n"; 

	uintptr_t moduleBase = (uintptr_t)GetModuleHandle(L"iw3mp.exe");

	//calling it with NULL also gives you the address of the .exe module
	moduleBase = (uintptr_t)GetModuleHandle(NULL);

	bool bTilt = true;
	bool bStrafe = true;

	while (true)
	{
		Sleep(1);
		if (GetAsyncKeyState('A') && GetAsyncKeyState(VK_SHIFT) && bTilt == true)
		{
			*(float*)tilt = 90;
		}
		else if (GetAsyncKeyState('D') && GetAsyncKeyState(VK_SHIFT) && bTilt == true)
		{
			*(float*)tilt = -90;
		}
		else if (GetAsyncKeyState(VK_SPACE) && bTilt == true)
		{
			*(float*)tilt = 0;
		}

		//bTilt 
		if (GetAsyncKeyState(VK_NUMPAD3) & 1)
		{
			bTilt = !bTilt; // toggles it on and off
		}
	}	

		//bStrafe
	{
		Sleep(1);
		if (GetAsyncKeyState(VK_NUMPAD2) & 1)
			bStrafe = !bStrafe;
	}

	fclose(f);
	FreeConsole();
	FreeLibraryAndExitThread(hModule, 0);
	return 0;
}

BOOL APIENTRY DllMain(HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		CloseHandle(CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)HackThread, hModule, 0, nullptr));
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}
