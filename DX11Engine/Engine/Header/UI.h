#pragma once

#include "Component.h"

#include "UIManager.h"

NS_BEGIN(Engine)

class ENGINE_DLL CUI abstract : public CComponent
{
protected:
	CUI();
	~CUI();

protected:
	CRectTransform* m_pRectTransform;

public:
	HRESULT Initialize() override;
};

NS_END

