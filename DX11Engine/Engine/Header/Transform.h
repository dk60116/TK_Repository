#pragma once

#include "Component.h"

NS_BEGIN(Engine)

class ENGINE_DLL CTransform final : public CComponent
{
protected:
	explicit CTransform();
	~CTransform();

public:
	static CTransform* Create();

public:
	void Update() override;
};

NS_END

