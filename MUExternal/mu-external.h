#pragma once
#include "stdafx.h"
#include "Process/process.h"

namespace MU
{
	class External
	{
	public:
		static DWORD WINAPI MainThread(LPVOID);
	private:
		static DWORD UnpackCommand(const char* cmd);
	};
}
