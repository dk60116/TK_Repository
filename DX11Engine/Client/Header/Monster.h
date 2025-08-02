#pragma once

#include "epch.h"

class CMonster abstract : public CComponent
{
protected:
	CMonster();
	~CMonster();

public:
	static CMonster* Create();
	HRESULT Initialize() override;

	void Awake() override;
	void Start() override;
	void Update() override;
	void OnDestroy() override;

protected:
	wstring m_strSkinnedMeshBufferName;
	vector<CSkinnedMeshRenderer*> m_vMeshRenderers;
	CTexture* m_pBaseMap;
	CAnimator* m_pAnimator;
};

