#include "epch.h"
#include "RectTransform.h"

CRectTransform::CRectTransform()
	: m_pUI(nullptr)
	, m_vRPosition({})
	, m_fWidth(0.f)
	, m_fHeight(0.f)
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

void CRectTransform::Update()
{
	__super::Update();

	CCanvas* canvas = m_pUI->Get_Canvas();

	if (!canvas)
		return;

	vector2 canvasSize = vector2(canvas->Get_Transform()->Get_LocalScale().x, canvas->Get_Transform()->Get_LocalScale().y);

	m_fWidth = canvasSize.x * 100.f * m_vScale.x;

	if (CInput::GetInstance().GetKeyDown_Editor(M))
		CDebug::Log(m_pGameObject->Get_ObjectName() + L": " + to_wstring(m_fWidth));
;}

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
