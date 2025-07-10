#include "epch.h"
#include "RectTransform.h"

CRectTransform::CRectTransform()
	: m_iWidth(0)
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
