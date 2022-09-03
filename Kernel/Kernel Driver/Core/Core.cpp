#include "Core.h"

namespace DH
{
	namespace Fn
	{
		MmGetSystemRoutineAddress__ MmGetSystemRoutineAddressFn = nullptr;
		DbgPrint__ DbgPrintFn = nullptr;
		RtlCaptureContext__ RtlCaptureContextFn = nullptr;
		ExAllocatePool__ ExAllocatePoolFn = nullptr;
		KeCapturePersistentThreadState__ KeCapturePersistentThreadStateFn = nullptr;
		memcpy__ memcpyFn = nullptr;
		ExFreePoolWithTag__ ExFreePoolWithTagFn = nullptr;
		IoGetDeviceObjectPointer__ IoGetDeviceObjectPointerFn = nullptr;
		strcmp__ strcmpFn = nullptr;
		MmMapLockedPagesSpecifyCache__ MmMapLockedPagesSpecifyCacheFn = nullptr;
		PsLookupProcessByProcessId__ PsLookupProcessByProcessIdFn = nullptr;
		KeAttachProcess__ KeAttachProcessFn = nullptr;
		ZwQueryVirtualMemory__ ZwQueryVirtualMemoryFn = nullptr;
		ZwAllocateVirtualMemory__ ZwAllocateVirtualMemoryFn = nullptr;
		IoAllocateMdl__ IoAllocateMdlFn = nullptr;
		ZwFreeVirtualMemory__ ZwFreeVirtualMemoryFn = nullptr;
		ZwProtectVirtualMemory__ ZwProtectVirtualMemoryFn = nullptr;
		MmProbeAndLockPages__ MmProbeAndLockPagesFn = nullptr;
		MmProtectMdlSystemAddress__ MmProtectMdlSystemAddressFn = nullptr;
		MmUnmapLockedPages__ MmUnmapLockedPagesFn = nullptr;
		MmUnlockPages__ MmUnlockPagesFn = nullptr;
		IoFreeMdl__ IoFreeMdlFn = nullptr;
		PsGetProcessPeb__ PsGetProcessPebFn = nullptr;
		PsGetProcessWow64Process__ PsGetProcessWow64ProcessFn = nullptr;
		_wcsicmp__ _wcsicmpFn = nullptr;
		KeDetachProcess__ KeDetachProcessFn = nullptr;
		ObfDereferenceObject__ ObfDereferenceObjectFn = nullptr;
		IofCompleteRequest__ IofCompleteRequestFn = nullptr;
		MmIsAddressValid__ MmIsAddressValidFn = nullptr;
		RtlGetVersion__ RtlGetVersionFn = nullptr;
		ZwOpenProcess__ ZwOpenProcessFn = nullptr;
		ZwTerminateProcess__ ZwTerminateProcessFn = nullptr;
		ZwClose__ ZwCloseFn = nullptr;
		RtlCreateUserThread__ RtlCreateUserThreadFn = nullptr;
		ZwSetSystemInformation_ ZwSetSystemInformationFn = nullptr;
		MmCopyVirtualMemory__ MmCopyVirtualMemoryFn = nullptr;
		MmMapLockedPages__ MmMapLockedPagesFn = nullptr;
		PsReleaseProcessExitSynchronization__ PsReleaseProcessExitSynchronizationFn = nullptr;
		ZwQuerySystemInformation__ ZwQuerySystemInformationFn = nullptr;
		PsGetThreadTeb__ PsGetThreadTebFn = nullptr;
	}

