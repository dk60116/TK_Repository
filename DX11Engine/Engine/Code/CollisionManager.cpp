#include "epch.h"
#include "CollisionManager.h"

CCollisionManager::CCollisionManager()
	: m_vColliderList({})
	, m_mCollisionFilter({})
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
	for (_uint i = 0; i < 32; ++i)
	{
		for (_uint j = 0; j < 32; ++j)
			GetInstance().m_mCollisionFilter.emplace(make_pair(1u << i, 1u << j), true);
	}

	return S_OK;
}

void CCollisionManager::Release()
{
	GetInstance().m_vColliderList.clear();
}

void CCollisionManager::UpdateCollision()
{
	const size_t n = GetInstance().m_vColliderList.size();
	
	if (n < 2) 
		return;

	for (size_t i = 0; i < GetInstance().m_vColliderList.size(); ++i)
	{
		for (size_t j = i; j < GetInstance().m_vColliderList.size(); ++j)
		{
			_uint layerA = GetInstance().m_vColliderList[i]->Get_GameObject()->GetLayer();
			_uint layerB = GetInstance().m_vColliderList[j]->Get_GameObject()->GetLayer();
			
			if (!GetInstance().m_mCollisionFilter[{layerA, layerB}])
				continue;
			if (!GetInstance().m_mCollisionFilter[{layerB, layerA}])
				continue;

			auto boxA = dynamic_cast<CBoxCollider*>(GetInstance().m_vColliderList[i]);
			auto boxB = dynamic_cast<CBoxCollider*>(GetInstance().m_vColliderList[j]);

			if (boxA && boxB)
			{
				_bool isContatc = CBoxCollider::IntersectOBBtoOBB(boxA->Get_WorldOBB(), boxB->Get_WorldOBB());

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

			auto sphereA = dynamic_cast<CSphereCollider*>(GetInstance().m_vColliderList[i]);
			auto sphereB = dynamic_cast<CSphereCollider*>(GetInstance().m_vColliderList[j]);

			if (sphereA && sphereB)
			{
				_bool isContatc = CSphereCollider::IntersectSPHEREToSPHERE(sphereA->Get_WorldSPHERE(), sphereB->Get_WorldSPHERE());

				if (isContatc)
				{
					sphereA->EnterOther(sphereB);
					sphereB->EnterOther(sphereA);
				}
				else
				{
					sphereA->ExitOther(sphereB);
					sphereB->ExitOther(sphereA);
				}
			}

			if (boxA && sphereB)
			{
				_bool isContatc = CBoxCollider::IntersectOBBtoSPHERE(boxA->Get_WorldOBB(), sphereB->Get_WorldSPHERE());


				if (isContatc)
				{
					boxA->EnterOther(sphereB);
					sphereB->EnterOther(boxA);
				}
				else
				{
					boxA->ExitOther(sphereB);
					sphereB->ExitOther(boxA);
				}
			}

			if (boxB && sphereA)
			{
				_bool isContatc = CBoxCollider::IntersectOBBtoSPHERE(boxB->Get_WorldOBB(), sphereA->Get_WorldSPHERE());


				if (isContatc)
				{
					boxB->EnterOther(sphereA);
					sphereA->EnterOther(boxB);
				}
				else
				{
					boxB->ExitOther(sphereA);
					sphereA->ExitOther(boxB);
				}
			}
		}
	}

	GetInstance().m_vColliderList.clear();
}

const vector<CCollider*>& CCollisionManager::Get_ColliderList()
{
	return GetInstance().m_vColliderList;
}

CCollider* CCollisionManager::Add_Collider(CCollider* _collider)
{
	GetInstance().m_vColliderList.push_back(_collider);
	return GetInstance().m_vColliderList.back();
}

void CCollisionManager::Set_CollisionFilter(const _uint _layerA, const _uint _layerB, const _bool _isCollidable)
{
	GetInstance().m_mCollisionFilter[{_layerA, _layerB}] = _isCollidable;
	GetInstance().m_mCollisionFilter[{_layerB, _layerA}] = _isCollidable;
}

void CCollisionManager::Set_CollisionFilter(const wstring _layerA, const wstring _layerB, const _bool _isCollidable)
{
	_uint layerA = CSceneManager::NameToLayer(_layerA);
	_uint layerB = CSceneManager::NameToLayer(_layerB);

	GetInstance().m_mCollisionFilter[{layerA, layerB}] = _isCollidable;
	GetInstance().m_mCollisionFilter[{layerB, layerA}] = _isCollidable;
}
