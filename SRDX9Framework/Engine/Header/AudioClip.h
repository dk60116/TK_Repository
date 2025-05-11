#pragma once

#include "Resource.h"

BEGIN(ENGINE)

class ENGINE_DLL CAudioClip final
	: public CResource
{
public:
	CAudioClip();
	~CAudioClip();
};

END

