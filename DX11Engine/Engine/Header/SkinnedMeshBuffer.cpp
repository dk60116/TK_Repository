#include "epch.h"
#include "SkinnedMeshBuffer.h"

CSkinnedMeshBuffer::CSkinnedMeshBuffer()
    : CMeshBuffer{}
    , m_pImporter(nullptr)
    , m_pAssimpScene(nullptr)
    , m_vBoneNames({})
    , m_vBoneOffsetMatrices({})
{
	m_strName = L"Skinned Mesh Buffer";
}

CSkinnedMeshBuffer::~CSkinnedMeshBuffer()
{
    OnDestroy();
}

CSkinnedMeshBuffer* CSkinnedMeshBuffer::Create(const wstring& _filePath)
{
	return new CSkinnedMeshBuffer();
}

HRESULT CSkinnedMeshBuffer::Initialize(const wstring& _name, const wstring& _filePath, void* _desc)
{
    if (FAILED(CEngineResource::Initialize(_name, _filePath, _desc)))
        return E_FAIL;

    ID3D11Device* device = CGraphicDevice::GetInstance().Get_Device();

    if (!device)
        return E_FAIL;

    m_pImporter = new Assimp::Importer();

    m_pAssimpScene = m_pImporter->ReadFile
    (
        CEngineString::WStringToString(_filePath),
        aiProcess_Triangulate |
        aiProcess_JoinIdenticalVertices |
        aiProcess_GenSmoothNormals |
        aiProcess_CalcTangentSpace |
        aiProcess_ConvertToLeftHanded |
        aiProcess_LimitBoneWeights
    );

    if (!m_pAssimpScene || !m_pAssimpScene->HasMeshes())
    {
        CDebug::LogError(L"Assimp Skinned mesh load failed: " + m_strFilePath);
        return E_FAIL;
    }

    const aiMesh* mesh = m_pAssimpScene->mMeshes[0];

    using VTX = VertexSkinnedBuffer;

    vector<VTX> vertices;
    vector<_uint> indices;

    for (_uint i = 0; i < mesh->mNumVertices; ++i)
    {
        VTX v = {};
        const aiVector3D& pos = mesh->mVertices[i];
        const aiVector3D& normal = mesh->mNormals[i];
        const aiVector3D& tangent = mesh->mTangents ? mesh->mTangents[i] : aiVector3D(0.f, 0.f, 0.f);

        v.position = _float3(pos.x, pos.y, pos.z);
        v.normal = _float3(normal.x, normal.y, normal.z);

        if (mesh->HasTextureCoords(0))
            v.uv = _float2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);

        vertices.push_back(v);
    }

    for (_uint f = 0; f < mesh->mNumFaces; ++f)
    {
        const aiFace& face = mesh->mFaces[f];
        if (face.mNumIndices != 3)
            continue;
        indices.push_back(face.mIndices[0]);
        indices.push_back(face.mIndices[1]);
        indices.push_back(face.mIndices[2]);
    }

    m_vBoneNames.clear();

    for (UINT b = 0; b < mesh->mNumBones; ++b)
    {
        string bn = mesh->mBones[b]->mName.C_Str();
        m_vBoneNames.emplace_back(bn.begin(), bn.end());
    }

    m_vBoneOffsetMatrices.clear();
    m_vBoneOffsetMatrices.reserve(mesh->mNumBones);

    for (UINT b = 0; b < mesh->mNumBones; ++b)
    {
        const aiBone* bone = mesh->mBones[b];

        // Assimp 행렬 -> XMMATRIX 변환
        const aiMatrix4x4& offset = bone->mOffsetMatrix;

        _matrix matOffset = XMMatrixTranspose
        (
            _matrix
            (
                offset.a1, offset.a2, offset.a3, offset.a4,
                offset.b1, offset.b2, offset.b3, offset.b4,
                offset.c1, offset.c2, offset.c3, offset.c4,
                offset.d1, offset.d2, offset.d3, offset.d4
            )
        );

        _float4x4 mat4x4 = {};

        XMStoreFloat4x4(&mat4x4, matOffset);

        m_vBoneOffsetMatrices.push_back(mat4x4);
    }

    FillBoneWeightsAndIndices(mesh, vertices);

    D3D11_BUFFER_DESC vbDesc = {};
    vbDesc.ByteWidth = _uint(vertices.size() * sizeof(VTX));
    vbDesc.Usage = D3D11_USAGE_DEFAULT;
    vbDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

    D3D11_SUBRESOURCE_DATA vbData = {};
    vbData.pSysMem = vertices.data();

    // Index Buffer
    D3D11_BUFFER_DESC ibDesc = {};
    ibDesc.ByteWidth = _uint(indices.size() * sizeof(_uint));
    ibDesc.Usage = D3D11_USAGE_DEFAULT;
    ibDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

    D3D11_SUBRESOURCE_DATA ibData = {};
    ibData.pSysMem = indices.data();

    if (FAILED(device->CreateBuffer(&vbDesc, &vbData, &m_pVertexBuffer)))
        return E_FAIL;

    if (FAILED(device->CreateBuffer(&ibDesc, &ibData, &m_pIndexBuffer)))
        return E_FAIL;

    m_sInfo.vertexSize = sizeof(VTX);
    m_sInfo.vertextCount = _uint(vertices.size());
    m_sInfo.indexCount = _uint(indices.size());
    m_sInfo.topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

    return S_OK;
}

