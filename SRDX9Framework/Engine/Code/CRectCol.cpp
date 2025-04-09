#include "CRectCol.h"

CRectCol::CRectCol()
	: CVIBuffer()
{
}

CRectCol::CRectCol(const CRectCol& _rhs)
{
}

CRectCol::~CRectCol()
{
}

HRESULT CRectCol::Ready_Buffer()
{
	m_sOptions.vtxSize = sizeof(VTXCOL);
	m_sOptions.vtxCnt = 4;
	m_sOptions.triCnt = 2;
	m_sOptions.fvf = FVF_COL;
	m_sOptions.idxSize = sizeof(INDEX16);
	m_sOptions.idxFmt = D3DFMT_INDEX16;

	if (FAILED(CVIBuffer::Ready_Buffer()))
		return E_FAIL;

	VTXCOL vertices[4] =
	{
		{{ -0.5f,  0.5f, 0.f }, m_sOptions.color.dColor()},
		{{  0.5f,  0.5f, 0.f }, m_sOptions.color.dColor()},
		{{  0.5f, -0.5f, 0.f }, m_sOptions.color.dColor()},
		{{ -0.5f, -0.5f, 0.f }, m_sOptions.color.dColor()}
	};

	if (FAILED(FillVertexBuffer(vertices, sizeof(vertices))))
		return E_FAIL;

	INDEX16 indices[2] = 
	{
		{ 0, 1, 2 },
		{ 0, 2, 3 }
	};

	if (FAILED(FillIndexBuffer(indices, sizeof(indices))))
		return E_FAIL;

	return S_OK;
}

void CRectCol::UpdateColor()
{
	VTXCOL* pVertices = nullptr;

	m_pVB->Lock(0, 0, (void**)&pVertices, 0);

	for (int i = 0; i < 4; ++i)
	{
		pVertices[i].dwColor = m_sOptions.color;
	}

	m_pVB->Unlock();
}

CComponent* CRectCol::Clone()
{
	return new CRectCol(*this);
}
