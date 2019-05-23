#pragma once
#include <Windows.h>
#include <wincrypt.h>
#include <time.h>

class RandomEngine
{
private:
	HCRYPTPROV m_hProvider;
	RandomEngine(const RandomEngine& copy) = delete;

public:
	RandomEngine();
	~RandomEngine();

	template <class T>
	T GetRandom()
	{
		T r;
		CryptGenRandom(m_hProvider, sizeof(T), reinterpret_cast<BYTE*>(&r));
		return r;
	}

	template <class T>
	T GetRandomBetween(T min, T max)
	{
		T r = GetRandom<T>();
		return r % (max - min) + min;
	}
};