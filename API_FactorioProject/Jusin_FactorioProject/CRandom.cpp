#include "CRandom.h"

CRandom::CRandom()
{
}

CRandom::~CRandom()
{
}

void CRandom::Init()
{
	std::random_device rd;
	m_rng.seed(rd());
}

int CRandom::Range(int _min, int _max)
{
	uniform_int_distribution<int> dist(_min, _max - 1);
	return dist(m_rng);
}

float CRandom::Range(float _min, float _max)
{
	uniform_real_distribution<float> dist(_min, _max);
	return dist(m_rng);
}
