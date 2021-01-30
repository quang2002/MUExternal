#include "process.h"

PLib::Process::Process(LPCSTR AppName)
{
    HWND hWnd = FindWindowA(NULL, AppName);
    if (hWnd)
    {
        DWORD pid; GetWindowThreadProcessId(hWnd, &pid);
        if (pid)
        {
            
        } else throw "pid null";
        
    }
    else throw "Couldn't find the window";
}