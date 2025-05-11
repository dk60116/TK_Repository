#pragma once

#include "Resource.h"

BEGIN(Engine)

class ENGINE_DLL CAudioClip final
	: public CResource
{
public:
	CAudioClip();
	~CAudioClip();

public:
	HRESULT Load(LPDIRECT3DDEVICE9 _device) override;
	void Release() override;

public:
	FMOD::Sound* getSound() const { return m_pSound; }

private:
	FMOD::Sound* m_pSound;
};

END

