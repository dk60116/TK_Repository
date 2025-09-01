#pragma once
#include "Component.h"

class CDungeonObject abstract : public CComponent
{
	typedef struct DungeonObjectDescription
	{
		_float scaleFactor = 0.015f;
	} DUNGEONOBJDESC;

protected:
	explicit CDungeonObject();
	~CDungeonObject();

public:
	HRESULT Initialize() override;

	void Awake() override;
	void Update() override;
	void OnDestroy() override;

public:
	void Set_Chapter(class CDungeonChapter* _chapter);
	const wstring& Get_ObjName();

protected:
	class CDungeonChapter* m_pChapter;
	wstring m_strObjName;
	DUNGEONOBJDESC m_sDescription;
	_bool m_bHasAnimation;
	vector<CMeshRenderer*> m_vRenderer;
	vector<CSkinnedMeshRenderer*> m_vSkinnedRenderer;
	CAnimator* m_pAnimator;
	CBoxCollider* m_pCollider;
};

