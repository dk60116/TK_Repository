#include "CTransform.h"

CTransform::CTransform()
	: m_v2Position(vector2::zero())
	, m_v2Scale(vector2::one())
	, m_v2ChildPosition(vector2::zero())
	, m_fRotation(0.f)
	, m_v2FixedDirection(vector2::zero())
	, m_v2Pivot(vector2::zero())
{
}

CTransform::~CTransform()
{
}

void CTransform::Translate(const vector2 _dir)
{
	m_v2FixedDirection = vector2::zero();
	m_v2FixedDirection += _dir;
}

void CTransform::Reset()
{
	m_v2FixedDirection = vector2::zero();
}
