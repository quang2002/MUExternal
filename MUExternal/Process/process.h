#pragma once

#include <Windows.h>


namespace PLib
{
    class Process
    {
    public:
        Process() : hProcess(NULL) {
            
        }

        Process(LPCSTR AppName);

    private:
        HANDLE hProcess;
    };
}
