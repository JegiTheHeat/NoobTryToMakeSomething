#pragma once
#include "stdafx.h"
#include <windows.h>
#include <vector>

namespace mem
{
	void Patch(FLOAT* dst, FLOAT* src, unsigned int size);
	void PatchEx(BYTE* dst, BYTE* src, unsigned int size, HANDLE hProcess);
	void Nop(FLOAT* dst, unsigned int);
	void NopEx(BYTE* dst, unsigned int size, HANDLE hProcess);
	uintptr_t FindDMAAddy(uintptr_t ptr, std::vector<unsigned int> offsets);
}