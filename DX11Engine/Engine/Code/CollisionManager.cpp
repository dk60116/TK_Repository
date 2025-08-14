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
		for (size_t j = i + 1; j < GetInstance().m_vColliderList.size(); ++j)
		{
			_uint layerA = GetInstance().m_vColliderList[i]->Get_GameObject()->GetLayer();
			_uint layerB = GetInstance().m_vColliderList[j]->Get_GameObject()->GetLayer();
			
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

			if (boxA && boxB)
			{
				colA = boxA;
				colB = boxB;
				isContact = CBoxCollider::IntersectOBBtoOBB(boxA->Get_WorldOBB(), boxB->Get_WorldOBB());

				if (isContact)
				{
					//boxA->EnterOther(boxB);
					//boxB->EnterOther(boxA);

					//if (!boxA->IsTrigger() && !boxB->IsTrigger())
					//{
					//	// (b) RigidBody 존재 여부 판단
					//	// 실제 엔진에 맞게 교체:
					//	// auto* rbA = dynamic_cast<CRigidBody*>(boxA->Get_GameObject()->Get_Component(L"RigidBody"));
					//	// auto* rbB = dynamic_cast<CRigidBody*>(boxB->Get_GameObject()->Get_Component(L"RigidBody"));
					//	CRigidBody* rbA = boxA->
					//	auto* rbB = /* TODO: Get RB from boxB->Get_GameObject() */ nullptr;

					//	if (rbA || rbB)
					//	{
					//		// 정규화된 분리 축이 들어오므로 pen * axis 로 이동량 계산
					//		// axis 방향: B를 +axis로, A를 -axis로 밀면 분리되도록 위 Intersect 함수가 보장
					//		const vector3 pushDir = axis;      // normalized
					//		const _float  depth = pen;       // penetration depth

					//		vector3 moveA = vector3::zero();
					//		vector3 moveB = vector3::zero();

					//		if (rbA && rbB) {
					//			// 4) 둘 다 RB: 반반
					//			moveA = -pushDir * (depth * 0.5f);
					//			moveB = pushDir * (depth * 0.5f);
					//		}
					//		else if (rbA && !rbB) {
					//			// 3) A만 RB: A만 전부 이동
					//			moveA = -pushDir * depth;
					//		}
					//		else if (!rbA && rbB) {
					//			// 3) B만 RB: B만 전부 이동
					//			moveB = pushDir * depth;
					//		}
				}
			}
			else if (sphereA && sphereB)
			{
				colA = sphereA;
				colB = sphereB;
				isContact = CSphereCollider::IntersectSPHEREToSPHERE(sphereA->Get_WorldSPHERE(), sphereB->Get_WorldSPHERE());
			}
			else if (boxA && sphereB)
			{
				colA = boxA;
				colB = sphereB;
				isContact = CBoxCollider::IntersectOBBtoSPHERE(boxA->Get_WorldOBB(), sphereB->Get_WorldSPHERE());
			}
			else if (boxB && sphereA)
			{
				colA = boxB;
				colB = sphereA;
				isContact = CBoxCollider::IntersectOBBtoSPHERE(boxB->Get_WorldOBB(), sphereA->Get_WorldSPHERE());
			}

			if (colA && colB)
			{
				if (isContact)
				{
					colA->EnterOther(colB);
					colB->EnterOther(colA);
				}
				else
				{
					colA->ExitOther(colB);
					colB->ExitOther(colA);
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
