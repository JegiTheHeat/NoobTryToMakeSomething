// dllmain.cpp : Defines the entry point for the DLL application.
#include "stdafx.h"
#include <iostream>
#include "mem.h"

DWORD tilt = 0x00C84FE0;
DWORD strafe = 0x00C84FDC;
class keyState
{
    int key;
    bool pressed;
    public:
    keyState(int key)
        :key(key)
    {}
    bool isPressed()
    {
        return this->pressed;
    }
    void update()
    {
        this->pressed = GetAsyncKeyState(this->key);
    }
};

DWORD WINAPI HackThread(HMODULE hModule)
{
	//Create Console
	AllocConsole();
	FILE* f;
	freopen_s(&f, "CONOUT$", "w", stdout);

	std::cout << "Lefty Lex is king, Yes you are, Jegi approves!\n"; 

	uintptr_t moduleBase = (uintptr_t)GetModuleHandle(L"iw3mp.exe");

	//calling it with NULL also gives you the address of the .exe module
	moduleBase = (uintptr_t)GetModuleHandle(NULL);


    	float * p_angle = (float *) 0x00C84FDC;
    	int state = STATE_FIRSTTURN;
    	int rotating_to = ROTATE_LEFT;
    // register keys we want
    	keyState key_a('A');
    	keyState key_d('D');
    	keyState key_numpad1(VK_NUMPAD1);

    	bool b_rotate = true; // change if you want it to start disabled
    	float rotate_increment = 1.1;
    	float rotate_instant = 45.0;
	while (true)
	{
		Sleep(1);
		        /*
            lets get the key states first
            will refresh fast enough and we don't have to get them all the time
        */

        key_a.update();
        key_d.update();
        key_numpad1.update();

        key_a.print();
        if (key_numpad1.isPressed())
            b_rotate = !b_rotate;
        /*
            intitial state:
            STATE_FIRSTTURN : nothing happens till key is pressed
            waiting if something triggers the rest of the sequence
        */
        if (b_rotate)
        {
            switch (state)
            {
            case STATE_FIRSTTURN:
                if (key_a.isPressed())
                {
                    state = STATE_NEXTTURN;
                    *p_angle -= 45;
                    rotating_to = ROTATE_LEFT;
                }
                if (key_d.isPressed())
                {
                    state = STATE_NEXTTURN;
                    *p_angle += 45;
                    rotating_to = ROTATE_RIGHT;
                }
                break;
            case STATE_NEXTTURN:
                if (key_a.isPressed() && rotating_to == ROTATE_RIGHT)
                {
                    *p_angle -= 90;
                    rotating_to = ROTATE_LEFT;
                }
                if (key_d.isPressed() && rotating_to == ROTATE_LEFT)
                {
                    *p_angle += 90;
                    rotating_to = ROTATE_RIGHT;
                }
                break;
            default:
                break;
            }
            if(rotating_to != ROTATE_NONE)
            {
                if (rotating_to == ROTATE_LEFT && key_a.isPressed())
                    *p_angle -= rotate_increment;
                if (rotating_to == ROTATE_RIGHT && key_d.isPressed())
                    *p_angle += rotate_increment;
            }
        }

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
