#include "Common.h"
#include "skCrypt.h"
#define RELATIVE_ADDR(addr, size) ((PVOID)((PBYTE)(addr) + *(PINT)((PBYTE)(addr) + ((size) - (INT)sizeof(INT))) + (size)))

DWORD64 G::EnumerateDebuggingDevicesOriginal = 0x0;

NTSTATUS NTAPI DriverEntry(PVOID pParam1, PVOID pParam2)
{
	DH::Core::Initialize((DWORD64)pParam1); 
	
	PVOID addr = DH::Utils::FindPatternImage((PCHAR)DH::Core::dwKernelBase, skCrypt("\x48\x8B\x05\x00\x00\x00\x00\xE8\x00\x00\x00\x00\x8B\xC8\x85\xC0\x78\x40"), skCrypt("xxx????x????xxxxxx"));

	if (!addr)
	{
		
		return 12;
	}

	PVOID s = RELATIVE_ADDR(addr, 7);

	
	G::EnumerateDebuggingDevicesOriginal = (DWORD64)InterlockedExchangePointer((volatile PVOID*)s, (PVOID)DH::EnumerateDebuggingDevicesHook);
	

	return 0;
}
