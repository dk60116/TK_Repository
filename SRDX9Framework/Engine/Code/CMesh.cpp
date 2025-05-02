#include "CMesh.h"

CMesh::CMesh()
    : m_eMeshType(CUBE)
{
}

CMesh::~CMesh()
{
}

HRESULT CMesh::Ready_Buffer(LPDIRECT3DDEVICE9 _device)
{
    return __super::Ready_Buffer(_device);
}

void CMesh::UpdateColor()
{
}

HRESULT CMesh::Ready_Mesh(LPDIRECT3DDEVICE9 _device)
{
    switch (m_eMeshType)
    {
    case CMesh::CUBE:
        return Create_Cube(_device);
    case CMesh::SPHERE:
        return Create_Sphere(_device);
    case CMesh::CAPSULE:
        return Create_Capsule(_device);
    case CMesh::CYLINDER:
        return Create_Cylinder(_device);
    case CMesh::PLANE:
        return Create_Plane(_device);
    case CMesh::QUAD:
        return Create_Quad(_device);
    case CMesh::MODEL:
        break;
    default:
        break;
    }

    return E_FAIL;
}

HRESULT CMesh::Create_Mesh(const void* _pVertices, UINT _vertexSize, UINT _vertexCount, const void* _pIndices, UINT _indexSize, UINT _indexCount, DWORD _fvf, D3DFORMAT _idxFormat)
{
	return E_NOTIMPL;
}

HRESULT CMesh::Create_Cube(LPDIRECT3DDEVICE9 _device)
{
    const _float s = 0.5f;

    D3DCOLOR color = ColorValue::white().dColor();

    static const VTLTEX vertices[] =
    {
        // Front
        { {-s, s, s}, vector3::forward().dVector(), {0.f, 0.f}},
        { {s, s, s}, vector3::forward().dVector(), {1.f, 0.f} },
        { {s, -s, s}, vector3::forward().dVector(), {1.f, 1.f} },
        { {-s, -s, s}, vector3::forward().dVector(), {0.f, 1.f} },

        // Back
        { {s, s, -s},  vector3::back().dVector(), {0.f, 0.f} },
        { {-s, s, -s}, vector3::back().dVector(), {1.f, 0.f} },
        { {-s, -s, -s},vector3::back().dVector(), {1.f, 1.f} },
        { {s, -s, -s}, vector3::back().dVector(), {0.f, 1.f} },

        // Top
        { {-s, s, -s}, vector3::up().dVector(), {0.f, 0.f} },
        { {s, s, -s},  vector3::up().dVector(), {1.f, 0.f} },
        { {s, s, s},   vector3::up().dVector(), {1.f, 1.f} },
        { {-s, s, s},  vector3::up().dVector(), {0.f, 1.f} },

        // Bottom
        { {-s, -s, s}, vector3::down().dVector(), {0.f, 0.f} },
        { {s, -s, s}, vector3::down().dVector(), {1.f, 0.f} },
        { {s, -s, -s}, vector3::down().dVector(), {1.f, 1.f} },
        { {-s, -s, -s}, vector3::down().dVector(), {0.f, 1.f} },

        // Left
        { {-s, s, -s},  vector3::left().dVector(), {0.f, 0.f} },
        { {-s, s, s},  vector3::left().dVector(), {1.f, 0.f} },
        { {-s, -s, s}, vector3::left().dVector(), {1.f, 1.f} },
        { {-s, -s, -s}, vector3::left().dVector(), {0.f, 1.f} },

        // Right
        { {s, s, s},   vector3::right().dVector(), {0.f, 0.f} },
        { {s, s, -s},  vector3::right().dVector(), {1.f, 0.f} },
        { {s, -s, -s}, vector3::right().dVector(), {1.f, 1.f} },
        { {s, -s, s},  vector3::right().dVector(), {0.f, 1.f} },
    };

    static const INDEX16 indices[] =
    {
        {0, 1, 2}, {0, 2, 3},             // Front
        {4, 5, 6}, {4, 6, 7},             // Back
        {8, 9, 10}, {8, 10, 11},          // Top
        {12, 13, 14}, {12, 14, 15},       // Bottom
        {16, 17, 18}, {16, 18, 19},       // Left
        {20, 21, 22}, {20, 22, 23},       // Right
    };

    m_sOptions.vtxCnt = 24;
    m_sOptions.vtxSize = sizeof(VTLTEX);
    m_sOptions.fvf = FVF_LTEX;
    m_sOptions.triCnt = 12;
    m_sOptions.idxFmt = D3DFMT_INDEX16;
    m_sOptions.idxSize = sizeof(INDEX16);

    if (FAILED(Ready_Buffer(_device)))
        return E_FAIL;

    if (FAILED(FillVertexBuffer(vertices, sizeof(vertices))))
        return E_FAIL;

    if (FAILED(FillIndexBuffer(indices, sizeof(indices))))
        return E_FAIL;

    return S_OK;
}

