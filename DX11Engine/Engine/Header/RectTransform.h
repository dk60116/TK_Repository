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
	void OnDestroy() override;

public:
	void SetParent(CTransform* _parent) override;

private:
	void Set_UI(CUI* _pUI);

private:
	CUI* m_pUI;
	_int m_iWidth, m_iHeight;
};

NS_END

