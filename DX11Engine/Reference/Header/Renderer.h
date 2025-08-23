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

	const _bool Is_UseInstancing() const;
	void Set_UseInstancing(const _bool _value);
	void SetInstanceWorlds(const vector<_float4x4>& _worlds);
	void AddInstanceWorld(const _float4x4& _w);
	const size_t GetInstanceCount() const;
	const _bool HasInstances() const;

protected:
	CMaterial* m_pMaterial;
	CMaterial* m_pOutlineMat;

	_bool m_bUseInstancing;
	vector<_float4x4> m_vInstanceWorlds;
};

NS_END

