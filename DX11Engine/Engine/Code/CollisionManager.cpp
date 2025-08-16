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
	auto& colliderList = GetInstance().m_vColliderList;
	const size_t n = colliderList.size();

	if (n < 2) 
		return;

	for (size_t i = 0; i < colliderList.size(); ++i)
	{
		for (size_t j = i + 1; j < colliderList.size(); ++j)
		{
			_uint layerA = colliderList[i]->Get_GameObject()->GetLayer();
			_uint layerB = colliderList[j]->Get_GameObject()->GetLayer();
			
			if (colliderList[i]->m_iColliderID == colliderList[j]->m_iColliderID)
				continue;

			if (!GetInstance().m_mCollisionFilter[{layerA, layerB}])
				continue;
			if (!GetInstance().m_mCollisionFilter[{layerB, layerA}])
				continue;

			auto boxA = dynamic_cast<CBoxCollider*>(GetInstance().m_vColliderList[i]);
			auto boxB = dynamic_cast<CBoxCollider*>(GetInstance().m_vColliderList[j]);

			auto sphereA = dynamic_cast<CSphereCollider*>(GetInstance().m_vColliderList[i]);
			auto sphereB = dynamic_cast<CSphereCollider*>(GetInstance().m_vColliderList[j]);

			_bool isContact = false;

			CCollider* colA = nullptr;
			CCollider* colB = nullptr;

			_float pen = 0.f;
			vector3 axis = {};

			if (boxA && boxB)
			{
				colA = boxA;
				colB = boxB;
				isContact = CBoxCollider::IntersectOBBtoOBB(boxA->Get_WorldOBB(), boxB->Get_WorldOBB(), &pen, &axis);
			}
			else if (sphereA && sphereB)
			{
				colA = sphereA;
				colB = sphereB;
				isContact = CSphereCollider::IntersectSPHEREToSPHERE(sphereA->Get_WorldSPHERE(), sphereB->Get_WorldSPHERE(), &pen, &axis);
			}
			else if (boxA && sphereB)
			{
				colA = boxA;
				colB = sphereB;
				isContact = CBoxCollider::IntersectOBBtoSPHERE(boxA->Get_WorldOBB(), sphereB->Get_WorldSPHERE(), &pen, &axis);
			}
			else if (boxB && sphereA)
			{
				colA = boxB;
				colB = sphereA;
				isContact = CBoxCollider::IntersectOBBtoSPHERE(boxB->Get_WorldOBB(), sphereA->Get_WorldSPHERE(), &pen, &axis);
			}

			if (colA && colB)
			{
				if (isContact)
				{
					colA->EnterOther(colB);
					colB->EnterOther(colA);

					if (isContact)
					{
						if (!colA->IsTrigger() && !colB->IsTrigger())
						{
							CRigidBody* rbA = colA->Get_RigidBody();
							CRigidBody* rbB = colB->Get_RigidBody();

							if (rbA && !rbA->IsKinematic() || rbB && !rbB->IsKinematic())
							{
								vector3 moveA = {};
								vector3 moveB = {};

								if (rbA && rbB)
								{
									moveA = -axis * (pen * 0.5f);
									moveB = axis * (pen * 0.5f);
								}
								else if (rbA && !rbB)
								{
									moveA = -axis * pen;
								}
								else if (!rbA && rbB)
								{
									moveB = axis * pen;
								}

								if (rbA)
									rbA->Get_Transform()->Add_Position(moveA * DELTA_TIME * 10.f);
								if (rbB)
									rbB->Get_Transform()->Add_Position(moveB * DELTA_TIME * 10.f);
							}
						}
					}
				}
				else
				{
					colA->ExitOther(colB);
					colB->ExitOther(colA);
				}
			}
		}
	}

	colliderList.clear();
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
