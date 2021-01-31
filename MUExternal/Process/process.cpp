#include "process.h"

PLib::Process::Process(LPCSTR AppName)
{
    try
    {
        this->Attach(AppName);
    }
    catch (const char * exception)
    {
        using namespace std;
        cout << exception << endl;
    }
}

BOOL PLib::Process::Attach(LPCSTR AppName)
{
    HWND hWnd = FindWindowA(NULL, AppName);
    if (hWnd)
    {
        DWORD pid; GetWindowThreadProcessId(hWnd, &pid);
        if (pid)
        {
            hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
            if (hProcess)
            {
                dwProcessId = pid;
                return TRUE;
            }
            else throw "Couldn't open process\n";
        }
        else throw "Couldn't get process id\n";

    }
    else throw "Couldn't find the window\n";
    return FALSE;
}

BOOL PLib::Process::Detach()
{
    this->dwProcessId = NULL;
    BOOL result = CloseHandle(this->hProcess);
    this->hProcess = NULL;
    return result;
}

BOOL PLib::Process::IsAttached()
{
    return (DWORD)this->hProcess;
}

DWORD PLib::Process::GetProcessID()
{
    return this->dwProcessId;
}

HANDLE PLib::Process::GetProcessHandle()
{
    return this->hProcess;
}
