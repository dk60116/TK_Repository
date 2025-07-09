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
	if (!m_pRectTransform)
		m_pRectTransform = m_pGameObject->AddComponent<CRectTransform>();

	return S_OK;
}
