#include "CCollider.h"
#include "CGameObject.h"

CCollider::CCollider()
	: m_pBodyObject(nullptr)
	, m_eShape(RECT)
	, m_v2Pivot(vector2::zero())
	, m_v2Size(vector2::one())
	, m_sColor(ColorValue::Green())
	, m_bIsStatic(true)
	, m_bIsTrigger(false)
	, m_lEnteredColliderList({})
{
}

CCollider::~CCollider()
{
	OnDestroy_();
}

void CCollider::AddEnterCollider(CCollider* _col)
{
	for (TRAVERSAL_ITER(m_lEnteredColliderList, it))
	{
		if ((*it) == _col)
			return;
	}

		if (m_pBodyObject)
	{
		m_pBodyObject->OnCollisionEnter(_col);
	}

	m_lEnteredColliderList.push_back(_col);
}

void CCollider::RemoveEnterCollider(CCollider* _col)
{
	for (auto it = m_lEnteredColliderList.begin(); it != m_lEnteredColliderList.end();)
	{
		if (*it == _col)
		{
			it = m_lEnteredColliderList.erase(it);
			if (m_pBodyObject)
			{
				m_pBodyObject->OnCollisionExit(_col);
			}
			return;
		}
		else
			++it;
	}
}
