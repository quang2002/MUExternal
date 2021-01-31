#pragma once

#include <Windows.h>
#include <iostream>

namespace PLib
{
	class Process
	{
	public:
		Process() : hProcess(NULL) {

		}

		Process(LPCSTR AppName);

		BOOL Attach(LPCSTR AppName);

		BOOL Detach();

		BOOL IsAttached();

		DWORD GetProcessID();

		HANDLE GetProcessHandle();

		BOOL WriteAOB(LPVOID Src, LPVOID Dst, SIZE_T Len)
		{
			return WriteProcessMemory(
				this->hProcess,
				Src,
				Dst,
				Len,
				NULL
			);
		}

		template <class _T>
		BOOL Write(LPVOID Address, _T Value)
		{
			return WriteProcessMemory(
				this->hProcess,
				Address,
				&Value,
				sizeof Value,
				NULL
			);
		}

		template <class _T>
		_T Read(LPVOID Address)
		{
			_T Value;
			ReadProcessMemory(
				this->hProcess,
				Address,
				&Value,
				sizeof Value,
				NULL
			);
			return Value;
		}

	private:
		HANDLE hProcess;
		DWORD dwProcessId;
	};


	template <Process& proc>
	class Pointer
	{
	public:
		Pointer() : base(NULL) {}

		Pointer(LPVOID base) : base((DWORD)base) {}

		Pointer(DWORD base) : base(base) {}

		operator DWORD&()
		{
			return base;
		}

		Pointer<proc> operator + (DWORD offset) CONST;
		Pointer<proc> operator - (DWORD offset) CONST;

		Pointer<proc>& operator += (CONST DWORD offset);
		Pointer<proc>& operator -= (CONST DWORD offset);

		template <class _T>
		Pointer<proc>& operator >> (_T& refva);

		template <class _T>
		Pointer<proc>& operator << (_T va);
	private:
		DWORD base;
	};

	template<Process& proc>
	inline Pointer<proc> Pointer<proc>::operator+(DWORD offset) CONST
	{
		return Pointer<proc>(base + offset);
	}

	template<Process& proc>
	inline Pointer<proc> Pointer<proc>::operator-(DWORD offset) CONST
	{
		return Pointer<proc>(base - offset);
	}

	template<Process& proc>
	inline Pointer<proc>& Pointer<proc>::operator+=(const DWORD offset)
	{
		*this = *this + offset;
		return *this;
	}

	template<Process& proc>
	inline Pointer<proc>& Pointer<proc>::operator-=(const DWORD offset)
	{
		*this = *this - offset;
		return *this;
	}

	template<Process& proc>
	template<class _T>
	inline Pointer<proc>& Pointer<proc>::operator>>(_T& refva)
	{
		refva = proc.Read<_T>((DWORD)base);
		base += sizeof refva;
		return *this;
	}

	template<Process& proc>
	template<class _T>
	inline Pointer<proc>& Pointer<proc>::operator<<(_T va)
	{
		proc.Write<_T>((DWORD)base, va);
		return *this;
	}

}
