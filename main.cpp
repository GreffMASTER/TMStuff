#include "main.h"
#include "mainthread.h"
#include "minhook/include/MinHook.h"

FILE* f;

extern "C" DLL_EXPORT BOOL APIENTRY DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{

    switch (fdwReason)
    {
        case DLL_PROCESS_ATTACH:
            AllocConsole();
            freopen_s(&f, "CONOUT$", "w", stdout);
            printf("MH_Initialize...");
            if (MH_Initialize() != MH_OK)
            {
                printf("FAIL!\n");
                MessageBoxA(NULL, "MinHook Initialize Failed", "MinHook Error", MB_OK | MB_ICONERROR);
                fclose(f);
                FreeConsole();
                return FALSE;
            }
            printf("OK!\n");
            printf("InitHooksPre...");
            if(InitHooksPre()) // 1 = fail
            {
                printf("FAIL!\n");
                MessageBoxA(NULL, "Failed to hook Pre-Init functions", "MinHook Error", MB_OK | MB_ICONERROR);
                fclose(f);
                FreeConsole();
                return FALSE;
            }
            printf("OK!\n");
            printf("Creating TMStuff thread...\n");
            CreateThread(0, 0, MainThread, hinstDLL, 0, 0);
            // attach to process
            // return FALSE to fail DLL load
            break;
        case DLL_PROCESS_DETACH:
            UninitHooks();
            fclose(f);
            FreeConsole();

            // detach from process
            break;

        case DLL_THREAD_ATTACH:
            // attach to thread
            break;

        case DLL_THREAD_DETACH:
            // detach from thread
            break;
    }
    return TRUE; // succesful
}


