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
	void RayUpdate_