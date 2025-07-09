#include "epch.h"
#include "RectTransform.h"

CRectTransform::CRectTransform()
{
}

CRectTransform::~CRectTransform()
{
}

CRectTransform* CRectTransform::Create()
{
	return new CRectTransform();
}
