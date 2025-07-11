#pragma once

#include "Transform.h"

NS_BEGIN(Engine)

class ENGINE_DLL CRectTransform final : public CTransform
{
public:
	struct Anchors
	{
		vector2 min = vector2::one() * 0.5f, max = vector2::one() * 0.5f;
	};

	friend class CGameObject;
	friend class CUI;

protected:
	CRectTransform();
	~CRectTransform();

private:
	static CRectTransform* Create();

public:
	void Update() override;
	void Render_Gizmo() override;
	void OnDestroy() override;

public:
	void SetParent(CTransform* _parent) override;

	const vector2 Get_AnchoredPosition() const;
	const _float Get_Width() const;
	const _float Get_Height() const;

private:
	void Set_UI(CUI* _pUI);

private:
	CUI* m_pUI;
	vector2 m_vAnchoredPosition, m_vAnchoredSclae;
	_float m_fWidth, m_fHeight;

	Anchors m_sAnchors;
	vector2 m_fPivot;

	CRectTransform* m_pParentRect;
	_bool m_bIsRootRect;
};

NS_END

