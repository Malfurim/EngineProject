#pragma once

#include <assert.h>
#include <string>

template <typename T> class CSingleton
{
	static T* ms_singleton;

public:
	CSingleton()
	{
		assert(!ms_singleton);
		int offset = (int)(T*)1 - (int)(CSingleton <T>*) (T*) 1;
		ms_singleton = (T*)((int)this + offset);
	}

	virtual ~CSingleton()
	{
		assert(ms_singleton);
		ms_singleton = nullptr;
	}

	static T& Instance()
	{
		assert(ms_singleton);
		return (*ms_singleton);
	}

	static T* InstancePtr()
	{
		return (ms_singleton);
	}
};

template <typename T> T* CSingleton <T>::ms_singleton = nullptr;



template <typename T> class Singleton
{
	static T* ms_singleton;

public:
	Singleton(Singleton const&) = delete;
	Singleton& operator=(Singleton const&) = delete;

	~Singleton()
	{
	}

	static T* InstancePtr()
	{
		if (ms_singleton == nullptr)
			ms_singleton = new T();
		return ms_singleton;
	}

protected:
	Singleton()
	{
		ms_singleton = new T();
	}

};

template <typename T> T* Singleton <T>::ms_singleton = nullptr;
