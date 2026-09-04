#pragma once

#include <string>

std::wstring GetFilename(const std::wstring& filePath);
uintmax_t GetFileSize(const std::wstring& filePath);
uintmax_t GetFileSize(const std::string& filePath);
int CountOf(const std::wstring& text, wchar_t character);
std::wstring ToWStringWithPrecision(float value, int precision);

