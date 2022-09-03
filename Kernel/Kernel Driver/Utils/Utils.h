#pragma once

#include "../Core/Core.h"

namespace DH
{
	namespace Utils
	{
		_FI void MemCpy(PVOID Dst, PVOID Src, ULONG Size)
		{
			__movsb((PUCHAR)Dst, (const PUCHAR)Src, Size);
		}

		_FI void MemZero(PVOID Ptr, SIZE_T Size, UCHAR Filling = 0)
		{
			__stosb((PUCHAR)Ptr, Filling, Size);
		}

		namespace R
		{
			template <typename StrType, typename StrType2>
			bool strstr_r(StrType Str, StrType2 InStr)
			{
				if (!InStr || !Str) return false;
				StrType CurPos = Str; for (int i = 0;; i++)
				{
					auto CharCur = *CurPos;
					if (!CharCur)
						return true;

					auto CharIn = InStr[i];

					if (!CharIn)
						break;

					if (ToLower(CharIn) != ToLower(CharCur))
						CurPos = Str;
					else
						CurPos++;

				}

				return false;
			}

			template <typename StrType, typename StrType2>
			__forceinline bool strcmp_r(StrType Str, StrType2 InStr, bool Two)
			{
				if (!Str || !InStr)
					return false;

				wchar_t c1, c2;
				do
				{
					c1 = *Str++;
					c2 = *InStr++;
					c1 = ToLower(c1);
					c2 = ToLower(c2);
					if (!c1 && (Two ? !c2 : 1))
						return true;

				} while (c1 == c2);

				return false;
			}

			template <typename StrType, typename StrType2>
			void strcpy_r(StrType Src, StrType2 Dst, wchar_t TNull = 0)
			{
				if (!Src || !Dst)
					return;

				while (true)
				{
					wchar_t WChar = *Dst = *Src++;
					if (WChar == TNull)
					{
						*Dst = 0;
						break;
					}

					Dst++;
				}
			}

			template <typename StrType>
			int strlen_r(StrType Str)
			{
				if (!Str)
					return 0;
				StrType Str2 = Str;
				while (*Str2)
					*Str2++;

				return (int)(Str2 - Str);
			}

			template <typename StrType, typename StrType2>
			__forceinline void strcat_r(StrType ToStr, StrType2 Str)
			{
				strcpy_r(Str, (StrType)&ToStr[StrLen(ToStr)]);
			}

			template <typename StrType, typename StrType2>
			__forceinline bool stricmp_r(StrType Str, StrType2 InStr, bool Two)
			{
				if (!Str || !InStr)
					return false;
				wchar_t c1, c2;

				do
				{
					c1 = *Str++;
					c2 = *InStr++;
					c1 = ToLower(c1);
					c2 = ToLower(c2);
					if (!c1 && (Two ? !c2 : 1))
						return true;

				} while (c1 == c2);

				return false;
			}
		}

		template <typename T>
		__forceinline PMMPTE GetPteFromVA(T VA)
		{
			return (PMMPTE)((Core::dwPTEBase) + (((ULONG64)(VA) >> 9) & 0x7FFFFFFFF8ull));
		}

		bool SetupHook(/*uint64_t GetAddrFunc,*/ PDRIVER_OBJECT DrvObj, PDRIVER_DISPATCH* Org, void* Hook);
		void* GetSysVA_MDL(PMDL Mdl);
		BYTE IsValidPtr(void* Addr, bool Write);
		void ForceWrite(void* Dst, void* Src, uint32_t Size);

		PVOID FindPatternImage(PCHAR base, PCHAR pattern, PCHAR mask);
	}


}