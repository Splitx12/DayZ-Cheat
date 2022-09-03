#pragma once

#include "../Core/Core.h"

namespace DH
{
	namespace CmdData
	{
		struct IOCOMMAND_t
		{
			ULONG NameHash;
			ULONG ProcessID;
			ULONG CommandID;
			PVOID Src, Dst;
			ULONG Size;
			DWORD Unique;
		};

		struct ModInfo_t
		{
			ULONG64 ModBase;
			ULONG ModSize;
		};

		//enum IO_CMD : ULONG
		//{
		//	CMD_NONE = 0x100000,
		//	CMD_READ_VM = 0x200000,
		//	CMD_WRITE_VM = 0x300000,
		//	//CMD_WRITE_PROT = 0x400000,
		//	CMD_WRITE_RO_VM = 0x500000,
		//	CMD_GET_MOD_INFO = 0x600000,
		//	CMD_ALLOC_VM = 0x700000,
		//	CMD_FREE_VM = 0x800000,
		//	CMD_CHECKSTATUS = 0x900000
		//};

	}

	INT64 NTAPI EnumerateDebuggingDevicesHook(CmdData::IOCOMMAND_t* pBuff, PINT64 status);

	//NTSTATUS __cdecl hookHandler(CmdData::IOCOMMAND_t* pBuff);

}