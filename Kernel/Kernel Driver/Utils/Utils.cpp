#include "Utils.h"

namespace DH
{
	namespace Utils
	{
		bool SetupHook(/*uint64_t GetAddrFunc, */PDRIVER_OBJECT DrvObj, PDRIVER_DISPATCH* Org, void* Hook)
		{
			//prep data
			PIMAGE_NT_HEADERS SysNtHead = (PIMAGE_NT_HEADERS)((PCHAR)DrvObj->DriverStart + ((PIMAGE_DOS_HEADER)DrvObj->DriverStart)->e_lfanew);
			PIMAGE_SECTION_HEADER Section = IMAGE_FIRST_SECTION(SysNtHead);
			//	ImpDef(strcmp);
			//	ImpSet(strcmp);

				//process sections
			for (WORD SectCnt = 0; SectCnt < SysNtHead->FileHeader.NumberOfSections; SectCnt++, Section++)
			{
				//find ".data" section
				if (!Fn::strcmpFn((const char*)Section->Name, ".data"))
				{
					//get section info
					uint8_t* HookAddr = (uint8_t*)((uint64_t)DrvObj->DriverStart + Section->VirtualAddress);

					MMPTE* pRegInfo = MiGetPteAddress(HookAddr);

					if (!pRegInfo->u.Hard.Valid)
						return false;

					//find place
					int CurSize = 0;
					uint8_t* Start = nullptr;
					for (int i = 0; i < 0x1000; i++)
					{
						//check byte
						if (!HookAddr[i])
						{
							if (!Start)
								Start = &HookAddr[i];

							CurSize++;
						}

						else
						{
							CurSize = 0;
							Start = nullptr;
						}
					}

					//place not found
					if (CurSize < 50)
						return false;

					//gen shellcode
					BYTE Call64[50] =
					{
						0x90, 0x90, 0x48, 0x89, 0x54, 0x24, 0x10, 0x48, 0x89, 0x4C, 0x24, 0x08,
						0x48, 0x83, 0xEC, 0x38, 0x48, 0xB8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
						0x00, 0x00, 0x48, 0x89, 0x44, 0x24, 0x20, 0x48, 0x8B, 0x54, 0x24, 0x48,
						0x48, 0x8B, 0x4C, 0x24, 0x40, 0xFF, 0x54, 0x24, 0x20, 0x48, 0x83, 0xC4,
						0x38, 0xC3
					};

					//write hook
					*(uint64_t*)(&Call64[18]) = (uint64_t)Hook;
					*Org = DrvObj->MajorFunction[IRP_MJ_DEVICE_CONTROL];
					pRegInfo->u.Hard.NoExecute = 0;
					/*Functions::memcpyFn*/Utils::MemCpy(Start, Call64, 50);
					DrvObj->MajorFunction[IRP_MJ_DEVICE_CONTROL] = (PDRIVER_DISPATCH)Start;
					return true;
				}
			}

			//not found
			return false;
		}


		void* GetSysVA_MDL(PMDL Mdl)
		{
			if (Mdl->MdlFlags & (MDL_MAPPED_TO_SYSTEM_VA | MDL_SOURCE_IS_NONPAGED_POOL))
				return Mdl->MappedSystemVa;
			else
				return Fn::MmMapLockedPagesSpecifyCacheFn(Mdl, KernelMode, MmCached, NULL, FALSE, HighPagePriority);
		}

		BYTE IsValidPtr(void* Addr, bool Write)
		{
			//get memory region info
			MEMORY_BASIC_INFORMATION MemInfo;

			if (!Fn::ZwQueryVirtualMemoryFn(ZwCurrentProcess(), Addr, MemoryBasicInformation, &MemInfo, sizeof(MEMORY_BASIC_INFORMATION), nullptr))
			{
				//read write check
				if ((MemInfo.State & MEM_COMMIT) && (!(MemInfo.Protect & (PAGE_NOACCESS | PAGE_GUARD))))
				{
					if (!Write || (MemInfo.Protect & (PAGE_READWRITE | PAGE_EXECUTE_READWRITE)))
						return (BYTE)1;
					else
						return (BYTE)2;
				}
			}

			//error
			return (BYTE)0;
		}

		void ForceWrite(void* Dst, void* Src, uint32_t Size)
		{
			PMDL Mdl = Fn::IoAllocateMdlFn(Dst, Size, false, false, nullptr);
			Fn::MmProbeAndLockPagesFn(Mdl, KernelMode, IoReadAccess);
			Dst = Fn::MmMapLockedPagesSpecifyCacheFn(Mdl, KernelMode, MmCached, nullptr, false, HighPagePriority);
			Fn::MmProtectMdlSystemAddressFn(Mdl, PAGE_READWRITE);
			Fn::memcpyFn(Dst, Src, Size);
			Fn::MmUnmapLockedPagesFn(Dst, Mdl);
			Fn::MmUnlockPagesFn(Mdl);
			Fn::IoFreeMdlFn(Mdl);
		}

		BOOL CheckMask(PCHAR base, PCHAR pattern, PCHAR mask)
		{
			for (; *mask; ++base, ++pattern, ++mask) 
			{
				if (*mask == 'x' && *base != *pattern)
				{
					return FALSE;
				}
			}

			return TRUE;
		}

		PVOID FindPattern(PCHAR base, DWORD length, PCHAR pattern, PCHAR mask) 
		{
			length -= (DWORD)strlen(mask);
			for (DWORD i = 0; i <= length; ++i) 
			{
				PVOID addr = &base[i];
				if (CheckMask((PCHAR)addr, pattern, mask))
				{
					return addr;
				}
			}

			return 0;
		}

		PVOID FindPatternImage(PCHAR base, PCHAR pattern, PCHAR mask)
		{
			PVOID match = 0;

			PIMAGE_NT_HEADERS headers = (PIMAGE_NT_HEADERS)(base + ((PIMAGE_DOS_HEADER)base)->e_lfanew);
			PIMAGE_SECTION_HEADER sections = IMAGE_FIRST_SECTION(headers);
			for (DWORD i = 0; i < headers->FileHeader.NumberOfSections; ++i)
			{
				PIMAGE_SECTION_HEADER section = &sections[i];
				if (*(PINT)section->Name == 'EGAP' || memcmp(section->Name, ".text", 5) == 0) 
				{
					match = FindPattern(base + section->VirtualAddress, section->Misc.VirtualSize, pattern, mask);
					if (match)
					{
						break;
					}
				}
			}

			return match;
		}

	}
}