#pragma once

#include "Component.h"

class ENGINE_DLL CRenderer abstract : public CComponent
{
protected:	
	explicit CRenderer();
	~CRenderer();

public:
	CRenderer* Create();
	
public:
	void OnDestroy() override;

protected:
	virtual void Render_WithCamera(CCamera* _cam) PURE;
	virtual void Render_Outline(CCamera* _cam) PURE;

public:
	void Set_Material(CMaterial* _material);

protected:
	CMaterial* m_pMaterial;
};

