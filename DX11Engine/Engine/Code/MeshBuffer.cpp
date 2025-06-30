#include "epch.h"
#include "MeshBuffer.h"
#include "SkinnedMeshBuffer.h"

CMeshBuffer::CMeshBuffer()
	: m_pVertexBuffer(nullptr)
	, m_pIndexBuffer(nullptr)
	, m_pFilter(nullptr)
	, m_sInfo({})
{
    m_strName = L"Mesh Buffer";
}

CMeshBuffer::~CMeshBuffer()
{
	OnDestroy();
}

CMeshBuffer* CMeshBuffer::Create(const wstring& _filePath)
{
    return new CMeshBuffer();
}

HRESULT CMeshBuffer::Initialize(const wstring& _name, wstring _filePath, void* _desc)
{
    if (FAILED(__super::Initialize(_name, _filePath, _desc)))
        return E_FAIL;

    MeshBufferInitiaizeInfo info = {};

    if (_filePath == L"../Assets/Cube")
        info = CreateCube();
    else
    {
        const _float scaleFactor = _desc ? *reinterpret_cast<_float*>(_desc) : 1.f;
        info = CreateObjectMesh(CEngineString::WStringToString(_filePath), scaleFactor);
    }

    if (!(info.buffer.size() > 0))
        return E_FAIL;

    if (info.desc.vertexSize == 0 || info.desc.vertextCount == 0)
        return E_FAIL;

    m_sInfo = info.desc;

    size_t size = info.desc.vertexSize * info.desc.vertextCount;

    m_pVertexSysMem = malloc(size);
    memcpy(m_pVertexSysMem, info.buffer.data(), size);

    if (info.desc.indexCount > 0 && !info.indices.empty())
    {
        size_t indexSize = sizeof(UINT) * info.desc.indexCount;
        m_pIndexSysMem = malloc(indexSize);
        memcpy(m_pIndexSysMem, info.indices.data(), indexSize);
    }

    // ¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡
    ID3D11Device* device = CGraphicDevice::GetInstance().Get_Device();

    // VertexBuffer »ý¼º
    D3D11_BUFFER_DESC vbDesc = {};
    vbDesc.ByteWidth = static_cast<UINT>(info.desc.vertexSize * info.desc.vertextCount);
    vbDesc.Usage = D3D11_USAGE_DEFAULT;
    vbDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

    D3D11_SUBRESOURCE_DATA vbData = {};
    vbData.pSysMem = info.buffer.data();

    if (FAILED(device->CreateBuffer(&vbDesc, &vbData, &m_pVertexBuffer)))
        return E_FAIL;

    // IndexBuffer »ý¼º
    if (info.desc.indexCount > 0 && info.indices.size() > 0)
    {
        D3D11_BUFFER_DESC ibDesc = {};
        ibDesc.ByteWidth = sizeof(UINT) * info.desc.indexCount;
        ibDesc.Usage = D3D11_USAGE_DEFAULT;
        ibDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

        D3D11_SUBRESOURCE_DATA ibData = {};
        ibData.pSysMem = info.indices.data();

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

    Safe_Release(m_pFilter);
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

CMeshBuffer::MeshBufferInitiaizeInfo CMeshBuffer::CreateCube()
{
    MeshBufferInitiaizeInfo info = {};

    const _float length = 0.5f;

    VertexTexNormalTangentBuffer cubeVertices[24] =
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
        2,1,0, 3,2,0,   // ¾Õ
        6,5,4, 7,6,4,   // µÚ
        10,9,8,11,10,8,  // ¿Þ
        14,13,12,15,14,12,// ¿À
        18,17,16,19,18,16,// À§
        22,21,20,23,22,20 // ¾Æ·¡
    };

    CMeshBuffer::MESHBUFFERDESC desc{};
    desc.topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
    desc.vertexSize = sizeof(VertexTexNormalTangentBuffer);
    desc.vertextCount = _countof(cubeVertices);
    desc.indexCount = _countof(cubeIndices);

    info.buffer.assign(begin(cubeVertices), end(cubeVertices));
    info.indices.assign(begin(cubeIndices), end(cubeIndices));
    info.desc = desc;

    return info;
}

CMeshBuffer::MeshBufferInitiaizeInfo CMeshBuffer::CreateSphere()
{
    MeshBufferInitiaizeInfo result = {};
    return result;
}

CMeshBuffer::MeshBufferInitiaizeInfo CMeshBuffer::CreateTriangle()
{
    MeshBufferInitiaizeInfo result = {};
    return result;

    //MeshBufferInitiaizeInfo info = {};

    //struct Vertex { XMFLOAT3 pos; };

    //const _float length = 0.5f;

    //Vertex triVerts[3] = 
    //{
    //    { XMFLOAT3(0.f, length, length) },
    //    { XMFLOAT3(length, -length, 0.0f) },
    //    { XMFLOAT3(-length, -length, 0.0f) }
    //};

    //info.buffer = triVerts;

    //// ¹öÆÛ ¼³¸í
    //CMeshBuffer::MESHBUFFERDESC desc{};
    //desc.topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
    //desc.vertexSize = sizeof(Vertex);
    //desc.vertextCount = 3;
    //desc.indexCount = 0;
    //desc.indices = nullptr;

    //info.buffer = 

    //if (FAILED(newBuffer->Initialize(triVerts, desc)))
    //{
    //    Safe_Release(newBuffer);
    //    return nullptr;
    //}

    //return newBuffer;
}

CMeshBuffer::MeshBufferInitiaizeInfo CMeshBuffer::CreateObjectMesh(const string& _filePath, const _float _scaleFactor)
{
    MeshBufferInitiaizeInfo info = {};

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

    /* 3) ¦¡¦¡ CMeshBuffer »ý¼º ¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡ */
    MESHBUFFERDESC desc{};
    desc.topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
    desc.vertexSize = sizeof(VTX);
    desc.vertextCount = static_cast<UINT>(vertices.size());
    desc.indexCount = static_cast<UINT>(indices.size());

    info.buffer.assign(vertices.begin(), vertices.end());
    info.indices.assign(indices.begin(), indices.end());
    info.desc = desc;

    return info;
}

const CMeshBuffer::MESHBUFFERDESC& CMeshBuffer::Get_Info()
{
	return m_sInfo;
}

void CMeshBuffer::Set_Filter(CMeshFilter* _filter)
{
    m_pFilter = _filter;

    if (m_pFilter)
        m_pFilter->AddRef();
}

ID3D11Buffer* CMeshBuffer::Get_VertexBuffer() const
{
	return m_pVertexBuffer.Get();
}

ID3D11Buffer* CMeshBuffer::Get_IndexBuffer() const
{
	return m_pIndexBuffer.Get();
}
