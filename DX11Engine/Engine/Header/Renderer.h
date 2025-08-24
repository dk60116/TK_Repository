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
	virtual CMeshBuffer* Get_MeshBuffer() PURE;

	const _bool Is_UseInstancing() const;
	void Set_UseInstancing(const _bool _value);

protected:
	CMaterial* m_pMaterial;
	CMaterial* m_pOutlineMat;
};

NS_END

