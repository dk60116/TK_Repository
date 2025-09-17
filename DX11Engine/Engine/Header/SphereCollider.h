#pragma once

#include "Collider.h"

NS_BEGIN(Engine)

class ENGINE_DLL CSphereCollider final : public CCollider
{
	friend class CGameObject;
	friend class CCollisionManager;

public:
	typedef struct SphereCol : public COLLIDERTF
	{
		vector3 center = {};
		_float radius = 0.5f;
	}SPHERE;

private:
	explicit CSphereCollider();
	~CSphereCollider();

private:
	static CSphereCollider* Create();
	CComponent* Clone() const override;

public:
	HRESULT Initialize(void* _desc) override;
	void Update() override;
	void LateUpdate() override;
	void Render_Editor() override;
	void OnDestroy() override;

public:
	void Set_Center(const vector3 _center) override;
	void Set_Size(const _float _size);

public:
	static _bool Raycast(CPhysics::Ray _ray, const SPHERE& _sphere, _float& _outT);

private:
	void Build_WorldSPHERE();
	static _bool IntersectSPHEREToSPHERE(const SPHERE& _sphereA, const SPHERE& _sphereB, _float* _outPen = nullptr, vector3* _outAxis = nullptr);

public:
	const SPHERE& Get_WorldSPHERE();

public:
	SPHERE m_sLocal, m_sWorld;
};

NS_END

