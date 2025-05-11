#pragma once

#include "Resource.h"

BEGIN(Engine)

class ENGINE_DLL CAudioClip final
	: public CResource
{
public:
	CAudioClip();
	~CAudioClip();
};

END

