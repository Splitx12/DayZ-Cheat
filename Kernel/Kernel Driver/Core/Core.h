#pragma once
#include "../Common.h"

namespace DH
{
	namespace Fn
	{
		using MmGetSystemRoutineAddress__ = PVOID(NTAPI*)(_In_ PUNICODE_STRING);
		extern MmGetSystemRoutineAddress__ MmGetSystemRoutineAddressFn;

		using DbgPrint__ = ULONG(__cdecl*)(PCSTR, ...);
		extern DbgPrint__ DbgPrintFn;

		using RtlCaptureContext__ = NTSYSAPI VOID(NTAPI*)(_Out_ PCONTEXT);
		extern RtlCaptureContext__ RtlCaptureContextFn;

		using ExAllocatePool__ = PVOID(*)(POOL_TYPE, SIZE_T);
		extern ExAllocatePool__ ExAllocatePoolFn;

		using KeCapturePersistentThreadState__ = NTKERNELAPI ULONG(__stdcall*)(IN PCONTEXT, IN PKTHREAD, IN ULONG, IN ULONG, IN ULONG, IN ULONG, IN ULONG, OUT PVOID);
		extern KeCapturePersistentThreadState__ KeCapturePersistentThreadStateFn;

		using memcpy__ = void* (__cdecl*)(_Out_writes_bytes_all_(_MaxCount) void* _Dst, _In_reads_bytes_(_MaxCount) const void* _Src, _In_ size_t _MaxCount);
		extern memcpy__ memcpyFn;

		using ExFreePoolWithTag__ = NTKERNELAPI VOID(__cdecl*)(PVOID, _In_ ULONG);
		extern ExFreePoolWithTag__ ExFreePoolWithTagFn;

		using IoGetDeviceObjectPointer__ = NTKERNELAPI NTSTATUS(__cdecl*)(PUNICODE_STRING, ACCESS_MASK, PFILE_OBJECT*, PDEVICE_OBJECT*);
		extern IoGetDeviceObjectPointer__ IoGetDeviceObjectPointerFn;

		using strcmp__ = int(__cdecl*)(const char*, const char*);
		extern strcmp__ strcmpFn;

		using MmMapLockedPagesSpecifyCache__ = PVOID(__cdecl*)(PMDL, __drv_strictType(KPROCESSOR_MODE / enum _MODE, __drv_typeConst)KPROCESSOR_MODE, __drv_strictTypeMatch(__drv_typeCond)MEMORY_CACHING_TYPE, PVOID, ULONG, ULONG);
		extern MmMapLockedPagesSpecifyCache__ MmMapLockedPagesSpecifyCacheFn;

		using PsLookupProcessByProcessId__ = NTSTATUS(__cdecl*)(HANDLE, PEPROCESS*);
		extern PsLookupProcessByProcessId__ PsLookupProcessByProcessIdFn;

		using KeAttachProcess__ = void(__cdecl*)(PRKPROCESS);
		extern KeAttachProcess__ KeAttachProcessFn;

		using ZwQueryVirtualMemory__ = NTSYSAPI NTSTATUS(__cdecl*)(HANDLE, PVOID, MEMORY_INFORMATION_CLASS, PVOID, SIZE_T, PSIZE_T);
		extern ZwQueryVirtualMemory__ ZwQueryVirtualMemoryFn;

		using ZwAllocateVirtualMemory__ = NTSYSAPI NTSTATUS(__cdecl*)(HANDLE, PVOID*, ULONG_PTR, PSIZE_T, ULONG, ULONG);
		extern ZwAllocateVirtualMemory__ ZwAllocateVirtualMemoryFn;

		using ZwFreeVirtualMemory__ = NTSYSAPI NTSTATUS(__cdecl*)(HANDLE, PVOID*, PSIZE_T, ULONG);
		extern ZwFreeVirtualMemory__ ZwFreeVirtualMemoryFn;

		using ZwProtectVirtualMemory__ = NTSYSAPI NTSTATUS(__cdecl*)(IN HANDLE, IN OUT PVOID*, IN OUT SIZE_T*, IN ULONG, OUT PULONG);
		extern ZwProtectVirtualMemory__ ZwProtectVirtualMemoryFn;

		using IoAllocateMdl__ = PMDL(__cdecl*)(__drv_aliasesMem PVOID, ULONG, BOOLEAN, BOOLEAN, PIRP);
		extern IoAllocateMdl__ IoAllocateMdlFn;

		using MmProbeAndLockPages__ = void(__cdecl*)(PMDL, KPROCESSOR_MODE, LOCK_OPERATION);
		extern MmProbeAndLockPages__ MmProbeAndLockPagesFn;

		using MmProtectMdlSystemAddress__ = NTSTATUS(__cdecl*)(PMDL, ULONG);
		extern MmProtectMdlSystemAddress__ MmProtectMdlSystemAddressFn;

		using MmUnmapLockedPages__ = void(__cdecl*)(PVOID, PMDL);
		extern MmUnmapLockedPages__ MmUnmapLockedPagesFn;

		using MmUnlockPages__ = void(__cdecl*)(PMDL);
		extern MmUnlockPages__ MmUnlockPagesFn;

		using IoFreeMdl__ = void(__cdecl*)(PMDL);
		extern IoFreeMdl__ IoFreeMdlFn;

		using PsGetProcessPeb__ = NTKERNELAPI PPEB(NTAPI*)(PEPROCESS);
		extern PsGetProcessPeb__ PsGetProcessPebFn;

		using PsGetProcessWow64Process__ = NTKERNELAPI PVOID(NTAPI*)(PEPROCESS);
		extern PsGetProcessWow64Process__ PsGetProcessWow64ProcessFn;

