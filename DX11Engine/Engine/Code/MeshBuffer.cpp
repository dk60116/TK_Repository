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

CMeshBuffer* CMeshBuffer::Create(const wstring& _filePath)
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
    else if (_filePath == L"../Assets/LineRect")
        info = CreateLineRect();
    else if (_filePath == L"../Assets/Cube")
        info = CreateCube();
    else if (_filePath == L"../Assets/Quad")
        info = CreateQuad();
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

    ID3D11Device* device = CGraphicDevice::GetInstance().Get_Device();

    // VertexBuffer 생성
    D3D11_BUFFER_DESC vbDesc = {};
    vbDesc.ByteWidth = static_cast<UINT>(info.desc.vertexSize * info.desc.vertextCount);
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
        ibDesc.ByteWidth = sizeof(UINT) * info.desc.indexCount;
        ibDesc.Usage = D3D11_USAGE_DEFAULT;
        ibDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

        D3D11_SUBRESOURCE_DATA ibData = {};
        ibData.pSysMem = info.indices.data();

        hr = FAILED(device->CreateBuffer(&ibDesc, &ibData, &m_pIndexBuffer));
    }

    if (FAILED(hr))
    {
        CDebug::LogError(L"Assimp MeshBuffer load failed: " + m_strFilePath);
        return E_FAIL;
    }

    return hr;
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

    info.buffer.assign(begin(lineVertices), end(lineVertices));

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

    info.buffer.assign(begin(rectVertices), end(rectVertices));
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

    info.buffer.assign(begin(quadVertices), end(quadVertices));
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

    // 버텍스 복사
    info.buffer.assign(std::begin(triVerts), std::end(triVerts));

    // 인덱스 복사
    info.indices.assign(std::begin(triIndices), std::end(triIndices));

    // 메쉬 설명
    MESHBUFFERDESC desc{};
    desc.topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
    desc.vertexSize = sizeof(VTX);
    desc.vertextCount = _countof(triVerts);
    desc.indexCount = _countof(triIndices);

    info.desc = desc;

    return info;
}

