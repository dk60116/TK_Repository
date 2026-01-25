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
	virtual void Render_WithCamera(class CCamera* _cam) PURE;
	virtual void Render_ShadowDepth(class CLight* _light, CMaterial* _mat) PURE;
	virtual void Render_Outline(CCamera* _cam) PURE;

public:
	CMaterial* Get_Material();
	void Set_Material(CMaterial* _material);
	virtual CMeshBuffer* Get_MeshBuffer() PURE;

	const bool IsCastShadow() const;
	const bool IsRenderShadow() const;

protected:
	CMaterial* m_pMaterial;
	CMaterial* m_pOutlineMat;

	_bool m_bCastShadow;
	_bool m_bRenderShadow;
};

NS_END

