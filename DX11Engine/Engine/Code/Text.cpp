#include "epch.h"
#include "Text.h"

CText::CText()
{
}

CText::~CText()
{
}

HRESULT CText::Initialize()
{
	if (FAILED(__super::Initialize()))
		return E_FAIL;

	return S_OK;
}

void CText::Render_Editor()
{
}

void CText::Render()
{
}

void CText::OnDestroy()
{
}
