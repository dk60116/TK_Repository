#include "epch.h"
#include "MeshBuffer.h"
#include "SkinnedMeshBuffer.h"

CMeshBuffer::CMeshBuffer()
	: m_pVertexBuffer(nullptr)
	, m_pIndexBuffer(nullptr)
	, m_sInfo({})
    , m_pVertexSysMem(nullptr)
    , m_pIndexSysMem(nullptr)
{
    m_strName = L"Mesh Buffer";
}

CMeshBuffer::~CMeshBuffer()
{
	OnDestroy();
}

CMeshBuffer* CMeshBuffer::Create()
{
    return new CMeshBuffer();
}

HRESULT CMeshBuffer::Initialize(const wstring& _name, const wstring& _filePath, void* _desc)
{
    if (FAILED(__super::Initialize(_name, _filePath, _desc)))
        return E_FAIL;

    MeshBufferInitiaizeInfo info = {};

    if (_filePath == L"../Assets/Line")
        info = CreateLine();
    else if (_filePath == L"../Assets/Rect")
        info = CreateRect();
    else if (_filePath == L"../Assets/LineRect")
        info = CreateLineRect();
    else if (_filePath == L"../Assets/Cube")
        info = CreateCube();
    else if (_filePath == L"../Assets/Quad")
        info = CreateQuad();
    else if (_filePath == L"../Assets/Terrain")
    {
        TERRAINBUFFERDESC terainDesc = {};

        if (_desc)
            terainDesc = *reinterpret_cast<TERRAINBUFFERDESC*>(_desc);

        info = CreateTerrain(terainDesc.landscape, terainDesc.portrait, 0);
    }
    else
        return S_OK;

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
        size_t indexSize = sizeof(_uint) * info.desc.indexCount;
        m_pIndexSysMem = malloc(indexSize);
        memcpy(m_pIndexSysMem, info.indices.data(), indexSize);
    }

    ID3D11Device* device = CGraphicDevice::GetInstance().Get_Device();

    // VertexBuffer 생성
    D3D11_BUFFER_DESC vbDesc = {};
    vbDesc.ByteWidth = static_cast<_uint>(info.desc.vertexSize * info.desc.vertextCount);
    vbDesc.Usage = D3D11_USAGE_DEFAULT;
    vbDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

    D3D11_SUBRESOURCE_DATA vbData = {};
    vbData.pSysMem = info.buffer.data();

    HRESULT hr = S_OK;

    hr = device->CreateBuffer(&vbDesc, &vbData, &m_pVertexBuffer);

    // IndexBuffer 생성
    if (info.desc.indexCount > 0 && info.indices.size() > 0)
    {
        D3D11_BUFFER_DESC ibDesc = {};
        ibDesc.ByteWidth = sizeof(_uint) * info.desc.indexCount;
        ibDesc.Usage = D3D11_USAGE_DEFAULT;
        ibDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

        D3D11_SUBRESOURCE_DATA ibData = {};
        ibData.pSysMem = info.indices.data();

        hr = device->CreateBuffer(&ibDesc, &ibData, &m_pIndexBuffer);
    }

    if (FAILED(hr))
    {
        CDebug::LogError(L"Assimp MeshBuffer load failed: " + m_strFilePath);
        return E_FAIL;
    }

    return hr;
}

