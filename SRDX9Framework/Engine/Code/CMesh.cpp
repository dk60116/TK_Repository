#include "CMesh.h"

CMesh::CMesh()
    : m_eMeshType(CUBE)
{
}

CMesh::~CMesh()
{
    OnDestroy();
}

void CMesh::Awake()
{
}

void CMesh::Render()
{
}

HRESULT CMesh::Ready_Buffer()
{
    return CVIBuffer::Ready_Buffer();
}

void CMesh::UpdateColor()
{
}

HRESULT CMesh::Ready_Mesh()
{
    switch (m_eMeshType)
    {
    case CMesh::CUBE:
        return Create_Cube();
    case CMesh::SPHERE:
        return Create_Sphere();
    case CMesh::CAPSULE:
        return Create_Capsule();
    case CMesh::CYLINDER:
        return Create_Cylinder();
    case CMesh::PLANE:
        return Create_Plane();
    case CMesh::QUAD:
        return Create_Quad();
    default:
        break;
    }

    return E_FAIL;
}

HRESULT CMesh::Create_Mesh(const void* _pVertices, UINT vertexSize, UINT vertexCount, const void* _pIndices, UINT indexSize, UINT _indexCount, DWORD _fvf, D3DFORMAT _idxFormat)
{
	return E_NOTIMPL;
}

HRESULT CMesh::Create_Cube()
{
    const _float s = 0.5f;

    D3DCOLOR color = ColorValue::white().dColor();

    static const VTXTEX vertices[] =
    {
        // Front
        { {-s, s, s}, ColorValue::blue().dColor(), {0.f, 0.f}},
        { {s, s, s}, ColorValue::blue().dColor(), {1.f, 0.f} },
        { {s, -s, s}, ColorValue::blue().dColor(), {1.f, 1.f} },
        { {-s, -s, s}, ColorValue::blue().dColor(), {0.f, 1.f} },

        // Back
        { {s, s, -s},  ColorValue::red().dColor(), {0.f, 0.f} },
        { {-s, s, -s}, ColorValue::red().dColor(), {1.f, 0.f} },
        { {-s, -s, -s},ColorValue::red().dColor(), {1.f, 1.f} },
        { {s, -s, -s}, ColorValue::red().dColor(), {0.f, 1.f} },

        // Top
        { {-s, s, -s}, ColorValue::yellow().dColor(), {0.f, 0.f} },
        { {s, s, -s},  ColorValue::yellow().dColor(), {1.f, 0.f} },
        { {s, s, s},   ColorValue::yellow().dColor(), {1.f, 1.f} },
        { {-s, s, s},  ColorValue::yellow().dColor(), {0.f, 1.f} },

        // Bottom
        { {-s, -s, s}, ColorValue::green().dColor(), {0.f, 0.f} },
        { {s, -s, s},  ColorValue::green().dColor(), {1.f, 0.f} },
        { {s, -s, -s}, ColorValue::green().dColor(), {1.f, 1.f} },
        { {-s, -s, -s},ColorValue::green().dColor(), {0.f, 1.f} },

        // Left
        { {-s, s, -s},  ColorValue::black().dColor(), {0.f, 0.f} },
        { {-s, s, s},  ColorValue::black().dColor(), {1.f, 0.f} },
        { {-s, -s, s}, ColorValue::black().dColor(), {1.f, 1.f} },
        { {-s, -s, -s},ColorValue::black().dColor(), {0.f, 1.f} },

        // Right
        { {s, s, s},   color, {0.f, 0.f} },
        { {s, s, -s},  color, {1.f, 0.f} },
        { {s, -s, -s}, color, {1.f, 1.f} },
        { {s, -s, s},  color, {0.f, 1.f} },
    };

    static const INDEX16 indices[] =
    {
        {0, 2, 1}, {0, 3, 2},
        {4, 6, 5}, {4, 7, 6},
        {8, 10, 9}, {8, 11, 10},
        {12, 14, 13}, {12, 15, 14},
        {16, 18, 17}, {16, 19, 18},
        {20, 22, 21}, {20, 23, 22},
    };

    m_sOptions.vtxCnt = 24;
    m_sOptions.vtxSize = sizeof(VTXTEX);
    m_sOptions.fvf = FVF_TEX;
    m_sOptions.triCnt = 12;
    m_sOptions.idxFmt = D3DFMT_INDEX16;
    m_sOptions.idxSize = sizeof(INDEX16);

    if (FAILED(Ready_Buffer()))
        return E_FAIL;

    if (FAILED(FillVertexBuffer(vertices, sizeof(vertices))))
        return E_FAIL;

    if (FAILED(FillIndexBuffer(indices, sizeof(indices))))
        return E_FAIL;

    return S_OK;
}

HRESULT CMesh::Create_Sphere()
{
	return E_NOTIMPL;
}

HRESULT CMesh::Create_Capsule()
{
	return E_NOTIMPL;
}

HRESULT CMesh::Create_Cylinder()
{
	return E_NOTIMPL;
}

HRESULT CMesh::Create_Plane()
{
	return E_NOTIMPL;
}

HRESULT CMesh::Create_Quad()
{
	return E_NOTIMPL;
}

HRESULT CMesh::Create_Custom()
{
    return E_NOTIMPL;
}