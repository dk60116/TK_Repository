#pragma once

#include "Component.h"

NS_BEGIN(Engine)

class ENGINE_DLL CCamera final : public CComponent
{
public:
	enum ViewMode { PERSPECTIVE, ORTHOGRAPHIC };

protected:
	explicit CCamera();
	~CCamera();

public:
	static CCamera* Create();

public:
	void Initialize() override;
	void Update() override;

private:
	void UpdateViewMatrix();
	void UpdateProjectionMatrix();
};

NS_END

