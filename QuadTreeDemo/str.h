#pragma once
#include <string>

using namespace std;

inline wstring CutAfterPoint(const wstring& instr, int incount)
{
	std::size_t pos = instr.find(L".");
	if (pos == wstring::npos)
		return instr;

	int ilen = pos + incount + 1;

	if (ilen <= 0)
		return L"";

	std::size_t len = static_cast<std::size_t>(ilen);

	if (len >= instr.length())
		return instr;

	return instr.substr(0, len);
}
