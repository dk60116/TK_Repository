#include "AudioClip.h"

CAudioClip::CAudioClip()
{
}

CAudioClip::~CAudioClip()
{
	Release();
}

HRESULT CAudioClip::Load(LPDIRECT3DDEVICE9 _device)
{
	return S_OK;
}

void CAudioClip::Release()
{
}
