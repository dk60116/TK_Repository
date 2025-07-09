#include "epch.h"
#include "Canvas.h"

CCanvas::CCanvas()
{
}

CCanvas::~CCanvas()
{
}

CCanvas* CCanvas::Create()
{
	return new CCanvas();
}

HRESULT CCanvas::Initialize()
{
	if (FAILED(__super::Initialize()))
		return E_FAIL;

	return S_OK;
}

void CCanvas::Update()
{
}