	void Core::Initialize(IN DWORD64 KernelBase)
	{
		// Set global ptrs
		dwKernelBase = KernelBase;
		
		Fn::DbgPrintFn = (Fn::DbgPrint__)Core::GetProcAdress(Core::dwKernelBase, "DbgPrint");
		Fn::RtlCaptureContextFn = (Fn::RtlCaptureContext__)Core::GetProcAdress(Core::dwKernelBase, "RtlCaptureContext");
		Fn::ExAllocatePoolFn = (Fn::ExAllocatePool__)Core::GetProcAdress(Core::dwKernelBase, "ExAllocatePool");
		Fn::KeCapturePersistentThreadStateFn = (Fn::KeCapturePersistentThreadState__)Core::GetProcAdress(Core::dwKernelBase, "KeCapturePersistentThreadState");
		Fn::memcpyFn = (Fn::memcpy__)Core::GetProcAdress(Core::dwKernelBase, "memcpy");
		Fn::ExFreePoolWithTagFn = (Fn::ExFreePoolWithTag__)Core::GetProcAdress(Core::dwKernelBase, "ExFreePoolWithTag");
		Fn::IoGetDeviceObjectPointerFn = (Fn::IoGetDeviceObjectPointer__)Core::GetProcAdress(Core::dwKernelBase, "IoGetDeviceObjectPointer");
		Fn::strcmpFn = (Fn::strcmp__)Core::GetProcAdress(Core::dwKernelBase, "strcmp");
		Fn::MmMapLockedPagesSpecifyCacheFn = (Fn::MmMapLockedPagesSpecifyCache__)Core::GetProcAdress(Core::dwKernelBase, "MmMapLockedPagesSpecifyCache");
		Fn::PsLookupProcessByProcessIdFn = (Fn::PsLookupProcessByProcessId__)Core::GetProcAdress(Core::dwKernelBase, "PsLookupProcessByProcessId");
		Fn::KeAttachProcessFn = (Fn::KeAttachProcess__)Core::GetProcAdress(Core::dwKernelBase, "KeAttachProcess");
		Fn::ZwQueryVirtualMemoryFn = (Fn::ZwQueryVirtualMemory__)Core::GetProcAdress(Core::dwKernelBase, "ZwQueryVirtualMemory");
		Fn::ZwAllocateVirtualMemoryFn = (Fn::ZwAllocateVirtualMemory__)Core::GetProcAdress(Core::dwKernelBase, "ZwAllocateVirtualMemory");
		Fn::ZwProtectVirtualMemoryFn = (Fn::ZwProtectVirtualMemory__)Core::GetProcAdress(Core::dwKernelBase, "ZwProtectVirtualMemory");
		Fn::IoAllocateMdlFn = (Fn::IoAllocateMdl__)Core::GetProcAdress(Core::dwKernelBase, "IoAllocateMdl");
		Fn::ZwFreeVirtualMemoryFn = (Fn::ZwFreeVirtualMemory__)Core::GetProcAdress(Core::dwKernelBase, "ZwFreeVirtualMemory");
		Fn::MmProbeAndLockPagesFn = (Fn::MmProbeAndLockPages__)Core::GetProcAdress(Core::dwKernelBase, "MmProbeAndLockPages");
		Fn::MmProtectMdlSystemAddressFn = (Fn::MmProtectMdlSystemAddress__)Core::GetProcAdress(Core::dwKernelBase, "MmProtectMdlSystemAddress");
		Fn::MmUnmapLockedPagesFn = (Fn::MmUnmapLockedPages__)Core::GetProcAdress(Core::dwKernelBase, "MmUnmapLockedPages");
		Fn::MmUnlockPagesFn = (Fn::MmUnlockPages__)Core::GetProcAdress(Core::dwKernelBase, "MmUnlockPages");
		Fn::IoFreeMdlFn = (Fn::MmUnlockPages__)Core::GetProcAdress(Core::dwKernelBase, "IoFreeMdl");
		Fn::PsGetProcessPebFn = (Fn::PsGetProcessPeb__)Core::GetProcAdress(Core::dwKernelBase, "PsGetProcessPeb");
		Fn::PsGetThreadTebFn = (Fn::PsGetThreadTeb__)Core::GetProcAdress(Core::dwKernelBase, "PsGetThreadTeb");
		Fn::PsGetProcessWow64ProcessFn = (Fn::PsGetProcessWow64Process__)Core::GetProcAdress(Core::dwKernelBase, "PsGetProcessWow64Process");
		Fn::_wcsicmpFn = (Fn::_wcsicmp__)Core::GetProcAdress(Core::dwKernelBase, "_wcsicmp");
		Fn::KeDetachProcessFn = (Fn::KeDetachProcess__)Core::GetProcAdress(Core::dwKernelBase, "KeDetachProcess");
		Fn::ObfDereferenceObjectFn = (Fn::ObfDereferenceObject__)Core::GetProcAdress(Core::dwKernelBase, "ObfDereferenceObject");
		Fn::IofCompleteRequestFn = (Fn::IofCompleteRequest__)Core::GetProcAdress(Core::dwKernelBase, "IofCompleteRequest");
		Fn::MmIsAddressValidFn = (Fn::MmIsAddressValid__)Core::GetProcAdress(Core::dwKernelBase, "MmIsAddressValid");
		Fn::RtlGetVersionFn = (Fn::RtlGetVersion__)Core::GetProcAdress(Core::dwKernelBase, "RtlGetVersion");
		Fn::ZwOpenProcessFn = (Fn::ZwOpenProcess__)Core::GetProcAdress(Core::dwKernelBase, "ZwOpenProcess");
		Fn::ZwTerminateProcessFn = (Fn::ZwTerminateProcess__)Core::GetProcAdress(Core::dwKernelBase, "ZwTerminateProcess");
		Fn::ZwCloseFn = (Fn::ZwClose__)Core::GetProcAdress(Core::dwKernelBase, "ZwClose");
		Fn::RtlCreateUserThreadFn = (Fn::RtlCreateUserThread__)Core::GetProcAdress(Core::dwKernelBase, "RtlCreateUserThread");
		Fn::ZwSetSystemInformationFn = (Fn::ZwSetSystemInformation_)Core::GetProcAdress(Core::dwKernelBase, "ZwSetSystemInformation");
		Fn::MmCopyVirtualMemoryFn = (Fn::MmCopyVirtualMemory__)Core::GetProcAdress(Core::dwKernelBase, "MmCopyVirtualMemory");
		Fn::MmMapLockedPagesFn = (Fn::MmMapLockedPages__)Core::GetProcAdress(Core::dwKernelBase, "MmMapLockedPages");
		Fn::PsReleaseProcessExitSynchronizationFn = (Fn::PsReleaseProcessExitSynchronization__)Core::GetProcAdress(Core::dwKernelBase, "PsReleaseProcessExitSynchronization");
		Fn::ZwQuerySystemInformationFn = (Fn::ZwQuerySystemInformation__)Core::GetProcAdress(Core::dwKernelBase, "ZwQuerySystemInformation");

		Core::InitializeDebuggerBlock();
		WinVer = Core::GetWinVer();

		if (WinVer > L::EVerShort::Win10_RS1)
		{
			Core::dwPTEBase = (DWORD64)Core::DebuggerBlock.PteBase;
		}
		else
		{
			Core::dwPTEBase = (DWORD64)0xFFFFF68000000000;
		}
	}

