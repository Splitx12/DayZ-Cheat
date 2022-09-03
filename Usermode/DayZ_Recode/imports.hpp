#pragma once
#include <msxml.h>
#include <atomic>
#include <mutex>
#include <Windows.h>
#include <winternl.h>
#include <TlHelp32.h>
#include <cstdint>
#include <vector>
#include <random>
#include <memoryapi.h>
#include <xstring>
#include <string>
#include <thread>
#include <vector>
#include <chrono>
#include <iostream>
#include <string_view>
#include <dwmapi.h>
#include <xlocbuf>
#include <codecvt>
#include <xmmintrin.h>
#include <emmintrin.h>
#include "Driver.h"
#pragma comment(lib, "dwmapi.lib")
#pragma comment(lib, "ntdll.lib")

EXTERN_C
{
    NTSTATUS NTAPI NtQuerySystemInformation(IN SYSTEM_INFORMATION_CLASS SystemInformationClass,
    OUT PVOID SystemInformation,
    IN ULONG SystemInformationLength,
    OUT PULONG ReturnLength OPTIONAL
    );
}

