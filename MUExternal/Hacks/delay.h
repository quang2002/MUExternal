#ifndef __HACK_DELAY_H__
#define __HACK_DELAY_H__

#pragma once
#include "../Process/process.h"

namespace Hacks
{
	class Delay
	{
	public:
		Delay(PLib::Process* proc);

		BOOL Init();

		BOOL GetCurrentState();
		BOOL Enable();
		BOOL Disable();

	private:
		BOOL statement;
		PLib::Process* proc;
	};
}

#endif // !__HACK_DELAY_H__
