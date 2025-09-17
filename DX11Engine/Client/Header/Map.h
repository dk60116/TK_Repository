#pragma once
#include "Component.h"

class CMap abstract : public CComponent
{
public:
	struct MapOptions
	{
		_float scaleFactor = 0.01f;
	};

protected:
	explicit CMap();
	~CMap();

public:
	HRESULT Initialize(void* _desc) override;

	void Awake() override;
	void Update() override;
	void OnDestroy() override;

public:
	const wstring& Get_MapName() const;

private:
	void AttachTextures();

protected:
	wstring m_strMapName;

	MapOptions m_sOptions;

	vector<CMeshRenderer*> m_vRendererList;
	map<wstring, vector<wstring>> m_mTextureNameList;

	class CMapCollision* m_pMapCollision;
};

