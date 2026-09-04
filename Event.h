#pragma once

#include <vector>
#include <functional>

template<typename... Args>
class Event
{
public:
	void operator+=(std::function<void(Args...)> callback)
	{
		m_callback.push_back(callback);
	}

	void Invoke(Args... args)
	{
		for (const auto& callback : m_callback)
		{
			if (callback)
			{
				callback(args...);
			}
		}
	}

	void operator()(Args... args)
	{
		Invoke(args...);
	}

	void Clear()
	{
		m_callback.clear();
	}

private:
	std::vector<std::function<void(Args...)>> m_callback;
};