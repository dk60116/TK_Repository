#include "AudioClip.h"

CAudioClip::CAudioClip()
	: m_pSound(nullptr)
{
}

CAudioClip::~CAudioClip()
{
	Release();
}

HRESULT CAudioClip::Load(LPDIRECT3DDEVICE9 _device)
{
	Release();

	if (m_strFilePath.empty())
		return E_FAIL;

	CDebug::Log(L"[AudioClip::Load] File = " + m_strFilePath);

	return S_OK;
}

void CAudioClip::Release()
{
	__super::Release();
}