CMeshBuffer::MeshBufferInitiaizeInfo CMeshBuffer::CreateObjectMesh(const string& _filePath, const _float _scaleFactor)
{
    MeshBufferInitiaizeInfo info = {};

    using VTX = VertexTexNormalTangentBuffer;

    // 모델 로딩 
    Assimp::Importer importer;

    const aiScene* scene = importer.ReadFile
    (
        _filePath,
        aiProcess_Triangulate |   // 모든 면을 삼각형화
        aiProcess_JoinIdenticalVertices |   // 중복 정점 병합
        aiProcess_GenNormals |   // 노멀 없으면 생성
        aiProcess_CalcTangentSpace |   // 텍스처 좌표 기반 탄젠트
        aiProcess_ConvertToLeftHanded |   // DirectX 좌표계
        aiProcess_FlipUVs
    );

    if (!scene)
    {
        const char* error = importer.GetErrorString();
        OutputDebugStringA("Assimp load failed: ");
        OutputDebugStringA(error);
        OutputDebugStringA("\n");

        return MeshBufferInitiaizeInfo();
    }

    vector<VTX>  vertices;
    vector<UINT> indices;

    auto copyMesh = [&](const aiMesh* mesh)
        {
            const UINT base = static_cast<UINT>(vertices.size());

            /* 정점 */
            for (UINT i = 0; i < mesh->mNumVertices; ++i)
            {
                VTX v{};
                /* 위치 */
                v.position = 
                { 
                    mesh->mVertices[i].x,
                    mesh->mVertices[i].y,
                    mesh->mVertices[i].z
                };

                v.position.x *= _scaleFactor;
                v.position.y *= _scaleFactor;
                v.position.z *= _scaleFactor;
                                
                 // 노멀
                if (mesh->HasNormals()) 
                    v.normal = 
                { 
                    mesh->mNormals[i].x,
                    mesh->mNormals[i].y,
                    mesh->mNormals[i].z 
                };
                else
                    v.normal = { 0,0,0 };

                // UV(0)
                if (mesh->HasTextureCoords(0))
                    v.uv = 
                {
                    mesh->mTextureCoords[0][i].x,
                    mesh->mTextureCoords[0][i].y
                };
                else
                    v.uv = { 0,0 };

                // 탄젠트
                if (mesh->HasTangentsAndBitangents())
                    v.tangent = 
                { 
                    mesh->mTangents[i].x,
                    mesh->mTangents[i].y,
                    mesh->mTangents[i].z 
                };
                else
                    v.tangent = { 0,0,0 };

                vertices.emplace_back(v);
            }

            // 인덱스(얼굴)
            for (UINT f = 0; f < mesh->mNumFaces; ++f)
            {
                const aiFace& face = mesh->mFaces[f];
                if (face.mNumIndices != 3) 
                    continue;
                indices.push_back(base + face.mIndices[0]);
                indices.push_back(base + face.mIndices[1]);
                indices.push_back(base + face.mIndices[2]);
            }
        };

    // 씬 노드 재귀 순회
    function<void(const aiNode*)> traverse = [&](const aiNode* node)
        {
            for (UINT m = 0; m < node->mNumMeshes; ++m)
                copyMesh(scene->mMeshes[node->mMeshes[m]]);

            for (UINT c = 0; c < node->mNumChildren; ++c)
                traverse(node->mChildren[c]);
        };

    traverse(scene->mRootNode);

    // MeshBuffer 생성
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

void CMeshBuffer::Set_Scalefactor(const _float _value)
{
    OnDestroy();

    // 새 MeshBuffer 정보 생성
    MeshBufferInitiaizeInfo info = CreateObjectMesh
    (
        CEngineString::WStringToString(m_strFilePath),
        _value
    );

    if (info.buffer.empty() || info.desc.vertexSize == 0 || info.desc.vertextCount == 0)
        return;

    // 정보 저장
    m_sInfo = info.desc;

    size_t size = info.desc.vertexSize * info.desc.vertextCount;

    // CPU 메모리 복사
    m_pVertexSysMem = malloc(size);
    memcpy(m_pVertexSysMem, info.buffer.data(), size);

    if (info.desc.indexCount > 0 && !info.indices.empty())
    {
        size_t indexSize = sizeof(UINT) * info.desc.indexCount;
        m_pIndexSysMem = malloc(indexSize);
        memcpy(m_pIndexSysMem, info.indices.data(), indexSize);
    }

    // GPU 버퍼 생성
    ID3D11Device* device = CGraphicDevice::GetInstance().Get_Device();

    // Vertex Buffer
    D3D11_BUFFER_DESC vbDesc = {};
    vbDesc.ByteWidth = static_cast<UINT>(size);
    vbDesc.Usage = D3D11_USAGE_DEFAULT;
    vbDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

    D3D11_SUBRESOURCE_DATA vbData = {};
    vbData.pSysMem = info.buffer.data();

    if (FAILED(device->CreateBuffer(&vbDesc, &vbData, &m_pVertexBuffer)))
        return;

    // Index Buffer
    if (info.desc.indexCount > 0 && !info.indices.empty())
    {
        D3D11_BUFFER_DESC ibDesc = {};
        ibDesc.ByteWidth = sizeof(UINT) * info.desc.indexCount;
        ibDesc.Usage = D3D11_USAGE_DEFAULT;
        ibDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

        D3D11_SUBRESOURCE_DATA ibData = {};
        ibData.pSysMem = info.indices.data();

        if (FAILED(device->CreateBuffer(&ibDesc, &ibData, &m_pIndexBuffer)))
            return;
    }
}

ID3D11Buffer* CMeshBuffer::Get_VertexBuffer() const
{
	return m_pVertexBuffer.Get();
}

ID3D11Buffer* CMeshBuffer::Get_IndexBuffer() const
{
	return m_pIndexBuffer.Get();
}