void CSkinnedMeshBuffer::Render()
{
    if (!m_pVertexBuffer)
        return;

    ID3D11DeviceContext* context = CGraphicDevice::GetInstance().Get_Context();

    _uint stride = m_sInfo.vertexSize;
    _uint offset = 0;

    context->IASetVertexBuffers(0, 1, m_pVertexBuffer.GetAddressOf(), &stride, &offset);

    if (m_pIndexBuffer)
    {
        context->IASetIndexBuffer(m_pIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
        context->IASetPrimitiveTopology(m_sInfo.topology);
        context->DrawIndexed(m_sInfo.indexCount, 0, 0);
    }
    else
    {
        context->IASetPrimitiveTopology(m_sInfo.topology);
        context->Draw(m_sInfo.vertextCount, 0);
    }
}

void CSkinnedMeshBuffer::OnDestroy()
{
    __super::OnDestroy();

    if (m_pImporter)
    {
        Safe_Delete(m_pImporter);
        m_pAssimpScene = nullptr;
    }

    m_vBoneNames.clear();
}

const _float4x4& CSkinnedMeshBuffer::Get_BoneOffsetMatrix(const _uint _index)
{
    return m_vBoneOffsetMatrices[_index];
}

void CSkinnedMeshBuffer::FillBoneWeightsAndIndices(const aiMesh* mesh, vector<VertexSkinnedBuffer>& vertices)
{
    // 본 인덱스/가중치 할당
    for (_uint i = 0; i < mesh->mNumBones; ++i)
    {
        const aiBone* bone = mesh->mBones[i];

        for (_uint j = 0; j < bone->mNumWeights; ++j)
        {
            const aiVertexWeight& vw = bone->mWeights[j];

            _uint vertexId = vw.mVertexId;
            float weight = vw.mWeight;

            auto& v = vertices[vertexId];

            for (_uint k = 0; k < 4; ++k)
            {
                if (v.boneWeights[k] == 0.0f)
                {
                    v.boneIndices[k] = i;
                    v.boneWeights[k] = weight;
                    break;
                }
            }
        }
    }

    // 각 버텍스의 가중치를 큰 순서로 정렬 + 인덱스 함께 정렬
    for (auto& v : vertices)
    {
        // 가중치와 인덱스를 쌍으로 모음
        vector<pair<_uint, float>> bonePairs;
        for (int k = 0; k < 4; ++k)
        {
            if (v.boneWeights[k] > 0.0f)
                bonePairs.emplace_back(v.boneIndices[k], v.boneWeights[k]);
        }

        // 큰 가중치 순으로 정렬
        sort(bonePairs.begin(), bonePairs.end(),
            [](const pair<_uint, float>& a, const pair<_uint, float>& b)
            {
                return a.second > b.second;
            });

        // 다시 배열에 복사
        for (int k = 0; k < 4; ++k)
        {
            if (k < bonePairs.size())
            {
                v.boneIndices[k] = bonePairs[k].first;
                v.boneWeights[k] = bonePairs[k].second;
            }
            else
            {
                v.boneIndices[k] = 0;
                v.boneWeights[k] = 0.0f;
            }
        }

        // 정규화
        float sum = v.boneWeights[0] + v.boneWeights[1] + v.boneWeights[2] + v.boneWeights[3];
        if (sum > 0.0f)
        {
            for (int k = 0; k < 4; ++k)
                v.boneWeights[k] /= sum;
        }
    }
}
