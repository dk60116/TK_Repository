#pragma once

#include "pch.h"
#include <random>

class CRandom
{
	SingletonClass(CRandom);

public:
	void Init();

public:
	int Range(int _min, int _max);
	float Range(float _min, float _max);

private:
	mt19937 m_rng;
};

