#include <windows.h>
#include "TextArea.h"

static TextArea* objptr;

BOOL APIENTRY DllMain(
    HMODULE hModule,
    DWORD ul_reason_for_call,
    LPVOID lpReserved)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        // Since this dll acts as singleton pattern, instance is constructed at process attached.
        objptr = new TextArea();
        break;
    case DLL_THREAD_ATTACH:
        // NOP
        break;
    case DLL_THREAD_DETACH:
        // NOP, resource will be released during unload DLL
        break;
    case DLL_PROCESS_DETACH:
        if (objptr != NULL)
        {
            delete objptr;
            objptr = NULL;
        }
        break;
    default:
        break;
    }
    return TRUE;
    UNREFERENCED_PARAMETER(hModule);
    UNREFERENCED_PARAMETER(lpReserved);
}


DLLAPI TextArea* __stdcall GetControlBlock()
{
    return objptr;
}
