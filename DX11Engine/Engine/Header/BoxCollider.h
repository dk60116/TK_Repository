#pragma once

#include "Collider.h"
#include "SphereCollider.h"

NS_BEGIN(Engine)

class ENGINE_DLL CBoxCollider final : public CCollider
{
	friend class CGameObject;
	friend class CCollisionManager;

public:
	typedef struct OrientedBoundingBox : public COLLIDERTF
	{
		vector3 halfExtents = vector3::one() * 0.5f; 
		vector3 axis[3] = {};

	}OBB;

private:
	explicit CBoxCollider();
	~CBoxCollider();

private:
	static CBoxCollider* Create();
	CComponent* Clone() const override;

public:
	HRESULT Initialize() override;
	void Update() override;
	void LateUpdate() override;
	void Render_Editor() override;
	void OnDestroy() override;

public:
	void Set_Center(vector3 _center) override;
	void Set_Size(vector3 _size);

public:
	static _bool Raycast(CPhysics::Ray _ray, const OBB& _box, _float& _outT);
	static void GetCorners(const OBB& _b, vector3 _outCorners[8]);

private:
	void Build_WorldOBB();
	static _bool IntersectOBBtoOBB(const OBB& _boxA, const OBB& _boxB, _float* _outPen = nullptr, vector3* _outAxis = nullptr);
	static _bool IntersectOBBtoSPHERE(const OBB& _box, const CSphereCollider::SPHERE& _sphere, _float* _outPen = nullptr, vector3* _outAxis = nullptr);
	static _float ProjectionRadius(const OBB& _obb, const vector3& _n);

public:
	const OBB& Get_WorldOBB();
	
private:
	OBB m_sLocal;
	OBB m_sWorld;
};

NS_END

