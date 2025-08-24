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
	virtual void Render_Outline(CCamera* _cam) PURE;

public:
	CMaterial* Get_Material();
	void Set_Material(CMaterial* _material);
	virtual class CMeshBuffer* Get_MeshBuffer() PURE;

protected:
	void Bind_InstanceData(_fmatrix _matWorld, CMeshBuffer* _buffer);

protected:
	CMaterial* m_pMaterial;
	CMaterial* m_pOutlineMat;
};

NS_END

