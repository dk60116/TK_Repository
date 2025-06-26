#include "epch.h"
#include "MeshBuffer.h"

#ifdef new
#undef new
#endif

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

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

    // ¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡
    ID3D11Device* device = CGraphicDevice::GetInstance().Get_Device();

    // VertexBuffer »ý¼º
    D3D11_BUFFER_DESC vbDesc = {};
    vbDesc.ByteWidth = static_cast<UINT>(_description.vertexSize * _description.vertextCount);
    vbDesc.Usage = D3D11_USAGE_DEFAULT;
    vbDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

    D3D11_SUBRESOURCE_DATA vbData = {};
    vbData.pSysMem = _vertices;

    if (FAILED(device->CreateBuffer(&vbDesc, &vbData, &m_pVertexBuffer)))
        return E_FAIL;

    // IndexBuffer »ý¼º
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
        CGraphicDevice::GetInstance().Get_Context()->DrawIndexed(m_sInfo.indexCount, 0, 0);
    else
        CGraphicDevice::GetInstance().Get_Context()->Draw(m_sInfo.vertextCount, 0);
}

CMeshBuffer* CMeshBuffer::CreateCube(CMeshFilter* _filter)
{
    const _float length = 0.5f;

    static const VertexTexNormalTangentBuffer cubeVertices[24] =
    {
        // ¾Õ(-Z)
            {{-length, -length, -length}, { 0,  0, -1}, {0, 1}, {1, 0, 0}},
            {{ length, -length, -length}, { 0,  0, -1}, {1, 1}, {1, 0, 0}},
            {{ length,  length, -length}, { 0,  0, -1}, {1, 0}, {1, 0, 0}},
            {{-length,  length, -length}, { 0,  0, -1}, {0, 0}, {1, 0, 0}},
                                                        
            // µÚ(+Z)                                   
            {{ length, -length,  length}, { 0,  0,  1}, {0, 1}, {-1 ,0 ,0}},
            {{-length, -length,  length}, { 0,  0,  1}, {1, 1}, {-1 ,0 ,0}},
            {{-length,  length,  length}, { 0,  0,  1}, {1, 0}, {-1 ,0 ,0}},
            {{ length,  length,  length}, { 0,  0,  1}, {0, 0}, {-1 ,0 ,0}},
                                                        
            // ¿ÞÂÊ(-X)                                   
            {{-length, -length,  length}, {-1,  0,  0}, {0, 1}, {0, 0, -1}},
            {{-length, -length, -length}, {-1,  0,  0}, {1, 1}, {0, 0, -1}},
            {{-length,  length, -length}, {-1,  0,  0}, {1, 0}, {0, 0, -1}},
            {{-length,  length,  length}, {-1,  0,  0}, {0, 0}, {0, 0, -1}},
                                
            // ¿À¸¥ÂÊ(+X)                                   
            {{ length, -length, -length}, { 1,  0,  0}, {0, 1}, {0, 0, 1}},
            {{ length, -length,  length}, { 1,  0,  0}, {1, 1}, {0, 0, 1}},
            {{ length,  length,  length}, { 1,  0,  0}, {1, 0}, {0, 0, 1}},
            {{ length,  length, -length}, { 1,  0,  0}, {0, 0}, {0, 0, 1}},
                 
            // À§(+Y)                    
            {{-length,  length, -length}, { 0,  1,  0}, {0, 1}, {1, 0, 0}},
            {{ length,  length, -length}, { 0,  1,  0}, {1, 1}, {1, 0, 0}},
            {{ length,  length,  length}, { 0,  1,  0}, {1, 0}, {1, 0, 0}},
            {{-length,  length,  length}, { 0,  1,  0}, {0, 0}, {1, 0, 0}},
                
            // ¾Æ·¡(-Y)                      
            {{-length, -length,  length}, { 0, -1,  0}, {0, 1}, {1, 0, 0}},
            {{ length, -length,  length}, { 0, -1,  0}, {1, 1}, {1, 0, 0}},
            {{ length, -length, -length}, { 0, -1,  0}, {1, 0}, {1, 0, 0}},
            {{-length, -length, -length}, { 0, -1,  0}, {0, 0}, {1, 0, 0}},
    };

    static UINT cubeIndices[36] =
    {
         0,  1,  2,  0,  2,  3,   // ¾Õ
         4,  5,  6,  4,  6,  7,   // µÚ
         8,  9, 10,  8, 10, 11,   // ¿Þ
        12, 13, 14, 12, 14, 15,   // ¿À
        16, 17, 18, 16, 18, 19,   // À§
        20, 21, 22, 20, 22, 23    // ¾Æ·¡
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

CMeshBuffer* CMeshBuffer::CreateTriangle(CMeshFilter* _filter)
{
    struct Vertex { XMFLOAT3 pos; };

    const _float length = 0.5f;

    Vertex triVerts[3] = 
    {
        { XMFLOAT3(0.f, length, length) },
        { XMFLOAT3(length, -length, 0.0f) },
        { XMFLOAT3(-length, -length, 0.0f) }
    };

    // ¹öÆÛ ¼³¸í
    CMeshBuffer::MESHBUFFERDESC desc{};
    desc.topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
    desc.vertexSize = sizeof(Vertex);
    desc.vertextCount = 3;
    desc.indexCount = 0;
    desc.indices = nullptr;

    CMeshBuffer* newBuffer = new CMeshBuffer();

    newBuffer->m_pFilter = _filter;

    if (FAILED(newBuffer->Initialize(triVerts, desc)))
    {
        Safe_Release(newBuffer);
        return nullptr;
    }

    return newBuffer;
}

CMeshBuffer* CMeshBuffer::CreateObjectMesh(CMeshFilter* _filter, const string& _filePath, const _float _scaleFactor)
{
    using VTX = VertexTexNormalTangentBuffer;

    /* 1) ¦¡¦¡ ¸ðµ¨ ·Îµù ¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡ */
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile
    (
        _filePath,
        aiProcess_Triangulate |   // ¸ðµç ¸éÀ» »ï°¢ÇüÈ­
        aiProcess_JoinIdenticalVertices |   // Áßº¹ Á¤Á¡ º´ÇÕ
        aiProcess_GenNormals |   // ³ë¸Ö ¾øÀ¸¸é »ý¼º
        aiProcess_CalcTangentSpace |   // ÅØ½ºÃ³ ÁÂÇ¥ ±â¹Ý ÅºÁ¨Æ®
        aiProcess_ConvertToLeftHanded |   // DirectX ÁÂÇ¥°è
        aiProcess_FlipUVs
    );                  // Blender->DX11 º¸Åë µÚÁýÈû

    if (!scene || !scene->HasMeshes())
        return nullptr;

    /* 2) ¦¡¦¡ Á¤Á¡ ¡¤ ÀÎµ¦½º ¸ðÀ¸±â ¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡ */
    vector<VTX>  vertices;
    vector<UINT> indices;

    auto copyMesh = [&](const aiMesh* mesh)
        {
            const UINT base = static_cast<UINT>(vertices.size());

            /* Á¤Á¡ */
            for (UINT i = 0; i < mesh->mNumVertices; ++i)
            {
                VTX v{};
                /* À§Ä¡ */
                v.position = { mesh->mVertices[i].x,
                          mesh->mVertices[i].y,
                          mesh->mVertices[i].z };

                v.position.x *= _scaleFactor;
                v.position.y *= _scaleFactor;
                v.position.z *= _scaleFactor;
                                
                /* ³ë¸Ö *
                if (mesh->HasNormals())
                    v.normal = { mesh->mNormals[i].x,
                                 mesh->mNormals[i].y,
                                 mesh->mNormals[i].z };
                else
                    v.normal = { 0,0,0 };

                /* UV(0) */
                if (mesh->HasTextureCoords(0))
                    v.uv = { mesh->mTextureCoords[0][i].x,
                             mesh->mTextureCoords[0][i].y };
                else
                    v.uv = { 0,0 };

                /* ÅºÁ¨Æ® */
                if (mesh->HasTangentsAndBitangents())
                    v.tangent = { mesh->mTangents[i].x,
                                  mesh->mTangents[i].y,
                                  mesh->mTangents[i].z };
                else
                    v.tangent = { 0,0,0 };

                vertices.emplace_back(v);
            }

            /* ÀÎµ¦½º(¾ó±¼) */
            for (UINT f = 0; f < mesh->mNumFaces; ++f)
            {
                const aiFace& face = mesh->mFaces[f];
                if (face.mNumIndices != 3) continue;     // »ï°¢¸é¸¸
                indices.push_back(base + face.mIndices[0]);
                indices.push_back(base + face.mIndices[1]);
                indices.push_back(base + face.mIndices[2]);
            }
        };

    /* ¾À ³ëµå Àç±Í ¼øÈ¸ */
    function<void(const aiNode*)> traverse = [&](const aiNode* node)
        {
            for (UINT m = 0; m < node->mNumMeshes; ++m)
                copyMesh(scene->mMeshes[node->mMeshes[m]]);

            for (UINT c = 0; c < node->mNumChildren; ++c)
                traverse(node->mChildren[c]);
        };

    traverse(scene->mRootNode);

    if (vertices.empty())
        return nullptr;

    /* 3) ¦¡¦¡ CMeshBuffer »ý¼º ¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡ */
    MESHBUFFERDESC desc{};
    desc.topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
    desc.vertexSize = sizeof(VTX);
    desc.vertextCount = static_cast<UINT>(vertices.size());
    desc.indexCount = static_cast<UINT>(indices.size());
    desc.indices = indices.data();   // Initialize ¾È¿¡¼­ º¹»çµÊ

    auto* newBuf = new CMeshBuffer();
    newBuf->m_pFilter = _filter;

    if (FAILED(newBuf->Initialize(vertices.data(), desc)))
    {
        Safe_Release(newBuf);
        return nullptr;
    }

    return newBuf;
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