		using _wcsicmp__ = int(__cdecl*)(_In_z_ const wchar_t* _Str1, _In_z_ const wchar_t* _Str2);
		extern _wcsicmp__ _wcsicmpFn;

		using KeDetachProcess__ = void(__cdecl*)();
		extern KeDetachProcess__ KeDetachProcessFn;

		using ObfDereferenceObject__ = NTKERNELAPI LONG_PTR(FASTCALL*)(_In_ PVOID);
		extern ObfDereferenceObject__ ObfDereferenceObjectFn;

		using IofCompleteRequest__ = void(__cdecl*)(PIRP, CCHAR);
		extern IofCompleteRequest__ IofCompleteRequestFn;

		using MmIsAddressValid__ = NTKERNELAPI BOOLEAN(__cdecl*)(_In_ PVOID);
		extern MmIsAddressValid__ MmIsAddressValidFn;

		using RtlGetVersion__ = NTSTATUS(NTAPI*)(PRTL_OSVERSIONINFOW);
		extern RtlGetVersion__ RtlGetVersionFn;

		using ZwOpenProcess__ = NTSYSAPI NTSTATUS(NTAPI*)(PHANDLE, ACCESS_MASK, POBJECT_ATTRIBUTES, PCLIENT_ID);
		extern ZwOpenProcess__ ZwOpenProcessFn;

		using ZwTerminateProcess__ = NTSYSAPI NTSTATUS(NTAPI*)(HANDLE, NTSTATUS);
		extern ZwTerminateProcess__ ZwTerminateProcessFn;

		using ZwClose__ = NTSYSAPI NTSTATUS(NTAPI*)(HANDLE);
		extern ZwClose__ ZwCloseFn;

		using RtlCreateUserThread__ = NTSTATUS(NTAPI*)(IN HANDLE, IN PSECURITY_DESCRIPTOR, IN BOOLEAN, IN ULONG, IN OUT PULONG, IN OUT PULONG, IN PVOID, IN PVOID, OUT PHANDLE, OUT PCLIENT_ID);
		extern RtlCreateUserThread__ RtlCreateUserThreadFn;

		using ZwSetSystemInformation_ = NTSTATUS(NTAPI*)(SYSTEM_INFORMATION_CLASS, PVOID, ULONG);
		extern ZwSetSystemInformation_ ZwSetSystemInformationFn;

		using MmCopyVirtualMemory__ = NTSTATUS(NTAPI*)(PEPROCESS, PVOID, PEPROCESS, PVOID, SIZE_T, KPROCESSOR_MODE, PSIZE_T);
		extern MmCopyVirtualMemory__ MmCopyVirtualMemoryFn;

		using MmMapLockedPages__ = PVOID(*)(PMDL, __drv_strictType(KPROCESSOR_MODE / enum _MODE, __drv_typeConst)KPROCESSOR_MODE);
		extern MmMapLockedPages__ MmMapLockedPagesFn;

		using PsReleaseProcessExitSynchronization__ = NTSTATUS(NTAPI*)(__in PEPROCESS);
		extern PsReleaseProcessExitSynchronization__ PsReleaseProcessExitSynchronizationFn;

		using ZwQuerySystemInformation__ = NTSTATUS(WINAPI*)(_In_ SYSTEM_INFORMATION_CLASS, _Inout_ PVOID, _In_ ULONG, _Out_opt_ PULONG);
		extern ZwQuerySystemInformation__ ZwQuerySystemInformationFn;

		using PsGetThreadTeb__ = PVOID(NTAPI*)(PETHREAD);
		extern PsGetThreadTeb__ PsGetThreadTebFn;
	}

	namespace L
	{
		enum EVerShort
		{
			WinUnsupported, // Unsupported OS 
			WinXP,          // Windows XP
			Win7,           // Windows 7
			Win8,           // Windows 8
			Win8Point1,     // Windows 8.1
			Win10,          // Windows 10
			Win10_RS1,      // Windows 10 Anniversary update
			Win10_RS2,      // Windows 10 Creators update
			Win10_RS3,      // Windows 10 Fall Creators update
			Win10_RS4,      // Windows 10 Spring Creators update
			Win10_RS5,      // Windows 10 October 2018 update
			Win10_19H1,     // Windows 10 May 2019 update
			Win10_19H2,     // Windows 10 November 2019 update
			Win10_20H1,     // Windows 10 April 2020 update
		};

		enum EBuildThreshold
		{
			Build_RS0 = 10586,
			Build_RS1 = 14393,
			Build_RS2 = 15063,
			Build_RS3 = 16299,
			Build_RS4 = 17134,
			Build_RS5 = 17763,
			Build_19H1 = 18362,
			Build_19H2 = 18363,
			Build_20H1 = 19041,
			Build_RS_MAX = 99999,
		};
	}

	namespace Core
	{
		void Initialize(IN DWORD64 KernelBase);

		PVOID GetProcAdress(DWORD64 ModBase, const char* Name);

		template <typename T>
		__forceinline PMMPTE GetPteFromVA(T VA)
		{
			return (PMMPTE)((ULONG64)Core::dwPTEBase + (((ULONG64)(VA) >> 9) & 0x7FFFFFFFF8ull));
		}

		void InitializeDebuggerBlock();
		L::EVerShort GetWinVer();

		extern L::EVerShort WinVer;
		extern DWORD64 dwKernelBase;
		extern DWORD64 dwPTEBase;
		extern KDDEBUGGER_DATA64 DebuggerBlock;
		extern PDRIVER_DISPATCH OrgCtrl;
	}
}

#define MiGetPteAddress(VA) ((PMMPTE)(DH::Core::dwPTEBase + (((uint64_t)(VA) >> 9) & 0x7FFFFFFFF8ull)))