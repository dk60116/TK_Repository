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
    switch (m_eRenderType)
    {
    case Buffer_Color:
        return ReadyColorRect();
    case Buffer_Texture:
        return ReadyTextureRect();
    default:
        break;
    }

    return E_FAIL;
}

void CRectCol::UpdateColor()
{
    switch (m_eRenderType)
    {
    case CVIBuffer::Buffer_Color: 
    {
        VTXCOL* pVertices = nullptr;
        if (SUCCEEDED(m_pVB->Lock(0, 0, (void**)&pVertices, 0)))
        {
            for (int i = 0; i < 4; ++i)
                pVertices[i].dwColor = m_sOptions.color;
            m_pVB->Unlock();
        }
    }
        break;
    case CVIBuffer::Buffer_Texture:
    {
        VTXTEX* pVertices = nullptr;
        if (SUCCEEDED(m_pVB->Lock(0, 0, (void**)&pVertices, 0)))
        {
            for (int i = 0; i < 4; ++i)
                pVertices[i].color = m_sOptions.color.dColor();

            m_pVB->Unlock();
        }
    }
        break;
    default:
        break;
    }
}

CComponent* CRectCol::Clone()
{
    return new CRectCol(*this);
}

HRESULT CRectCol::ReadyColorRect()
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

    if (m_pGameObject->getName() == L"Player")
        int a = 0;

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

HRESULT CRectCol::ReadyTextureRect()
{
    m_sOptions.vtxSize = sizeof(VTXTEX);
    m_sOptions.vtxCnt = 4;
    m_sOptions.triCnt = 2;
    m_sOptions.fvf = FVF_TEX;
    m_sOptions.idxSize = sizeof(INDEX16);
    m_sOptions.idxFmt = D3DFMT_INDEX16;

    if (FAILED(CVIBuffer::Ready_Buffer()))
        return E_FAIL;

    VTXTEX vertices[4] =
    {
        { { -0.5f,  0.5f, 0.f }, m_sOptions.color.dColor(), {0.f, 0.f}},
        { {  0.5f,  0.5f, 0.f }, m_sOptions.color.dColor(), { 1.f, 0.f } },
        { {  0.5f, -0.5f, 0.f }, m_sOptions.color.dColor(), { 1.f, 1.f } },
        { { -0.5f, -0.5f, 0.f }, m_sOptions.color.dColor(), { 0.f, 1.f } }
    };

    if (FAILED(FillVertexBuffer(vertices, sizeof(vertices))))
        return E_FAIL;

    INDEX16 indices[2] = { {0,1,2}, {0,2,3} };
    if (FAILED(FillIndexBuffer(indices, sizeof(indices))))
        return E_FAIL;

    return S_OK;
}
