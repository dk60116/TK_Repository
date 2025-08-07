#pragma once

#include "Component.h"

NS_BEGIN(Engine)

NS_BEGIN(EngineAI)

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
	void LateUpdate() override;
	void Render() override;
	void OnDestroy() override;

private:
	void RayUpdate_Floor();
	void RayUpdate_Direction();

private:
	_float m_fRadius;
	vector3 m_vPrevPosition;
	vector3 m_vMoveDirection;
};

NS_END

NS_END