HRESULT CMesh::Create_Sphere(LPDIRECT3DDEVICE9 _device)
{
    const int stacks = 18;
    const int slices = 36;
    const float radius = 0.5f;

    vector<VTLTEX> vertices;
    vector<INDEX16> indices;

    for (int stack = 0; stack <= stacks; ++stack)
    {
        float phi = D3DX_PI * stack / stacks;
        float y = cosf(phi);
        float r = sinf(phi);

        for (int slice = 0; slice <= slices; ++slice)
        {
            float theta = D3DX_PI * 2 * slice / slices;
            float x = r * cosf(theta);
            float z = r * sinf(theta);

            vector3 pos = vector3(x, y, z) * radius;
            vector3 normal = vector3(x, y, z).normalized();

            float u = static_cast<float>(slice) / slices;
            float v = static_cast<float>(stack) / stacks;

            vertices.push_back({ pos.dVector(), normal.dVector(), {u, v} });
        }
    }

    for (int stack = 0; stack < stacks; ++stack)
    {
        for (int slice = 0; slice < slices; ++slice)
        {
            int first = (stack * (slices + 1)) + slice;
            int second = first + slices + 1;

            indices.push_back({ static_cast<WORD>(first), static_cast<WORD>(second), static_cast<WORD>(first + 1) });
            indices.push_back({ static_cast<WORD>(second), static_cast<WORD>(second + 1), static_cast<WORD>(first + 1) });
        }
    }

    m_sOptions.vtxCnt = static_cast<_uint>(vertices.size());
    m_sOptions.vtxSize = sizeof(VTLTEX);
    m_sOptions.fvf = FVF_LTEX;

    m_sOptions.triCnt = static_cast<_uint>(indices.size());
    m_sOptions.idxSize = sizeof(INDEX16);
    m_sOptions.idxFmt = D3DFMT_INDEX16;

    if (FAILED(Ready_Buffer(_device)))
        return E_FAIL;

    if (FAILED(FillVertexBuffer(vertices.data(), static_cast<UINT>(vertices.size() * sizeof(VTLTEX)))))
        return E_FAIL;

    if (FAILED(FillIndexBuffer(indices.data(), static_cast<UINT>(indices.size() * sizeof(INDEX16)))))
        return E_FAIL;

    return S_OK;
}

HRESULT CMesh::Create_Capsule(LPDIRECT3DDEVICE9 _device)
{
	return E_NOTIMPL;
}

HRESULT CMesh::Create_Cylinder(LPDIRECT3DDEVICE9 _device)
{
	return E_NOTIMPL;
}

HRESULT CMesh::Create_Plane(LPDIRECT3DDEVICE9 _device)
{
	return E_NOTIMPL;
}

HRESULT CMesh::Create_Quad(LPDIRECT3DDEVICE9 _device)
{
	return E_NOTIMPL;
}

HRESULT CMesh::Create_Custom(LPDIRECT3DDEVICE9 _device)
{
    return E_NOTIMPL;
}