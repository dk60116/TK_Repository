#include "epch.h"
#include "MeshBuffer.h"

CMeshBuffer::CMeshBuffer()
	: m_pVertexBuffer(nullptr)
	, m_pIndexBuffer(nullptr)
	, m_pFilter(nullptr)
	, m_sInfo({})
{
}

CMeshBuffer::~CMeshBuffer()
{
	OnDestroy();
}

HRESULT CMeshBuffer::Initialize(const void* _vertices, MESHBUFFERDESC _description)
{
	if (!_vertices)
		return E_FAIL;

	if (_description.vertexSize == 0 || _description.vertextCount == 0)
		return E_FAIL;

	m_sInfo = _description;

	size_t size = _description.vertexSize * _description.vertextCount;

	m_pVertexSysMem = malloc(size);
	memcpy(m_pVertexSysMem, _vertices, size);

	if (_description.indexCount > 0 && _description.Indices != nullptr)
	{
		size_t indexSize = sizeof(UINT) * _description.indexCount;
		m_pIndexSysMem = malloc(indexSize);
		memcpy(m_pIndexSysMem, _description.Indices, indexSize);
	}

	return S_OK;
}

void CMeshBuffer::OnDestroy()
{
	if (m_pVertexSysMem)
	{
		free(m_pVertexSysMem);
		m_pVertexSysMem = nullptr;
	}

	if (m_pIndexSysMem)
	{
		free(m_pIndexSysMem);
		m_pIndexSysMem = nullptr;
	}
}

CMeshBuffer* CMeshBuffer::CreateCube(CMeshFilter* _filter)
{
    struct Vertex
    {
        vector3 position;
        vector3 normal;
        vector2 uv;
    };

    static Vertex cubeVertices[8] = 
    {
        {{-1,-1,-1}, {0,0,-1}, {0,1}}, {{1,-1,-1}, {0,0,-1}, {1,1}},
        {{1,1,-1}, {0,0,-1}, {1,0}}, {{-1,1,-1}, {0,0,-1}, {0,0}},
        {{-1,-1,1}, {0,0,1}, {0,1}}, {{1,-1,1}, {0,0,1}, {1,1}},
        {{1,1,1}, {0,0,1}, {1,0}}, {{-1,1,1}, {0,0,1}, {0,0}}
    };

    static UINT cubeIndices[] = 
    {
        // 앞면
        0, 1, 2, 0, 2, 3,
        // 뒷면
        5, 4, 7, 5, 7, 6,
        // 왼쪽
        4, 0, 3, 4, 3, 7,
        // 오른쪽
        1, 5, 6, 1, 6, 2,
        // 위
        3, 2, 6, 3, 6, 7,
        // 아래
        4, 5, 1, 4, 1, 0
    };

    CMeshBuffer::MESHBUFFERDESC desc{};
    desc.topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
    desc.vertexSize = sizeof(Vertex);
    desc.vertextCount = 8;
    desc.indexCount = _countof(cubeIndices);
    desc.Indices = cubeIndices;

    CMeshBuffer* newBuffer = new CMeshBuffer();
    newBuffer->m_pFilter = _filter;

    if (FAILED(newBuffer->Initialize(cubeVertices, desc)))
    {
        Safe_Release(newBuffer);
        return nullptr;
    }

    return newBuffer;
}

const CMeshBuffer::MESHBUFFERDESC& CMeshBuffer::Get_Info()
{
	return m_sInfo;
}

ID3D11Buffer* CMeshBuffer::Get_VertexBuffer()
{
	return m_pVertexBuffer.Get();
}

ID3D11Buffer* CMeshBuffer::Get_IndexBuffer()
{
	return m_pIndexBuffer.Get();
}
