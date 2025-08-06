#pragma once

#include "Component.h"

NS_BEGIN(Engine)

class ENGINE_DLL CNavMeshAgent : public CComponent
{
	friend class CGameObject;

private:
	CNavMeshAgent();
	~CNavMeshAgent();

private:
	static CNavMeshAgent* Create();
	CComponent* Clone() const override;

public:
	HRESULT Initialize() override;
	void Update() override;
	void Render() override;
	void OnDestroy() override;

public:
	static _bool PointInTri(const vector3& _p, const vector3& _a, const vector3& _b, const vector3& _c);
	static vector3 ClosestPointOnSegment(const vector3& _p, const vector3& _a, const vector3& _b, const vector3& _c);
};

NS_END

