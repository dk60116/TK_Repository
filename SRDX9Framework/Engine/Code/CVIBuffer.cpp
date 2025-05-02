#include "CVIBuffer.h"
#include "CManagement.h"

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

void CVIBuffer::Render_Outline(LPDIRECT3DDEVICE9 _device)
{
	if (!m_sOptions.editorOutline)
		return;

	// ── 0. 상태블록으로 통째 백업 → 실수 방지
	LPDIRECT3DSTATEBLOCK9 pSB = nullptr;
	_device->CreateStateBlock(D3DSBT_ALL, &pSB);
	pSB->Capture();

	// ── 1. 단색 셋업 (Texture-Factor 방식)
	_device->SetTexture(0, nullptr);
	_device->SetRenderState(D3DRS_TEXTUREFACTOR, m_sOptions.outlineColor.dColor());
	_device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
	_device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TFACTOR);
	_device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
	_device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TFACTOR);
	// Stage 1 이후 완전히 끄기 (안 끄면 다른 스테이지에 곱-소거될 수 있음)
	_device->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_DISABLE);
	_device->SetTextureStageState(1, D3DTSS_ALPHAOP, D3DTOP_DISABLE);

	_device->SetRenderState(D3DRS_LIGHTING, FALSE);   // 조명 끔
	_device->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);// 깊이 갱신 X
	_device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);// 앞면 제거(보통 CCW 가 앞면일 때)

	// ── 2. 월드 행렬 살짝 확장(실루엣이 바깥으로 튀어나오게)
	D3DXMATRIX matWorld, matScale, matOut;
	_device->GetTransform(D3DTS_WORLD, &matWorld);

	const float s = m_sOptions.outlineWidth;      // 예: 1.03f
	D3DXMatrixScaling(&matScale, s, s, s);
	matOut = matScale * matWorld;
	_device->SetTransform(D3DTS_WORLD, &matOut);

	// ── 3. 버퍼 바인딩 & 드로우
	_device->SetStreamSource(0, m_pVB, 0, m_sOptions.vtxSize);
	_device->SetFVF(m_sOptions.fvf);
	if (m_pIB) _device->SetIndices(m_pIB);

	if (m_pIB)
		_device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0,
			m_sOptions.vtxCnt, 0, m_sOptions.triCnt);
	else
		_device->DrawPrimitive(D3DPT_TRIANGLELIST, 0, m_sOptions.triCnt);

	// ── 4. 모든 상태 일괄 복구
	pSB->Apply();
	pSB->Release();
}

void CVIBuffer::Destroy()
{
	Safe_Release(m_pVB);
}

HRESULT CVIBuffer::FillVertexBuffer(const void* _pVertices, const UINT size)
{
	if (!m_pVB) 
		return E_FAIL;

	void* pData = nullptr;
	if (FAILED(m_pVB->Lock(0, size, &pData, 0)))
		return E_FAIL;

	memcpy(pData, _pVertices, size);
	m_pVB->Unlock();

	return S_OK;
}

HRESULT CVIBuffer::FillIndexBuffer(const void* _pIndices, const UINT _size)
{
	if (!m_pIB) 
		return E_FAIL;

	void* pData = nullptr;
	if (FAILED(m_pIB->Lock(0, _size, &pData, 0)))
		return E_FAIL;

	memcpy(pData, _pIndices, _size);
	m_pIB->Unlock();

	return S_OK;
}
