#pragma once

#include "Transform.h"

NS_BEGIN(Engine)

class ENGINE_DLL CRectTransform final : public CTransform
{
	friend class CGameObject;
	friend class CUI;

protected:
	CRectTransform();
	~CRectTransform();

private:
	static CRectTransform* Create();

public:
	void Update() override;
	void OnDestroy() override;

public:
	void SetParent(CTransform* _parent) override;

private:
	void Set_UI(CUI* _pUI);

private:
	CUI* m_pUI;
	vector2 m_vRPosition;
	_float m_fWidth, m_fHeight;
};

NS_END