	PVOID Core::GetProcAdress(DWORD64 ModBase, const char* Name)
	{
		//parse headers
		PIMAGE_NT_HEADERS pNTH = NT_HEADER(ModBase);

		PIMAGE_EXPORT_DIRECTORY ExportDir = (PIMAGE_EXPORT_DIRECTORY)((ULONG64)ModBase + pNTH->OptionalHeader.DataDirectory[0].VirtualAddress);

		//process records
		for (ULONG i = 0; i < ExportDir->NumberOfNames; i++)
		{
			//get ordinal & name
			USHORT Ordinal = ((USHORT*)((ULONG64)ModBase + ExportDir->AddressOfNameOrdinals))[i];
			const char* ExpName = (const char*)ModBase + ((ULONG*)((ULONG64)ModBase + ExportDir->AddressOfNames))[i];

			//check export name
			if (Utils::R::stricmp_r(Name, ExpName, true))
			{
				PVOID RetExport = (PVOID)((ULONG64)ModBase + ((ULONG*)((ULONG64)ModBase + ExportDir->AddressOfFunctions))[Ordinal]);
				DbgPrint("%s = 0x%p [mod: 0x%p]\n", Name, RetExport, ModBase);
				return RetExport;
			}
		}

		DbgPrint("Failure to get export -> %s [mod: 0x%p]\n", Name, ModBase);
		//no export
		return nullptr;
	}


