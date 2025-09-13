#pragma once

#include "Component.h"

class CMeshEffect abstract : public CComponent
{
public:
	typedef struct MeshEffectDescription
	{
		_uint meshCount = 1;
	}MESHEFFECTDESC;

protected:
	explicit CMeshEffect();
	~CMeshEffect();

public:
	HRESULT Initialize() override;
	void Awake() override;
	void Start() override;
	void Update() override;
	void OnDestroy() override;

protected:
	wstring m_strEffectName;
	MESHEFFECTDESC m_sDescription;
	vector<CMeshRenderer*> m_vMesheList;
	vector<CTexture*> m_vTextureList;
};

