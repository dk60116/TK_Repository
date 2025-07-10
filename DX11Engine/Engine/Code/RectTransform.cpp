#include "epch.h"
#include "RectTransform.h"

CRectTransform::CRectTransform()
	: m_pUI(nullptr)
	, m_iWidth(0)
	, m_iHeight(0)
{
	m_strName = L"Rect Transform";
}

CRectTransform::~CRectTransform()
{
}

CRectTransform* CRectTransform::Create()
{
	return new CRectTransform();
}

void CRectTransform::OnDestroy()
{
	__super::OnDestroy();

	Safe_Release(m_pUI);
}

void CRectTransform::Set_UI(CUI* _pUI)
{
	m_pUI = _pUI;
	m_pUI->AddRef();
}

void CRectTransform::SetParent(CTransform* _parent)
{
	__super::SetParent(_parent);

	CCanvas* canvas = _parent->Get_GameObject()->GetComponent<CCanvas>();

	if (canvas)
	{
		m_pUI->Set_Canvas(canvas);
		canvas->Add_UIObject(m_pUI);
	}
}
