#include "epch.h"
#include "UI.h"

CUI::CUI()
{
}

CUI::~CUI()
{
}

HRESULT CUI::Initialize()
{
	if (FAILED(__super::Initialize()))
		return E_FAIL;

	if (!m_pRectTransform)
	{
		m_pRectTransform = m_pGameObject->AddComponent<CRectTransform>();
		m_pGameObject->Set_Transform(m_pRectTransform);
	}

	return S_OK;
}
