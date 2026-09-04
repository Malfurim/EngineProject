#pragma once

#include <string>
#include <sstream>
#include <iomanip>

#ifndef _SAFE_DELETE_
#define SAFE_DELETE(p) if(p) { delete (p); (p) = nullptr; }
#endif

#ifndef _SAFE_DELETE_ARRAY_
#define SAFE_DELETE_ARRAY(p) if(p) { delete[] (p); (p) = nullptr; }
#endif

#ifndef _SAFE_RELEASE_
#define SAFE_RELEASE(p) if(p) { (p)->Release(); (p) = nullptr; }
#endif

#ifndef _SAFE_DELETE_VECTOR_
#define SAFE_DELETE_VECTOR(p) for (auto* resource : (p)) { delete resource;	} (p).clear();
#endif

std::wstring GetFilename(const std::wstring& filePath);
uintmax_t GetFileSize(const std::wstring& filePath);
uintmax_t GetFileSize(const std::string& filePath);
int CountOf(const std::wstring& text, wchar_t character);
std::wstring ToWStringWithPrecision(float value, int precision);

