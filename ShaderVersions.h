#pragma once

#include <string>
#include <sstream>

namespace ShaderVersion
{
	const unsigned int VERSION = (0 << 24) | (0 << 16) | (0 << 8) | (0 << 0);

	std::wstring GetVersionFromNumber(unsigned int version)
	{
		unsigned int num1 = (version >> 24) & 0xFF;
		unsigned int num2 = (version >> 16) & 0xFF;
		unsigned int num3 = (version >> 8) & 0xFF;
		unsigned int num4 = (version >> 0) & 0xFF;
		std::wstringstream stream;
		stream << num1 << L"." << num2 << L"." << num3 << L"." << num4;
		return stream.str();
	}

	unsigned int GetNumberFromVersion(std::wstring version)
	{
		std::wstringstream stream(version);
		unsigned int num1, num2, num3, num4;
		wchar_t dot;
		if (!(stream >> num1 >> dot >> num2 >> dot >> num3 >> dot >> num4))
		{
			return 0;
		}
		return (num1 << 24) | (num2 << 16) | (num3 << 8) | (num4 << 0);
	}

	std::string GetVersionString()
	{
		unsigned int num1 = (VERSION >> 24) & 0xFF;
		unsigned int num2 = (VERSION >> 16) & 0xFF;
		unsigned int num3 = (VERSION >> 8) & 0xFF;
		unsigned int num4 = (VERSION >> 0) & 0xFF;
		std::stringstream stream;
		stream << num1 << "." << num2 << "." << num3 << "." << num4;
		return stream.str();
	}

	std::wstring GetVersionWString()
	{
		unsigned int num1 = (VERSION >> 24) & 0xFF;
		unsigned int num2 = (VERSION >> 16) & 0xFF;
		unsigned int num3 = (VERSION >> 8) & 0xFF;
		unsigned int num4 = (VERSION >> 0) & 0xFF;
		std::wstringstream stream;
		stream << num1 << L"." << num2 << L"." << num3 << L"." << num4;
		return stream.str();
	}
}