	L::EVerShort Core::GetWinVer()
	{
		RTL_OSVERSIONINFOW OsInfo;

		if (Fn::RtlGetVersionFn(&OsInfo) == STATUS_SUCCESS)
		{
			if (OsInfo.dwMajorVersion != 0)
			{
				ULONG fullver = (OsInfo.dwMajorVersion << 8) | OsInfo.dwMinorVersion;
				switch (fullver)
				{
				case _WIN32_WINNT_WIN10:
					if (OsInfo.dwBuildNumber >= L::EBuildThreshold::Build_20H1)
						return L::EVerShort::Win10_20H1;
					else if (OsInfo.dwBuildNumber >= L::EBuildThreshold::Build_20H1)
						return L::EVerShort::Win10_19H1;
					else if (OsInfo.dwBuildNumber >= L::EBuildThreshold::Build_RS5)
						return L::EVerShort::Win10_RS5;
					else if (OsInfo.dwBuildNumber >= L::EBuildThreshold::Build_RS4)
						return L::EVerShort::Win10_RS4;
					else if (OsInfo.dwBuildNumber >= L::EBuildThreshold::Build_RS3)
						return L::EVerShort::Win10_RS3;
					else if (OsInfo.dwBuildNumber >= L::EBuildThreshold::Build_RS2)
						return L::EVerShort::Win10_RS2;
					else if (OsInfo.dwBuildNumber >= L::EBuildThreshold::Build_RS1)
						return L::EVerShort::Win10_RS1;
					else if (OsInfo.dwBuildNumber >= L::EBuildThreshold::Build_RS0)
						return L::EVerShort::Win10;
					break;

				case _WIN32_WINNT_WINBLUE:
					return L::EVerShort::Win8Point1;
					break;

				case _WIN32_WINNT_WIN8:
					return L::EVerShort::Win8;
					break;

				case _WIN32_WINNT_WIN7:
					return L::EVerShort::Win7;
					break;

				case _WIN32_WINNT_WINXP:
					return L::EVerShort::WinXP;
					break;

				default:
					return L::EVerShort::WinUnsupported;
				}
			}

			
		}
		return L::EVerShort::WinUnsupported;
	}

	void Core::InitializeDebuggerBlock()
	{
		CONTEXT Context = { 0 };
		Context.ContextFlags = CONTEXT_FULL;
		Fn::RtlCaptureContextFn(&Context);

		DUMP_HEADER* pDumpHeader = (DUMP_HEADER*)Fn::ExAllocatePoolFn(NonPagedPool, DUMP_BLOCK_SIZE);

		if (pDumpHeader)
		{
		//	TRACE(CCore::ELogLevel::E_DBG, "DumpHeader: 0x%p\n", pDumpHeader);

			Fn::KeCapturePersistentThreadStateFn(&Context, NULL, 0, 0, 0, 0, 0, pDumpHeader);
			Fn::memcpyFn(&Core::DebuggerBlock, (PUCHAR)pDumpHeader + KDDEBUGGER_DATA_OFFSET, sizeof(KDDEBUGGER_DATA64));

			Fn::ExFreePoolWithTagFn(pDumpHeader, 0);
		}
		/*else
			TRACE(CCore::ELogLevel::E_DBG, "DumpHeader failure\n");*/
	}

	namespace Core
	{
		L::EVerShort WinVer = {};
		DWORD64 dwKernelBase = 0x0;
		DWORD64 dwPTEBase = 0x0;
		KDDEBUGGER_DATA64 DebuggerBlock = {};
		PDRIVER_DISPATCH OrgCtrl = {};
	}
}
