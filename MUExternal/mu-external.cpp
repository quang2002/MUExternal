#include "mu-external.h"
#include "Hacks/delay.h"


PLib::Process proc;
typedef PLib::Pointer<proc> pointer_t;

Hacks::Delay hack_delay(&proc);


DWORD __stdcall MU::External::MainThread(LPVOID)
{
	std::cout << "[!] Waiting For SS2.VN ..." << std::endl;

	do
	{
		try
		{
			proc.Attach("SS2.VN");
		}
		catch (const char* msg)
		{
			std::cout << msg << std::endl;
		}
		Sleep(500);
	} while (!proc.IsAttached());

	std::cout << "[!] Process ID: " << proc.GetProcessID() << std::endl;
	std::cout << "[!] Attached Successfully!" << std::endl;

	hack_delay.Init();

	for (char cmd[512] = "\0"; proc.IsAttached(); Sleep(10))
	{
		std::cout << "\n> ";
		std::cin.getline(cmd, 512);
		MU::External::UnpackCommand(cmd);
	}

	std::cout << "[!] Closed." << std::endl;
	proc.Detach();
	return FALSE;
}

DWORD MU::External::UnpackCommand(const char* cmd)
{
	std::vector<std::string> splited_cmd;
	std::string tmp = "";

	for (char* s = (char*)cmd;; s++)
	{
		if (*s != ' ' && *s != '\n' && *s != '\0') tmp += *s;
		else if (tmp.size())
		{
			splited_cmd.push_back(tmp);
			tmp.clear();
		}

		if (*s == '\0') break;
	}

	size_t para_count = splited_cmd.size();
	BOOL isExecuted = FALSE;
	switch (para_count)
	{
	case 1:
		if (!strcmp(splited_cmd[0].c_str(), "quit"))
		{
			proc.Detach();
			isExecuted = TRUE;
		}
		if (!strcmp(splited_cmd[0].c_str(), "clear"))
		{
			system("cls");
			isExecuted = TRUE;
		}
		if (!strcmp(splited_cmd[0].c_str(), "help"))
		{
			std::cout << "\n\
---PROGRAM------------------------------------------------------\n\
clear: Clear console\n\
help: Show this list\n\
quit: Close program\n\
---HACK-FEATURES------------------------------------------------\n\
delay <on|off>: hack delay\n\
			";
			isExecuted = TRUE;
		}

		if (!strcmp(splited_cmd[0].c_str(), "delay"))
		{
			std::cout << (hack_delay.GetCurrentState() ? "delay: on" : "delay: off");
		}
		break;
	case 2:
		if (!strcmp(splited_cmd[0].c_str(), "delay"))
		{
			if (!strcmp(splited_cmd[1].c_str(), "on") || !strcmp(splited_cmd[1].c_str(), "1"))
			{
				std::cout << "delay: on";
				// hack_delay.Enable();
				proc.Write<BYTE>((LPVOID)(0x400000 + 0x1DE16C + 2), 0x40);
			}
			else if (!strcmp(splited_cmd[1].c_str(), "off") || !strcmp(splited_cmd[1].c_str(), "0"))
			{
				std::cout << "delay: off";
				hack_delay.Disable();
			}
			else std::cout << "[!] Wrong arguments.\non, 1: enable\noff, 0: disable";
			std::cout << std::endl;
			isExecuted = TRUE;
		}
		break;
	default:
		break;
	}

	if (!isExecuted && para_count > 0)
		std::cout << "[!] Wrong command. Type \"help\" for a list of commands.";

	return 0;
}
