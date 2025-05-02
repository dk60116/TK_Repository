#pragma once

#include "UObject.h"
#include "CComponent.h"

BEGIN(Engine)

struct VIBufferOptions
{
	_ulong vtxSize = 0;
	_ulong vtxCnt = 0;
	_ulong triCnt = 0;
	_ulong fvf;

	_ulong idxSize;
	D3DFORMAT idxFmt;

	ColorValue color = ColorValue::white();

	ColorValue outlineColor = ColorValue(255, 100, 0);
	_bool editorOutline = true;
	_float outlineWidth = 1.05f;
};

class ENGINE_DLL CVIBuffer abstract
	: public UObject
{
protected:
	explicit CVIBuffer();
	virtual ~CVIBuffer();

public:
	virtual HRESULT	Ready_Buffer(LPDIRECT3DDEVICE9 _device);
	virtual void Render_Buffer(LPDIRECT3DDEVICE9 _device);
	virtual void Render_Outline(LPDIRECT3DDEVICE9 _device);
	void Destroy();

public:
	HRESULT FillVertexBuffer(const void* _pVertices, const UINT _size);
	HRESULT FillIndexBuffer(const void* _pIndices, const UINT size);
	virtual void UpdateColor() PURE;

	VIBufferOptions& getOptions() { return m_sOptions; }

protected:
	LPDIRECT3DVERTEXBUFFER9 m_pVB;
	LPDIRECT3DINDEXBUFFER9 m_pIB;

	VIBufferOptions m_sOptions;
};

END