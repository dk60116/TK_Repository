#pragma once

#include "epch.h"
#include <random>

NS_BEGIN(Engine)

class ENGINE_DLL CRandom
{
	SINGLETONCLASS(CRandom);

public:
	static HRESULT Initialize();

public:
	static _int Range(const _int _min, const _int _max);
	static _float Range(const _float _min, const _float _max);

private:
	mt19937 m_rng;
};

NS_END

