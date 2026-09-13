#include "Utils.h"
#include <sstream>
#include <iomanip>

std::wstring GetFilename(const std::wstring& filePath)
{
	std::wstring path = filePath;
	int posSlash = path.find_last_of(L"/");
	int posDot = path.find_last_of(L".");
	if (posSlash >= 0 && posDot >= 0)
	{
		return path.substr(posSlash + 1, posDot - posSlash - 1);
	}
	if (posSlash >= 0 && posDot < 0)
	{
		std::wstring name = path;
		while (posSlash == (name.length() - 1))
		{
			name = name.substr(0, name.length() - 1);
			posSlash = name.find_last_of(L"/");
		}
		return name.substr(posSlash + 1, name.length());
	}
	if (posSlash < 0 && posDot >= 0)
	{
		return path.substr(0, posDot);
	}
	return path;
}

uintmax_t GetFileSize(const std::wstring& filePath)
{
	FILE* file = nullptr;
	uintmax_t length = 0;
	_wfopen_s(&file, filePath.c_str(), L"rb");
	if (file != nullptr)
	{
		fseek(file, 0, SEEK_END);
		length = (uintmax_t)ftell(file);
		fclose(file);
	}
	file = nullptr;
	return length;
}


uintmax_t GetFileSize(const std::string& filePath)
{
	FILE* file = nullptr;
	uintmax_t length = 0;
	fopen_s(&file, filePath.c_str(), "rb");
	if (file != nullptr)
	{
		fseek(file, 0, SEEK_END);
		length = (uintmax_t)ftell(file);
		fclose(file);
	}
	file = nullptr;
	return length;
}

int CountOf(const std::wstring& text, wchar_t character)
{
	return static_cast<int>(std::count(text.begin(), text.end(), character));	// using int(std::count()) doesn't make it faster
}

std::wstring ToWStringWithPrecision(float value, int precision)
{
	std::wstringstream stream;
	stream << std::fixed << std::setprecision(precision) << value;
	return stream.str();
}
