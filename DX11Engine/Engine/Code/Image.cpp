#include "epch.h"
#include "Image.h"

CImage::CImage()
	: m_pTexture(nullptr)
{
	m_strName = L"Image";
}

CImage::~CImage()
{
}

CImage* CImage::Create()
{
	return new CImage();
}

HRESULT CImage::Initialize()
{
	if (FAILED(__super::Initialize()))
		return E_FAIL;

	return S_OK;
}

void CImage::Render_Editor()
{
}

void CImage::Render()
{
}

void CImage::OnDestroy()
{
}
