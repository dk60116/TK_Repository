#pragma once

#include "epch.h"

class CPlayer : public CComponent
{
protected:
	CPlayer();
	~CPlayer();

public:
	static CPlayer* Create();
	HRESULT Initialize() override;

	void Awake() override;
	void Start() override;
	void Update() override;
	void OnDestroy() override;

private:
	CMeshRenderer* m_pMeshRenderer;
};

