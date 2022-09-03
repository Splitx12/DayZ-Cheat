#include "IO Header.h"

#define MM_ZERO_ACCESS 0
#define DATA_UNIQUE (0x1234)

BOOL SafeCopy(PVOID dest, PVOID src, SIZE_T size)
{
	SIZE_T returnSize = 0;
	if (NT_SUCCESS(DH::Fn::MmCopyVirtualMemoryFn(PsGetCurrentProcess(), src, PsGetCurrentProcess(), dest, size, KernelMode, &returnSize)) && returnSize == size) 
	{
		return TRUE;
	}

	return FALSE;
}

namespace DH
{
	CmdData::ModInfo_t BBGetUserModule(PEPROCESS pProcess, const char* ModName)
	{
		//32-Bit Process
		if (IoIs32bitProcess(nullptr))
		{
			//get & check peb
			PPEB32 pPeb32 = (PPEB32)Fn::PsGetProcessWow64ProcessFn(pProcess);
			if (!pPeb32 || !pPeb32->Ldr)
				goto FastExit;

			//search module
			for (PLIST_ENTRY32 pListEntry = (PLIST_ENTRY32)((PPEB_LDR_DATA32)pPeb32->Ldr)->InLoadOrderModuleList.Flink; pListEntry != &((PPEB_LDR_DATA32)pPeb32->Ldr)->InLoadOrderModuleList; pListEntry = (PLIST_ENTRY32)pListEntry->Flink)
			{
				PLDR_DATA_TABLE_ENTRY32 pEntry = CONTAINING_RECORD(pListEntry, LDR_DATA_TABLE_ENTRY32, InLoadOrderLinks);

				if (Utils::R::strcmp_r(ModName, (PWCH)pEntry->BaseDllName.Buffer, false))
					return { pEntry->DllBase, pEntry->SizeOfImage };
			}
		}
		else
		{
			//64-Bit Process
			PPEB pPeb = Fn::PsGetProcessPebFn(pProcess);
			if (!pPeb || !pPeb->Ldr)
				goto FastExit;

			//search module
			for (PLIST_ENTRY pListEntry = pPeb->Ldr->InLoadOrderModuleList.Flink; pListEntry != &pPeb->Ldr->InLoadOrderModuleList; pListEntry = pListEntry->Flink)
			{
				PLDR_DATA_TABLE_ENTRY pEntry = CONTAINING_RECORD(pListEntry, LDR_DATA_TABLE_ENTRY, InLoadOrderLinks);

				if (Utils::R::strcmp_r(ModName, pEntry->BaseDllName.Buffer, false))
					return { (ULONG64)pEntry->DllBase, pEntry->SizeOfImage };
			}
		}

		//failed
	FastExit:
		return CmdData::ModInfo_t{ 0, 0 };
	}

	enum IO_CMD : ULONG
	{
		CMD_NONE = 0x100000,
		CMD_READ_VM = 0x200000,
		CMD_WRITE_VM = 0x300000,
		CMD_WRITE_PROT = 0x400000,
		CMD_WRITE_RO_VM = 0x500000,
		CMD_GET_MOD_INFO = 0x600000,
		CMD_ALLOC_VM = 0x700000,
		CMD_FREE_VM = 0x800000,
		CMD_CHECKSTATUS = 0x900000
	};

