#include "CVIBuffer.h"

CVIBuffer::CVIBuffer()
	: m_pIB(nullptr)
	, m_pVB(nullptr)
	, m_sOptions({})
{
}

CVIBuffer::~CVIBuffer()
{
	Destroy();
}

HRESULT CVIBuffer::Ready_Buffer(LPDIRECT3DDEVICE9 _device)
{
	if (FAILED(_device->CreateVertexBuffer(m_sOptions.vtxCnt * m_sOptions.vtxSize, // 버텍스 버퍼의 크기
		0,						// 정적 버퍼로 생성(D3DUSAGE_DYNAMIC : 동적 버퍼)
		m_sOptions.fvf,				// 버텍스 속성 정보
		D3DPOOL_MANAGED,		// 정적 버퍼이기 때문에 MANAGED
		&m_pVB,					// 버텍스 버퍼를 저장할 포인터
		NULL)))
		return E_FAIL;

	if (FAILED(_device->CreateIndexBuffer(m_sOptions.triCnt * m_sOptions.idxSize, // 인덱스 버퍼의 크기
		0,						// 정적 버퍼로 생성(D3DUSAGE_DYNAMIC : 동적 버퍼)
		m_sOptions.idxFmt,				// 인덱스 속성 정보
		D3DPOOL_MANAGED,		// 정적 버퍼이기 때문에 MANAGED
		&m_pIB,					// 인덱스 버퍼를 저장할 포인터
		NULL)))
		return E_FAIL;

	return S_OK;
}

void CVIBuffer::Render_Buffer(LPDIRECT3DDEVICE9 _device)
{
	if (!m_pVB)
		return;

	_device->SetStreamSource(0, m_pVB, 0, m_sOptions.vtxSize);
	_device->SetFVF(m_sOptions.fvf);

	if (m_pIB)
	{
		_device->SetIndices(m_pIB);
		_device->DrawIndexedPrimitive
		(
			D3DPT_TRIANGLELIST,
			0, 0,
			m_sOptions.vtxCnt,
			0, m_sOptions.triCnt
		);
	}
	else
	{
		_device->DrawPrimitive(D3DPT_TRIANGLELIST, 0, m_sOptions.triCnt);
	}
}

void CVIBuffer::Destroy()
{
	Safe_Release(m_pVB);
}

HRESULT CVIBuffer::FillVertexBuffer(const void* pVertices, UINT size)
{
	if (!m_pVB) 
		return E_FAIL;

	void* pData = nullptr;
	if (FAILED(m_pVB->Lock(0, size, &pData, 0)))
		return E_FAIL;

	memcpy(pData, pVertices, size);
	m_pVB->Unlock();

	return S_OK;
}

HRESULT CVIBuffer::FillIndexBuffer(const void* pIndices, UINT size)
{
	if (!m_pIB) return E_FAIL;

	void* pData = nullptr;
	if (FAILED(m_pIB->Lock(0, size, &pData, 0)))
		return E_FAIL;

	memcpy(pData, pIndices, size);
	m_pIB->Unlock();

	return S_OK;
}
