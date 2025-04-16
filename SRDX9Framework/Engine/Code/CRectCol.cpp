#include "CRectCol.h"
#include "CDebug.h"
#include "CGameObject.h"

CRectCol::CRectCol()
    : CVIBuffer()
{
}

CRectCol::CRectCol(const CRectCol& _rhs)
{
}

CRectCol::~CRectCol()
{
    OnDestroy();
}

HRESULT CRectCol::Ready_Buffer()
{
    return ReadyRect();

    return E_FAIL;
}

void CRectCol::UpdateColor()
{
    VTXTEX* pVertices = nullptr;
    if (SUCCEEDED(m_pVB->Lock(0, 0, (void**)&pVertices, 0)))
    {
        for (int i = 0; i < 4; ++i)
            pVertices[i].color = m_sOptions.color.dColor();

        m_pVB->Unlock();
    }
}

CComponent* CRectCol::Clone()
{
    return new CRectCol(*this);
}

HRESULT CRectCol::ReadyRect()
{
    m_sOptions.vtxSize = sizeof(VTLTEX);
    m_sOptions.vtxCnt = 4;
    m_sOptions.triCnt = 2;
    m_sOptions.fvf = FVF_LTEX;
    m_sOptions.idxSize = sizeof(INDEX16);
    m_sOptions.idxFmt = D3DFMT_INDEX16;

    if (FAILED(CVIBuffer::Ready_Buffer()))
        return E_FAIL;

    VTLTEX vertices[4] =
    {
        { { -0.5f,  0.5f, 0.f }, vector3::back().dVector(), {0.f, 0.f}},
        { {  0.5f,  0.5f, 0.f }, vector3::back().dVector(), { 1.f, 0.f } },
        { {  0.5f, -0.5f, 0.f }, vector3::back().dVector(), { 1.f, 1.f } },
        { { -0.5f, -0.5f, 0.f }, vector3::back().dVector(), { 0.f, 1.f } }
    };

    if (FAILED(FillVertexBuffer(vertices, sizeof(vertices))))
        return E_FAIL;

    INDEX16 indices[2] = { {0,1,2}, {0,2,3} };
    if (FAILED(FillIndexBuffer(indices, sizeof(indices))))
        return E_FAIL;

    return S_OK;
}