	void Handler(CmdData::IOCOMMAND_t* Buff)
	{
		

		PEPROCESS Process = nullptr, CurProcess = IoGetCurrentProcess();

		if (!Fn::PsLookupProcessByProcessIdFn((HANDLE)Buff->ProcessID, &Process) && Process && !PsAcquireProcessExitSynchronization(Process))
		{
			size_t CopySize;

			switch (Buff->CommandID)
			{
				//read vm
			case CMD_READ_VM:
			{
				//Utils::R2P_Read(Process, (PVOID)Buff->Src, (PVOID)Buff->Dst, Buff->Size, nullptr);
				Fn::MmCopyVirtualMemoryFn(Process, Buff->Src, CurProcess, Buff->Dst, Buff->Size, MODE::UserMode, &CopySize);
			} break;

			//write vm
			case CMD_WRITE_VM:
			{
				//Utils::R2P_Write(Process, (PVOID)Buff->Dst, (PVOID)Buff->Src, Buff->Size, nullptr);
				Fn::MmCopyVirtualMemoryFn(CurProcess, Buff->Src, Process, Buff->Dst, Buff->Size, MODE::UserMode, &CopySize);
			} break;

			case CMD_GET_MOD_INFO:
			{
				//get mod base
				char ModName[512];
				CmdData::ModInfo_t ModData = { 0, 0 };
				SIZE_T StrSize = strlen((const char*)Buff->Src) + 1;

				if (StrSize <= 512)
				{
					//prep get modbase
					__movsb((PUCHAR)ModName, (PUCHAR)Buff->Src, StrSize);
					Fn::KeAttachProcessFn(Process);

					//get modbase
					ModData = BBGetUserModule(Process, ModName);
					Fn::KeDetachProcessFn();
				}

				*(CmdData::ModInfo_t*)Buff->Dst = ModData;
			} break;

			//alloc vm
			case CMD_ALLOC_VM:
			{
				Fn::KeAttachProcessFn(Process);

				//alloc usermode memory
				PVOID Base = nullptr;
				size_t AllocSize = SizeAlign(Buff->Size);
				Fn::ZwAllocateVirtualMemoryFn(ZwCurrentProcess(), &Base, 0, &AllocSize, MEM_COMMIT, PAGE_READWRITE);

				if (Base)
				{
					//pte patch
					for (size_t i = 0; i < AllocSize; i += PAGE_SIZE)
					{
						const auto Addr = (uintptr_t)Base + i;
						*(volatile uintptr_t*)(Addr);
						Utils::GetPteFromVA(Addr)->u.Hard.NoExecute = false;
					}

					
				}

				Fn::KeDetachProcessFn();

				//save alloc ptr
				*(PVOID*)Buff->Dst = Base;
			} break;

			case CMD_FREE_VM:
			{
				Fn::KeAttachProcessFn(Process);

				uint64_t SizeUL64 = SizeAlign(Buff->Size);

				if (!SizeUL64)
					Fn::ZwFreeVirtualMemoryFn(ZwCurrentProcess(), (void**)&Buff->Src, &SizeUL64, MEM_RELEASE);
				else
					Fn::ZwFreeVirtualMemoryFn(ZwCurrentProcess(), (void**)&Buff->Src, &SizeUL64, MEM_DECOMMIT);

				Fn::KeDetachProcessFn();

			} break;

			//check loaded
			case CMD_CHECKSTATUS:
			{
				*(DWORD64*)Buff->Dst = (DWORD64)((DWORD64)Buff->Src * (DWORD64)Buff->Src);
			} break;

			default:
			{
			} break;
			}

			//cleanup
			Fn::ObfDereferenceObjectFn(Process);
			Fn::PsReleaseProcessExitSynchronizationFn(Process);
		}
	}

	INT64 NTAPI EnumerateDebuggingDevicesHook(CmdData::IOCOMMAND_t* pBuff, PINT64 status) 
	{
		if (ExGetPreviousMode() != UserMode || !pBuff) 
		{
			return ((HKProc)G::EnumerateDebuggingDevicesOriginal)(pBuff, status);
		}

		// Can't use inline SEH for safe dereferences cause PG
		CmdData::IOCOMMAND_t SafeData = { 0 };
		if (!SafeCopy(&SafeData, pBuff, sizeof(CmdData::IOCOMMAND_t)) || SafeData.Unique != DATA_UNIQUE)
		{
			return ((HKProc)G::EnumerateDebuggingDevicesOriginal)(pBuff, status);
		}

		Handler(&SafeData);

		*status = STATUS_NOT_IMPLEMENTED;
		return 0;
	}
}