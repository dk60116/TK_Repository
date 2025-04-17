#pragma once

#include "CComponent.h"

BEGIN(Engine)

struct VIBufferOptions
{
	_ulong vtxSize;
	_ulong vtxCnt;
	_ulong triCnt;
	_ulong fvf;

	_ulong idxSize;
	D3DFORMAT idxFmt;

	ColorValue color;
};

class ENGINE_DLL CVIBuffer
    : public CComponent
{
public:
	explicit CVIBuffer();
	virtual ~CVIBuffer();

public:
	virtual HRESULT	Ready_Buffer();
	virtual void Render_Buffer();

public:
	void Start() override;
	void Update() override;
	void FixedUpdate() override;
	void LateUpdate() override;
	void OnEnable() override;
	void OnDisable() override;
	void OnDestroy() override;

public:
	HRESULT FillVertexBuffer(const void* pVertices, UINT size);
	HRESULT FillIndexBuffer(const void* pIndices, UINT size);
	virtual void UpdateColor() PURE;

	VIBufferOptions& getOptions() { return m_sOptions; }

protected:
	LPDIRECT3DVERTEXBUFFER9 m_pVB;
	LPDIRECT3DINDEXBUFFER9 m_pIB;

	VIBufferOptions m_sOptions;
};

END