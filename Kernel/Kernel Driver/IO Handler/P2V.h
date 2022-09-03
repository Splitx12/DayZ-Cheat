#pragma once

#include "../Core/Core.h"

namespace DH
{
	namespace Utils
	{
		NTSTATUS R2P_Read(PEPROCESS pProcess, PVOID Address, PVOID AllocatedBuffer, SIZE_T size, SIZE_T* read);
		NTSTATUS R2P_Write(PEPROCESS pProcess, PVOID Address, PVOID AllocatedBuffer, SIZE_T size, SIZE_T* written);
	}
}