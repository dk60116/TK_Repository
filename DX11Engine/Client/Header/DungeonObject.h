#pragma once
#include "Component.h"

class CDungeonObject abstract : public CComponent
{
	typedef struct DungeonObjectDescription
	{
		_float scaleFactor = 0.015f;
		_bool isSkinning = false;
		_bool hasCollider = true;
		_bool isTrigger = false;
		vector3 colliderCenter = vector3::zero();
		vector3 colliderSize = vector3::one();
	} DUNGEONOBJDESC;

protected:
	explicit CDungeonObject();
	~CDungeonObject();

public:
	HRESULT Initialize(void* _desc) override;

	void Awake() override;
	void Start() override;
	void Update() override;
	void OnDestroy() override;

public:
	void Set_Chapter(class CDungeonChapter* _chapter);
	const wstring& Get_ObjName();
	void Set_Operation(const _bool _on);

protected:
	class CDungeonChapter* m_pChapter;
	wstring m_strObjName;
	DUNGEONOBJDESC m_sDescription;
	vector<CMeshRenderer*> m_vRenderer;
	vector<CSkinnedMeshRenderer*> m_vSkinnedRenderer;
	CAnimator* m_pAnimator;
	CBoxCollider* m_pCollider;
	CBoxCollider* m_pBodyCollider;
	_bool m_bOperation;
};