HRESULT CMeshBuffer::Initailize_Custom(MeshBufferInitiaizeInfo _info, void* _desc)
{
    if (!(_info.buffer.size() > 0))
        return E_FAIL;

    if (_info.desc.vertexSize == 0 || _info.desc.vertextCount == 0)
        return E_FAIL;

    m_sInfo = {};
    m_sInfo = _info.desc;

    size_t size = _info.desc.vertexSize * _info.desc.vertextCount;

    m_pVertexSysMem = malloc(size);
    memcpy(m_pVertexSysMem, _info.buffer.data(), size);

    if (_info.desc.indexCount > 0 && !_info.indices.empty())
    {
        size_t indexSize = sizeof(_uint) * _info.desc.indexCount;
        m_pIndexSysMem = malloc(indexSize);
        memcpy(m_pIndexSysMem, _info.indices.data(), indexSize);
    }

    ID3D11Device* device = CGraphicDevice::GetInstance().Get_Device();

    // VertexBuffer 생성
    D3D11_BUFFER_DESC vbDesc = {};
    vbDesc.ByteWidth = static_cast<_uint>(_info.desc.vertexSize * _info.desc.vertextCount);
    vbDesc.Usage = D3D11_USAGE_DEFAULT;
    vbDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

    D3D11_SUBRESOURCE_DATA vbData = {};
    vbData.pSysMem = _info.buffer.data();

    HRESULT hr = S_OK;

    hr = device->CreateBuffer(&vbDesc, &vbData, &m_pVertexBuffer);

    // IndexBuffer 생성
    if (_info.desc.indexCount > 0 && _info.indices.size() > 0)
    {
        D3D11_BUFFER_DESC ibDesc = {};
        ibDesc.ByteWidth = sizeof(_uint) * _info.desc.indexCount;
        ibDesc.Usage = D3D11_USAGE_DEFAULT;
        ibDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

        D3D11_SUBRESOURCE_DATA ibData = {};
        ibData.pSysMem = _info.indices.data();

        hr = device->CreateBuffer(&ibDesc, &ibData, &m_pIndexBuffer);
    }

    if (FAILED(hr))
    {
        CDebug::LogError(L"Assimp MeshBuffer load failed: " + m_strFilePath);
        return E_FAIL;
    }

    return hr;
}

