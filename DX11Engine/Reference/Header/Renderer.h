#pragma once

#include "Component.h"

NS_BEGIN(Engine)

class ENGINE_DLL CRenderer abstract : public CComponent
{
protected:	
	explicit CRenderer();
	~CRenderer();
	
public:
	void OnPostRender_Editor() override;
	void OnPostRender() override;
	void OnDestroy() override;

protected:
	HRESULT Initialize(void* _desc) override;

public:
	virtual void Render_WithCamera(class CCamera* _cam) PURE;
	virtual void Render_Outline(CCamera* _cam) PURE;
	virtual void Render_Shadow(const _matrix& lightVP, _bool alphaCutout = false) PURE;

public:
	CMaterial* Get_Material();
	void Set_Material(CMaterial* _material);
	virtual class CMeshBuffer* Get_MeshBuffer() PURE;
	void SetNoneCull(const _bool _value);

protected:
	void Bind_InstanceData(_fmatrix _matWorld, CMeshBuffer* _buffer);
	void ResetShaderResources();

protected:
	CMaterial* m_pMaterial;
	CMaterial* m_pOutlineMat;
	_float m_fScaleFactor;
	_bool m_bShadowTarget;
	_bool m_bNoneCulling;
};

NS_END

