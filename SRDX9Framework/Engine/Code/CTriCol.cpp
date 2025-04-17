#include "CTriCol.h"

CTriCol::CTriCol()
{
}

CTriCol::CTriCol(const CTriCol& _rhs)
    : CVIBuffer(_rhs)
{
}

CTriCol::~CTriCol()
{
}

HRESULT CTriCol::Ready_Buffer(LPDIRECT3DDEVICE9 _device)
{
    return E_NOTIMPL;
}

void CTriCol::Render_Buffer(LPDIRECT3DDEVICE9 _device)
{
}

//void CTriCol::Awake()
//{
//	m_sOptions.triCnt = 1;
//	m_sOptions.vtxCnt = 3;
//	m_sOptions.fvf = FVF_COL;
//	m_sOptions.vtxSize = sizeof(VTXCOL);
//
//	if (FAILED(CVIBuffer::Ready_Buffer()))
//		return;
//
//	VTXCOL* pVertex = nullptr;
//
//	m_pVB->Lock(0, 0, (void**)&pVertex, 0);
//
//	pVertex[0].vPosition = { 0.f, 1.f, 0.f };
//	pVertex[0].dwColor = D3DXCOLOR(1.f, 0.f, 0.f, 1.f);
//
//	pVertex[1].vPosition = { 1.f, -1.f, 0.f };
//	pVertex[1].dwColor = D3DXCOLOR(0.f, 1.f, 0.f, 1.f);
//
//	pVertex[2].vPosition = { -1.f, -1.f, 0.f };
//	pVertex[2].dwColor = D3DXCOLOR(0.f, 0.f, 1.f, 1.f);
//
//	m_pVB->Unlock();
//
//	//INDEX16* pIndex = nullptr;
//
//	//m_pIB->Lock(0, 0, (void**)&pIndex, 0);
//
//	//// 오른쪽 위
//	//pIndex[0]._0 = 0;
//	//pIndex[0]._1 = 1;
//	//pIndex[0]._2 = 2;
//
//	//m_pIB->Unlock();
//}
//
//void CTriCol::Render()
//{
//	CVIBuffer::Render_Buffer();
//}

CComponent* CTriCol::Clone()
{
    return nullptr;
}
