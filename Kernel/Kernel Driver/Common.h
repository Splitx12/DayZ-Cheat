#pragma once
#pragma comment(linker, "/MERGE:.pdata=INIT")

#define DEBUG 0
//#define _NO_CRT_STDIO_INLINE
#define SizeAlign(Size) ((Size + 0xFFF) & 0xFFFFFFFFFFFFF000)
#define _FI __forceinline
#define ToLower(Char) ((Char >= 'A' && Char <= 'Z') ? (Char + 32) : Char)

//#include <ntddk.h>
#include <ntifs.h>
#include <Ntstrsafe.h>
#include <intrin.h>
#include <windef.h>

#include "Core/Defenitions.h"
#include "Core/Core.h"
#include "IO Handler/IO Header.h"
#include "Utils/Utils.h"
#include "IO Handler/P2V.h"

#define NT_HEADER(ModBase) (PIMAGE_NT_HEADERS)((ULONG64)(ModBase) + ((PIMAGE_DOS_HEADER)(ModBase))->e_lfanew)
#define IMAGE_FIRST_SECTION(NtHeader) (PIMAGE_SECTION_HEADER)(NtHeader + 1)

#define KDDEBUGGER_DATA_OFFSET 0x2080
#define DUMP_BLOCK_SIZE 0x40000

// https://ntdiff.github.io/
#define WINDOWS_1803 17134
#define WINDOWS_1809 17763
#define WINDOWS_1903 18362
#define WINDOWS_1909 18363
#define WINDOWS_2004 19041
#define WINDOWS_20H2 19569
#define WINDOWS_21H1 20180

#define PAGE_OFFSET_SIZE 12

using HKProc = INT64(NTAPI*)(PVOID, PVOID);

namespace G
{
	extern DWORD64 EnumerateDebuggingDevicesOriginal;
}

extern "C"
{
	NTKERNELAPI PPEB PsGetProcessPeb(PEPROCESS);
	NTKERNELAPI PVOID PsGetProcessWow64Process(PEPROCESS Process);
	NTKERNELAPI void PsReleaseProcessExitSynchronization(PEPROCESS);
	NTKERNELAPI NTSTATUS PsAcquireProcessExitSynchronization(PEPROCESS);
	NTKERNELAPI NTSTATUS KeUserModeCallback(ULONG, PVOID, ULONG, PVOID*, PULONG);
	NTSYSAPI NTSTATUS ZwProtectVirtualMemory(HANDLE, PVOID*, SIZE_T*, ULONG, PULONG);
	NTKERNELAPI NTSTATUS ZwQuerySystemInformation(SYSTEM_INFORMATION_CLASS, PVOID, ULONG, PULONG);
	NTSYSAPI NTSTATUS RtlCreateUserThread(HANDLE, PVOID, BOOLEAN, ULONG, SIZE_T, SIZE_T, PVOID, PVOID, PHANDLE, PCLIENT_ID);
	NTSYSAPI PVOID NTAPI RtlAvlRemoveNode(IN PRTL_AVL_TREE pTree, IN PMMADDRESS_NODE pNode);
}