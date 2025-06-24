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

    // ───────────────────────────────
    ID3D11Device* device = CGraphicDevice::GetInstance().Get_Device();

    // VertexBuffer 생성
    D3D11_BUFFER_DESC vbDesc = {};
    vbDesc.ByteWidth = static_cast<UINT>(_description.vertexSize * _description.vertextCount);
    vbDesc.Usage = D3D11_USAGE_DEFAULT;
    vbDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

    D3D11_SUBRESOURCE_DATA vbData = {};
    vbData.pSysMem = _vertices;

    if (FAILED(device->CreateBuffer(&vbDesc, &vbData, &m_pVertexBuffer)))
        return E_FAIL;

    // IndexBuffer 생성
    if (_description.indexCount > 0 && _description.indices)
    {
        D3D11_BUFFER_DESC ibDesc = {};
        ibDesc.ByteWidth = sizeof(UINT) * _description.indexCount;
        ibDesc.Usage = D3D11_USAGE_DEFAULT;
        ibDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

        D3D11_SUBRESOURCE_DATA ibData = {};
        ibData.pSysMem = _description.indices;

        if (FAILED(device->CreateBuffer(&ibDesc, &ibData, &m_pIndexBuffer)))
            return E_FAIL;
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

void CMeshBuffer::Render()
{
    if (!m_pVertexBuffer)
        return;

    UINT stride = m_sInfo.vertexSize;
    UINT offset = 0;

    CGraphicDevice::GetInstance().Get_Context()->IASetVertexBuffers
    (
        0, 1, m_pVertexBuffer.GetAddressOf(), &stride, &offset
    );

    if (m_pIndexBuffer)
    {
        CGraphicDevice::GetInstance().Get_Context()->IASetIndexBuffer
        (m_pIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
    }

    CGraphicDevice::GetInstance().Get_Context()->IASetPrimitiveTopology(m_sInfo.topology);

    if (m_pIndexBuffer)
    {
        CGraphicDevice::GetInstance().Get_Context()->DrawIndexed(m_sInfo.indexCount, 0, 0);
    }
    else
    {
        CGraphicDevice::GetInstance().Get_Context()->Draw(m_sInfo.vertextCount, 0);
    }
}

CMeshBuffer* CMeshBuffer::CreateCube(CMeshFilter* _filter)
{
    static const VertexTexNormalTangentBuffer cubeVertices[24] =
    {
        // 앞(-Z)
            {{-1, -1, -1}, { 0,  0, -1}, {0, 1}, {1, 0, 0}},
            {{ 1, -1, -1}, { 0,  0, -1}, {1, 1}, {1, 0, 0}},
            {{ 1,  1, -1}, { 0,  0, -1}, {1, 0}, {1, 0, 0}},
            {{-1,  1, -1}, { 0,  0, -1}, {0, 0}, {1, 0, 0}},
                                                        
            // 뒤(+Z)                                   
            {{ 1, -1,  1}, { 0,  0,  1}, {0, 1}, {-1 ,0 ,0}},
            {{-1, -1,  1}, { 0,  0,  1}, {1, 1}, {-1 ,0 ,0}},
            {{-1,  1,  1}, { 0,  0,  1}, {1, 0}, {-1 ,0 ,0}},
            {{ 1,  1,  1}, { 0,  0,  1}, {0, 0}, {-1 ,0 ,0}},
                                                        
            // 왼(-X)                                   
            {{-1, -1,  1}, {-1,  0,  0}, {0, 1}, {0, 0, -1}},
            {{-1, -1, -1}, {-1,  0,  0}, {1, 1}, {0, 0, -1}},
            {{-1,  1, -1}, {-1,  0,  0}, {1, 0}, {0, 0, -1}},
            {{-1,  1,  1}, {-1,  0,  0}, {0, 0}, {0, 0, -1}},
                                                        
            // 오(+X)                                   
            {{ 1, -1, -1}, { 1,  0,  0}, {0, 1}, {0, 0, 1}},
            {{ 1, -1,  1}, { 1,  0,  0}, {1, 1}, {0, 0, 1}},
            {{ 1,  1,  1}, { 1,  0,  0}, {1, 0}, {0, 0, 1}},
            {{ 1,  1, -1}, { 1,  0,  0}, {0, 0}, {0, 0, 1}},
                                                        
            // 위(+Y)                                   
            {{-1,  1, -1}, { 0,  1,  0}, {0, 1}, {1, 0, 0}},
            {{ 1,  1, -1}, { 0,  1,  0}, {1, 1}, {1, 0, 0}},
            {{ 1,  1,  1}, { 0,  1,  0}, {1, 0}, {1, 0, 0}},
            {{-1,  1,  1}, { 0,  1,  0}, {0, 0}, {1, 0, 0}},
                                                        
            // 아래(-Y)                                 
            {{-1, -1,  1}, { 0, -1,  0}, {0, 1}, {1, 0, 0}},
            {{ 1, -1,  1}, { 0, -1,  0}, {1, 1}, {1, 0, 0}},
            {{ 1, -1, -1}, { 0, -1,  0}, {1, 0}, {1, 0, 0}},
            {{-1, -1, -1}, { 0, -1,  0}, {0, 0}, {1, 0, 0}},
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
    desc.vertexSize = sizeof(VertexTexNormalTangentBuffer);
    desc.vertextCount = 24;
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

CMeshBuffer* CMeshBuffer::CreateSphere(CMeshFilter* _filter)
{
    return nullptr;
}

const CMeshBuffer::MESHBUFFERDESC& CMeshBuffer::Get_Info()
{
	return m_sInfo;
}

ID3D11Buffer* CMeshBuffer::Get_VertexBuffer() const
{
	return m_pVertexBuffer.Get();
}

ID3D11Buffer* CMeshBuffer::Get_IndexBuffer() const
{
	return m_pIndexBuffer.Get();
}
