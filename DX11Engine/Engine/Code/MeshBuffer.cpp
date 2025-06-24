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

	if (_description.indexCount > 0 && _description.indices != nullptr)
	{
		size_t indexSize = sizeof(UINT) * _description.indexCount;
		m_pIndexSysMem = malloc(indexSize);
		memcpy(m_pIndexSysMem, _description.indices, indexSize);
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
    static const VertexTexNormalBuffer cubeVertices[24] =
    {
        // 앞(-Z)
        {{-1, -1, -1}, { 0,  0, -1}, {0,1}},
        {{ 1, -1, -1}, { 0,  0, -1}, {1,1}},
        {{ 1,  1, -1}, { 0,  0, -1}, {1,0}},
        {{-1,  1, -1}, { 0,  0, -1}, {0,0}},

        // 뒤(+Z)
        {{ 1, -1,  1}, { 0,  0,  1}, {0,1}},
        {{-1, -1,  1}, { 0,  0,  1}, {1,1}},
        {{-1,  1,  1}, { 0,  0,  1}, {1,0}},
        {{ 1,  1,  1}, { 0,  0,  1}, {0,0}},

        // 왼(-X)
        {{-1, -1,  1}, {-1,  0,  0}, {0,1}},
        {{-1, -1, -1}, {-1,  0,  0}, {1,1}},
        {{-1,  1, -1}, {-1,  0,  0}, {1,0}},
        {{-1,  1,  1}, {-1,  0,  0}, {0,0}},

        // 오(+X)
        {{ 1, -1, -1}, { 1,  0,  0}, {0,1}},
        {{ 1, -1,  1}, { 1,  0,  0}, {1,1}},
        {{ 1,  1,  1}, { 1,  0,  0}, {1,0}},
        {{ 1,  1, -1}, { 1,  0,  0}, {0,0}},

        // 위(+Y)
        {{-1,  1, -1}, { 0,  1,  0}, {0,1}},
        {{ 1,  1, -1}, { 0,  1,  0}, {1,1}},
        {{ 1,  1,  1}, { 0,  1,  0}, {1,0}},
        {{-1,  1,  1}, { 0,  1,  0}, {0,0}},

        // 아래(-Y)
        {{-1, -1,  1}, { 0, -1,  0}, {0,1}},
        {{ 1, -1,  1}, { 0, -1,  0}, {1,1}},
        {{ 1, -1, -1}, { 0, -1,  0}, {1,0}},
        {{-1, -1, -1}, { 0, -1,  0}, {0,0}},
    };

    static UINT cubeIndices[36] =
    {
         0,  1,  2,  0,  2,  3,   // 앞
         4,  5,  6,  4,  6,  7,   // 뒤
         8,  9, 10,  8, 10, 11,   // 왼
        12, 13, 14, 12, 14, 15,   // 오
        16, 17, 18, 16, 18, 19,   // 위
        20, 21, 22, 20, 22, 23    // 아래
    };

    CMeshBuffer::MESHBUFFERDESC desc{};
    desc.topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
    desc.vertexSize = sizeof(VertexTexNormalBuffer);
    desc.vertextCount = 8;
    desc.indexCount = _countof(cubeIndices);
    desc.indices = cubeIndices;

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
