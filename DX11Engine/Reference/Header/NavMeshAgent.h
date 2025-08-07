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

private:
	_float m_fRadius;
};

NS_END

