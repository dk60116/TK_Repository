#include "epch.h"
#include "CollisionManager.h"

CCollisionManager::CCollisionManager()
	: m_vColliderList({})
{
}

CCollisionManager::~CCollisionManager()
{
	Release();
}

CCollisionManager& CCollisionManager::GetInstance()
{
	static CCollisionManager inst;
	return inst;
}

HRESULT CCollisionManager::Initialize()
{
	return S_OK;
}

void CCollisionManager::Release()
{
	m_vColliderList.clear();
}

void CCollisionManager::UpdateCollision()
{
	const size_t n = m_vColliderList.size();
	
	if (n < 2) 
		return;

	for (size_t i = 0; i < m_vColliderList.size(); ++i)
	{
		for (size_t j = i; j < m_vColliderList.size(); ++j)
		{
			auto boxA = dynamic_cast<CBoxCollider*>(m_vColliderList[i]);
			auto boxB = dynamic_cast<CBoxCollider*>(m_vColliderList[j]);

			if (boxA && boxB)
			{
				_bool isContatc = CBoxCollider::Intersect(boxA->Get_WorldOBB(), boxB->Get_WorldOBB());

				if (isContatc)
				{
					boxA->EnterOther(boxB);
					boxB->EnterOther(boxA);
				}
				else
				{
					boxA->ExitOther(boxB);
					boxB->ExitOther(boxA);
				}
			}
		}
	}

	m_vColliderList.clear();
}

const vector<CCollider*>& CCollisionManager::Get_ColliderList()
{
	return m_vColliderList;
}

CCollider* CCollisionManager::Add_Collider(CCollider* _collider)
{
	m_vColliderList.push_back(_collider);
	return m_vColliderList.back();
}
