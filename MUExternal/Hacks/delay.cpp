#include "delay.h"

#define MAIN_EXE 0x400000

LPVOID hook_adr = (LPVOID)(MAIN_EXE + 0x1DE16C);
LPVOID return_adr = (LPVOID)(MAIN_EXE + 0x1DE16C + 11);
LPVOID hook_fn_adr = NULL;

__declspec(naked) void hook_fn()
{
	__asm {
		; mov[eax + 0x12], 0x3F800000
		mov ax, [eax + 0x12]
		mov[edx + 0xA0], cx
		ret
	}
}

Hacks::Delay::Delay(PLib::Process* proc) : proc(proc), statement(FALSE)
{
	
}

BOOL Hacks::Delay::Init()
{
	DWORD old_protection;
	hook_fn_adr = VirtualAllocEx(proc->GetProcessHandle(), NULL, 128, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
	std::cout << std::hex << hook_fn_adr;
	VirtualProtect(hook_fn, 128, PAGE_EXECUTE_READWRITE, &old_protection);
	proc->WriteAOB(hook_fn_adr, hook_fn, 128);
	VirtualProtect(hook_fn, 128, old_protection, &old_protection);
	return 0;
}

BOOL Hacks::Delay::GetCurrentState()
{
	return this->statement;
}

BOOL Hacks::Delay::Enable()
{
	this->statement = TRUE;
	DWORD old_protection;
	BYTE buffer[11] = {
		0xE8, 0x00, 0x00, 0x00, 0x00,
		0x90, 0x90, 0x90, 0x90, 0x90, 0x90
	};

	VirtualProtectEx(proc->GetProcessHandle(), hook_adr, 11, PAGE_EXECUTE_READWRITE, &old_protection);

	*(DWORD*)(buffer + 1) = (DWORD)hook_fn_adr - (DWORD)hook_adr - 5;
	proc->WriteAOB(hook_adr, buffer, 11);

	VirtualProtectEx(proc->GetProcessHandle(), hook_adr, 11, old_protection, &old_protection);
	return 0;
}

BOOL Hacks::Delay::Disable()
{
	this->statement = FALSE;
	DWORD old_protection;
	BYTE buffer[11] = {
		0x66, 0x8B, 0x48, 0x12, 0x66, 0x89, 0x8A, 0xA0, 0x00, 0x00, 0x00
	};

	VirtualProtectEx(proc->GetProcessHandle(), hook_adr, 11, PAGE_EXECUTE_READWRITE, &old_protection);
	proc->Write<decltype(buffer)>(hook_adr, buffer);
	VirtualProtectEx(proc->GetProcessHandle(), hook_adr, 11, old_protection, &old_protection);
	return 0;
}
