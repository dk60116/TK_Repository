#pragma once

#include "Component.h"

NS_BEGIN(Engine)

class ENGINE_DLL CRigidBody final : public CComponent
{
	friend class CGameObject;

protected:
	explicit CRigidBody();
	~CRigidBody();

private:
	static CRigidBody* Create();
	CComponent* Clone() const override;

public:
	HRESULT Initialize() override;
	void Awake() override;
	void Update() override;
	void Render_Editor() override;
	void Render() override;

	void OnDestroy() override;

private:
	class CCollider* m_pCollider;
};

NS_END

