// dllmain.cpp : Defines the entry point for the DLL application.
#include "stdafx.h"
#include <iostream>
#include "mem.h"

#define STATE_FIRSTTURN 0
#define STATE_NEXTTURN 1
#define ROTATE_LEFT 2
#define ROTATE_RIGHT 1
#define ROTATE_NONE 0
#define leftKey GetAsyncKeyState('A')
#define rightKey GetAsyncKeyState('D')
#define forwardKey GetAsyncKeyState('W')
#define jumpKey GetAsyncKeyState(VK_SPACE)


DWORD strafe = 0x00C84FDC;
DWORD tiltaddy = 0x00C84FE0;
DWORD p_angle = 0x00C84FDC;

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

    std::cout << "Jegi is saying Hello.\n";
    std::cout << "Press Numpad1 for BrokenBot.\n";
    std::cout << "Press Numpad2 for Tilt.\n";

    uintptr_t moduleBase = (uintptr_t)GetModuleHandle(L"iw3mp.exe");

    //calling it with NULL also gives you the address of the .exe module
    moduleBase = (uintptr_t)GetModuleHandle(NULL);


    int state = STATE_FIRSTTURN;
    int rotating_to = ROTATE_LEFT;
    // register keys I want

    bool b_rotate = true; // change if you want it to start disabled
    float rotate_increment = 0.1; // Like i told you, i am out of my mind for now
    float rotate_instant = 40.0;



    bool tilt = true;

        while (true)
        {
        Sleep(1);
                if (b_rotate)
        {
            switch (state)
            {
            case STATE_FIRSTTURN:
                if (forwardKey && leftKey)
                {
                    state = STATE_NEXTTURN;
                    *(float*)p_angle += 45;
                    rotating_to = ROTATE_LEFT;
                }
                if (forwardKey && rightKey)
                {
                    state = STATE_NEXTTURN;
                    *(float*)p_angle -= 45;
                    rotating_to = ROTATE_RIGHT;
                }
                break;
            case STATE_NEXTTURN:
                if (forwardKey && leftKey && rotating_to == ROTATE_RIGHT)
                {
                    *(float*)p_angle += 45;
                    rotating_to = ROTATE_LEFT;
                }
                if (forwardKey && rightKey && rotating_to == ROTATE_LEFT)
                {
                    *(float*)p_angle -= 45;
                    rotating_to = ROTATE_RIGHT;
                }
                break;
            default:
                break;
            }
            if (rotating_to != ROTATE_NONE)
            {
                if (rotating_to == ROTATE_LEFT && leftKey)
                {
                    *(float*)p_angle += rotate_increment;
                }
                if (rotating_to == ROTATE_RIGHT && rightKey)
                {
                    *(float*)p_angle -= rotate_increment;
                }
            }
    }
        if (GetAsyncKeyState(VK_NUMPAD2) & 1)
        {
            Sleep(100);
            tilt = !tilt; // toggles it on and off
        }
        if (GetAsyncKeyState(VK_NUMPAD1))
        {
            b_rotate = !b_rotate;
        }
        if (leftKey && GetAsyncKeyState(VK_SHIFT) && tilt == true) //idk why this is getting error thats  why i said i messed up something :P
        {
            *(float*)tiltaddy = 90;
        }
        else if (rightKey && GetAsyncKeyState(VK_SHIFT) && tilt == true)
        {
            *(float*)tiltaddy = -90;
        }
        else if (GetAsyncKeyState(VK_SPACE) && tilt == true)
        {
            *(float*)tiltaddy = 0;
        }
    }
     // its is compiled need to test it out but rotations are fucked up completely, mostly first one when you are starting the jump

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
