#pragma once
#include <string>

class ResourceBase
{
public:
	virtual ~ResourceBase() = default;
	std::wstring GetName() const { return m_name; }
	std::wstring GetPath() const { return m_path; }
	unsigned int GetID() const { return m_id; }

protected:
	ResourceBase() = default;
	std::wstring m_name{ L"" };
	std::wstring m_path{ L"" };
	unsigned int m_id{ 0 };
};