wstring CMeshBuffer::FindMeshName(const aiScene* scene, _uint meshIndex, aiNode* node)
{
    if (!node)
        node = scene->mRootNode;

    for (unsigned int i = 0; i < node->mNumMeshes; ++i)
    {
        if (node->mMeshes[i] == meshIndex)
            return CEngineString::StringToWString(node->mName.C_Str());
    }

    for (unsigned int i = 0; i < node->mNumChildren; ++i)
    {
        wstring result = FindMeshName(scene, meshIndex, node->mChildren[i]);
        if (!result.empty())
            return result;
    }

    return L"";
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
    {
        CDebug::LogError("Mesh buffer failed render - No vertex buffer");
        return;
    }

    _uint stride = m_sInfo.vertexSize;
    _uint offset = 0;

    CGraphicDevice::GetInstance().Get_Context()->IASetVertexBuffers
    (
        0, 1, m_pVertexBuffer.GetAddressOf(), &stride, &offset
    );

    if (m_pIndexBuffer)
    {
        CGraphicDevice::GetInstance().Get_Context()->IASetIndexBuffer
        (m_pIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
    }

    if (!m_sInfo.useDeviceTopology)
        CGraphicDevice::GetInstance().Get_Context()->IASetPrimitiveTopology(m_sInfo.topology);

    if (m_pIndexBuffer)
        CGraphicDevice::GetInstance().Get_Context()->DrawIndexed(m_sInfo.indexCount, 0, 0);
    else
        CGraphicDevice::GetInstance().Get_Context()->Draw(m_sInfo.vertextCount, 0);
}

CMeshBuffer::MeshBufferInitiaizeInfo CMeshBuffer::CreateLine()
{
    MeshBufferInitiaizeInfo info = {};

    const _float length = 0.5f;

    VertexTexNormalTangentBuffer lineVertices[2] =
    {
        {{-length, 0, 0}, { 0, 0, 0}, {0, 0}, {0, 0, 0}},
        {{ length, 0, 0}, { 0, 0, 0}, {0, 0}, {0, 0, 0}}
    };

    CMeshBuffer::MESHBUFFERDESC desc{};
    desc.topology = D3D11_PRIMITIVE_TOPOLOGY_LINELIST;
    desc.vertexSize = sizeof(VertexTexNormalTangentBuffer);
    desc.vertextCount = _countof(lineVertices);
    desc.indexCount = 0;

    info.buffer.assign(reinterpret_cast<uint8_t*>(lineVertices), reinterpret_cast<uint8_t*>(lineVertices) + sizeof(lineVertices));

    info.desc = desc;

    return info;
}

CMeshBuffer::MeshBufferInitiaizeInfo CMeshBuffer::CreateLineRect()
{
    MeshBufferInitiaizeInfo info = {};

    const _float length = 0.5f;

    VertexTexNormalTangentBuffer rectVertices[8]
    {
        {{-length, length, 0}, { 0, 0, 0 }, { 0, 0 }, { 0, 0, 0 }},
        {{ length, length, 0}, { 0, 0, 0 }, { 0, 0 }, { 0, 0, 0 }},
        {{ length, length, 0}, { 0, 0, 0 }, { 0, 0 }, { 0, 0, 0 }},
        {{ length, -length, 0}, { 0, 0, 0 }, { 0, 0 }, { 0, 0, 0 }},
        {{ length, -length, 0}, { 0, 0, 0 }, { 0, 0 }, { 0, 0, 0 }},
        {{ -length, -length, 0}, { 0, 0, 0 }, { 0, 0 }, { 0, 0, 0 }},
        {{ -length, -length, 0}, { 0, 0, 0 }, { 0, 0 }, { 0, 0, 0 }},
        {{ -length, length, 0}, { 0, 0, 0 }, { 0, 0 }, { 0, 0, 0 }}
    };

    CMeshBuffer::MESHBUFFERDESC desc{};
    desc.topology = D3D11_PRIMITIVE_TOPOLOGY_LINELIST;
    desc.vertexSize = sizeof(VertexTexNormalTangentBuffer);
    desc.vertextCount = _countof(rectVertices);
    desc.indexCount = 0;

    info.buffer.assign(reinterpret_cast<uint8_t*>(rectVertices), reinterpret_cast<uint8_t*>(rectVertices) + sizeof(rectVertices));
    info.desc = desc;

    return info;
}

CMeshBuffer::MeshBufferInitiaizeInfo CMeshBuffer::CreateRect()
{
    MeshBufferInitiaizeInfo info = {};

    const _float length = 0.5f;

    VertexTexColorBuffer quadVertices[4] =
    {
            {{-length, -length, 0}, {0, 1}},
            {{ length, -length, 0}, {1, 1}},
            {{ length,  length, 0}, {1, 0}},
            {{-length,  length, 0}, {0, 0}}
    };

    static _uint quadIndices[6] =
    {
        2,1,0, 3,2,0
    };

    CMeshBuffer::MESHBUFFERDESC desc{};
    desc.topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
    desc.vertexSize = sizeof(VertexTexColorBuffer);
    desc.vertextCount = _countof(quadVertices);
    desc.indexCount = _countof(quadIndices);

    info.buffer.assign(reinterpret_cast<uint8_t*>(quadVertices), reinterpret_cast<uint8_t*>(quadVertices) + sizeof(quadVertices));
    info.indices.assign(begin(quadIndices), end(quadIndices));
    info.desc = desc;

    return info;
}

CMeshBuffer::MeshBufferInitiaizeInfo CMeshBuffer::CreateCube()
{
    MeshBufferInitiaizeInfo info = {};

    const _float length = 0.5f;

    VertexTexNormalTangentBuffer cubeVertices[24] =
    {
        // 앞(-Z)
            {{-length, -length, -length}, { 0,  0, -1}, {0, 1}, {1, 0, 0}},
            {{ length, -length, -length}, { 0,  0, -1}, {1, 1}, {1, 0, 0}},
            {{ length,  length, -length}, { 0,  0, -1}, {1, 0}, {1, 0, 0}},
            {{-length,  length, -length}, { 0,  0, -1}, {0, 0}, {1, 0, 0}},
                                                        
            // 뒤(+Z)                                   
            {{ length, -length,  length}, { 0,  0,  1}, {0, 1}, {-1 ,0 ,0}},
            {{-length, -length,  length}, { 0,  0,  1}, {1, 1}, {-1 ,0 ,0}},
            {{-length,  length,  length}, { 0,  0,  1}, {1, 0}, {-1 ,0 ,0}},
            {{ length,  length,  length}, { 0,  0,  1}, {0, 0}, {-1 ,0 ,0}},
                                                        
            // 왼쪽(-X)                                   
            {{-length, -length,  length}, {-1,  0,  0}, {0, 1}, {0, 0, -1}},
            {{-length, -length, -length}, {-1,  0,  0}, {1, 1}, {0, 0, -1}},
            {{-length,  length, -length}, {-1,  0,  0}, {1, 0}, {0, 0, -1}},
            {{-length,  length,  length}, {-1,  0,  0}, {0, 0}, {0, 0, -1}},
                                
            // 오른쪽(+X)                                   
            {{ length, -length, -length}, { 1,  0,  0}, {0, 1}, {0, 0, 1}},
            {{ length, -length,  length}, { 1,  0,  0}, {1, 1}, {0, 0, 1}},
            {{ length,  length,  length}, { 1,  0,  0}, {1, 0}, {0, 0, 1}},
            {{ length,  length, -length}, { 1,  0,  0}, {0, 0}, {0, 0, 1}},
                 
            // 위(+Y)                    
            {{-length,  length, -length}, { 0,  1,  0}, {0, 1}, {1, 0, 0}},
            {{ length,  length, -length}, { 0,  1,  0}, {1, 1}, {1, 0, 0}},
            {{ length,  length,  length}, { 0,  1,  0}, {1, 0}, {1, 0, 0}},
            {{-length,  length,  length}, { 0,  1,  0}, {0, 0}, {1, 0, 0}},
                
            // 아래(-Y)                      
            {{-length, -length,  length}, { 0, -1,  0}, {0, 1}, {1, 0, 0}},
            {{ length, -length,  length}, { 0, -1,  0}, {1, 1}, {1, 0, 0}},
            {{ length, -length, -length}, { 0, -1,  0}, {1, 0}, {1, 0, 0}},
            {{-length, -length, -length}, { 0, -1,  0}, {0, 0}, {1, 0, 0}},
    };

    static _uint cubeIndices[36] =
    {
        2,1,0, 3,2,0,   // 앞
        6,5,4, 7,6,4,   // 뒤
        10,9,8,11,10,8,  // 왼
        14,13,12,15,14,12,// 오
        18,17,16,19,18,16,// 위
        22,21,20,23,22,20 // 아래
    };

    CMeshBuffer::MESHBUFFERDESC desc{};
    desc.topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
    desc.vertexSize = sizeof(VertexTexNormalTangentBuffer);
    desc.vertextCount = _countof(cubeVertices);
    desc.indexCount = _countof(cubeIndices);

    info.buffer.assign(reinterpret_cast<uint8_t*>(cubeVertices), reinterpret_cast<uint8_t*>(cubeVertices) + sizeof(cubeVertices));
    info.indices.assign(begin(cubeIndices), end(cubeIndices));
    info.desc = desc;

    return info;
}

CMeshBuffer::MeshBufferInitiaizeInfo CMeshBuffer::CreateSphere()
{
    MeshBufferInitiaizeInfo result = {};
    return result;
}

CMeshBuffer::MeshBufferInitiaizeInfo CMeshBuffer::CreateQuad()
{
    MeshBufferInitiaizeInfo info = {};

    const _float length = 0.5f;

    VertexTexNormalTangentBuffer quadVertices[4] =
    {
            {{-length, -length, 0}, { 0,  0, -1}, {0, 1}, {1, 0, 0}},
            {{ length, -length, 0}, { 0,  0, -1}, {1, 1}, {1, 0, 0}},
            {{ length,  length, 0}, { 0,  0, -1}, {1, 0}, {1, 0, 0}},
            {{-length,  length, 0}, { 0,  0, -1}, {0, 0}, {1, 0, 0}},
    };

    static _uint quadIndices[6] =
    {
        2,1,0, 3,2,0
    };

    CMeshBuffer::MESHBUFFERDESC desc{};
    desc.topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
    desc.vertexSize = sizeof(VertexTexNormalTangentBuffer);
    desc.vertextCount = _countof(quadVertices);
    desc.indexCount = _countof(quadIndices);

    info.buffer.assign(reinterpret_cast<uint8_t*>(quadVertices),reinterpret_cast<uint8_t*>(quadVertices) + sizeof(quadVertices));
    info.indices.assign(begin(quadIndices), end(quadIndices));
    info.desc = desc;

    return info;
}

CMeshBuffer::MeshBufferInitiaizeInfo CMeshBuffer::CreateTriangle()
{
    MeshBufferInitiaizeInfo info = {};

    using VTX = VertexTexNormalTangentBuffer;

    const _float length = 0.5f;

    VTX triVerts[3] =
    {
        // 위쪽 꼭짓점
        {
            {0.f, length, 0.f},       // Position
            {0.f, 0.f, -1.f},         // Normal
            {0.5f, 0.f},              // UV
            {1.f, 0.f, 0.f}           // Tangent
        },
        // 오른쪽 아래 꼭짓점
        {
            {length, -length, 0.f},
            {0.f, 0.f, -1.f},
            {1.f, 1.f},
            {1.f, 0.f, 0.f}
        },
        // 왼쪽 아래 꼭짓점
        {
            {-length, -length, 0.f},
            {0.f, 0.f, -1.f},
            {0.f, 1.f},
            {1.f, 0.f, 0.f}
        }
    };

    // 인덱스 (0-1-2)
    _uint triIndices[3] = { 0, 1, 2 };

    info.buffer.assign(reinterpret_cast<uint8_t*>(triVerts), reinterpret_cast<uint8_t*>(triVerts) + sizeof(triVerts));
    info.indices.assign(begin(triIndices), end(triIndices));

    MESHBUFFERDESC desc{};
    desc.topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
    desc.vertexSize = sizeof(VTX);
    desc.vertextCount = _countof(triVerts);
    desc.indexCount = _countof(triIndices);

    info.desc = desc;

    return info;
}

CMeshBuffer::MeshBufferInitiaizeInfo CMeshBuffer::CreateTerrain(_uint _sizeX, _uint _sizeZ, const _float _scale)
{
    if (_sizeX < 1)
        _sizeX = 1;
    if (_sizeZ < 1)
        _sizeZ = 1;

    MeshBufferInitiaizeInfo info = {};

    using VTX = VertexTexNormalTangentBuffer;

    const _float startX = -static_cast<_float>(_sizeX) * _scale;
    const _float startZ = static_cast<_float>(_sizeZ) * _scale;

    const _uint vertCountX = _sizeX + 1;
    const _uint vertCountZ = _sizeZ + 1;
    const _uint totalVerts = vertCountX * vertCountZ;
    const _uint totalQuads = _sizeX * _sizeZ;
    const _uint totalIndices = totalQuads * 6;

    vector<VTX> vertices = {};
    vector<_uint> indices = {};
    vertices.reserve(totalVerts);
    indices.reserve(totalIndices);

    for (_uint z = 0; z < vertCountZ; ++z)
    {
        for (_uint x = 0; x < vertCountX; ++x)
        {
            VTX v = {};
            v.position =
            {
                startX + x * 1.f,
                0.f,
                startZ + z * 1.f
            };

            v.normal = { 0.f, 1.f, 0.f };

            v.uv =
            {
                static_cast<_float>(x) / _sizeX,
                static_cast<_float>(z) / _sizeZ
            };

            v.tangent = { 1.f, 0.f, 0.f };

            vertices.emplace_back(v);
        };
    }

    for (_uint z = 0; z < _sizeZ; ++z)
    {
        for (_uint x = 0; x < _sizeX; ++x)
        {
            _uint v0 = x + z * vertCountX;
            _uint v1 = (x + 1) + z * vertCountX;
            _uint v2 = x + (z + 1) * vertCountX;
            _uint v3 = (x + 1) + (z + 1) * vertCountX;

            indices.push_back(v0);
            indices.push_back(v1);
            indices.push_back(v2);

            indices.push_back(v2);
            indices.push_back(v1);
            indices.push_back(v3);
        }
    }

    info.buffer.assign(
        reinterpret_cast<const uint8_t*>(vertices.data()),
        reinterpret_cast<const uint8_t*>(vertices.data()) + sizeof(VTX) * vertices.size());

    info.indices.assign(indices.begin(), indices.end());

    MESHBUFFERDESC desc{};
    desc.topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
    desc.vertexSize = sizeof(VTX);
    desc.vertextCount = static_cast<_uint>(vertices.size());
    desc.indexCount = static_cast<_uint>(indices.size());

    info.desc = desc;

    return info;
}

CMeshBuffer::MeshBufferInitiaizeInfo CMeshBuffer::CreateObjectMesh(const aiScene* _aiScene, const _uint _index, const _float _scaleFactor)
{
    MeshBufferInitiaizeInfo info = {};
    using VTX = VertexTexNormalTangentBuffer;

    if (!_aiScene)
    {
        OutputDebugStringA("Assimp load failed or mesh index out of bounds.\n");
        return {};
    }

    const aiMesh* mesh = _aiScene->mMeshes[_index];

    vector<VTX> vertices;
    vector<_uint> indices;

    // 정점 복사
    for (_uint i = 0; i < mesh->mNumVertices; ++i)
    {
        VTX v{};
        v.position = {
            mesh->mVertices[i].x * _scaleFactor,
            mesh->mVertices[i].y * _scaleFactor,
            mesh->mVertices[i].z * _scaleFactor
        };

        v.normal = mesh->HasNormals() ?
            _float3{ mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z } :
            _float3{ 0, 0, 0 };

        v.uv = mesh->HasTextureCoords(0) ?
            _float2{ mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y } :
            _float2{ 0, 0 };

        v.tangent = mesh->HasTangentsAndBitangents() ?
            _float3{ mesh->mTangents[i].x, mesh->mTangents[i].y, mesh->mTangents[i].z } :
            _float3{ 0, 0, 0 };

        vertices.emplace_back(v);
    }

    // 인덱스 복사
    for (_uint f = 0; f < mesh->mNumFaces; ++f)
    {
        const aiFace& face = mesh->mFaces[f];
        if (face.mNumIndices != 3) continue;
        indices.push_back(face.mIndices[0]);
        indices.push_back(face.mIndices[1]);
        indices.push_back(face.mIndices[2]);
    }

    // 버퍼 정보 세팅
    MESHBUFFERDESC desc{};
    desc.topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
    desc.vertexSize = sizeof(VTX);
    desc.vertextCount = static_cast<_uint>(vertices.size());
    desc.indexCount = static_cast<_uint>(indices.size());

    info.buffer.assign
    (
        reinterpret_cast<const uint8_t*>(vertices.data()),
        reinterpret_cast<const uint8_t*>(vertices.data()) + sizeof(VTX) * vertices.size()
    );

    info.indices.assign(indices.begin(), indices.end());
    info.desc = desc;

    return info;
}

const CMeshBuffer::MESHBUFFERDESC& CMeshBuffer::Get_Info()
{
	return m_sInfo;
}

void CMeshBuffer::Set_Scalefactor(const _float _value)
{
    //OnDestroy();

    //// 새 MeshBuffer 정보 생성
    //MeshBufferInitiaizeInfo info = CreateObjectMesh
    //(
    //    CEngineString::WStringToString(m_strFilePath),
    //    0,
    //    _value
    //);

    //if (info.buffer.empty() || info.desc.vertexSize == 0 || info.desc.vertextCount == 0)
    //    return;

    //// 정보 저장
    //m_sInfo = info.desc;

    //size_t size = info.desc.vertexSize * info.desc.vertextCount;

    //// CPU 메모리 복사
    //m_pVertexSysMem = malloc(size);
    //memcpy(m_pVertexSysMem, info.buffer.data(), size);

    //if (info.desc.indexCount > 0 && !info.indices.empty())
    //{
    //    size_t indexSize = sizeof(_uint) * info.desc.indexCount;
    //    m_pIndexSysMem = malloc(indexSize);
    //    memcpy(m_pIndexSysMem, info.indices.data(), indexSize);
    //}

    //// GPU 버퍼 생성
    //ID3D11Device* device = CGraphicDevice::GetInstance().Get_Device();

    //// Vertex Buffer
    //D3D11_BUFFER_DESC vbDesc = {};
    //vbDesc.ByteWidth = static_cast<_uint>(size);
    //vbDesc.Usage = D3D11_USAGE_DEFAULT;
    //vbDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

    //D3D11_SUBRESOURCE_DATA vbData = {};
    //vbData.pSysMem = info.buffer.data();

    //if (FAILED(device->CreateBuffer(&vbDesc, &vbData, &m_pVertexBuffer)))
    //    return;

    //// Index Buffer
    //if (info.desc.indexCount > 0 && !info.indices.empty())
    //{
    //    D3D11_BUFFER_DESC ibDesc = {};
    //    ibDesc.ByteWidth = sizeof(_uint) * info.desc.indexCount;
    //    ibDesc.Usage = D3D11_USAGE_DEFAULT;
    //    ibDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

    //    D3D11_SUBRESOURCE_DATA ibData = {};
    //    ibData.pSysMem = info.indices.data();

    //    if (FAILED(device->CreateBuffer(&ibDesc, &ibData, &m_pIndexBuffer)))
    //        return;
    //}
}

ID3D11Buffer* CMeshBuffer::Get_VertexBuffer() const
{
	return m_pVertexBuffer.Get();
}

ID3D11Buffer* CMeshBuffer::Get_IndexBuffer() const
{
	return m_pIndexBuffer.Get();
}
