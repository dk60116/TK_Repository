#pragma once

#include "Engine_Define.h"

BEGIN(Engine)

class ENGINE_DLL CMaterial
{
public:
	CMaterial();
	~CMaterial();

private:
	D3DMATERIAL9 m_sMaterial;
};

END

