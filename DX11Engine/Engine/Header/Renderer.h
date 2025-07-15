#pragma once

#include "Component.h"

NS_BEGIN(Engine)

class ENGINE_DLL CRenderer abstract : public CComponent
{
protected:	
	explicit CRenderer();
	~CRenderer();
	
public:
	void OnDestroy() override;

protected:
	HRESULT Initialize() override;

public:
	virtual void Render_WithCamera(CCamera* _cam) PURE;
	virtual void Render_Outline(CCamera* _cam) PURE;

public:
	void Set_Material(CMaterial* _material);

protected:
	CMaterial* m_pMaterial;
	CMaterial* m_pOutlineMat;
};

NS_END

