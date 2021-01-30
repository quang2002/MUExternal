#pragma once

#include <Windows.h>


namespace PLib
{
    class Process
    {
    public:
        Process() : hProcess(NULL) {
            
        }
    private:
        HANDLE hProcess;
    };
}
