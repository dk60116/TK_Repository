#include "epch.h"
#include "SkinnedMeshBuffer.h"

CSkinnedMeshBuffer::CSkinnedMeshBuffer()
	: m_pVertexBuffer({})
	, m_pIndexBuffer({})
	, m_iIndexCount(0)
	, m_vBoneNames({})
	, m_vBindPoseInverse({})
	, m_vVertices({})
{
	m_strName = L"Skinned Mesh Buffer";
}

CSkinnedMeshBuffer::~CSkinnedMeshBuffer()
{
    OnDestroy();
}

CSkinnedMeshBuffer* CSkinnedMeshBuffer::Create()
{
	return new CSkinnedMeshBuffer();
}

HRESULT CSkinnedMeshBuffer::Initialize(const string& _filePath, float _scaleFactor)
{
    Assimp::Importer importer;

    const aiScene* scene = importer.ReadFile
    (
        _filePath,
        aiProcess_Triangulate |
        aiProcess_JoinIdenticalVertices |
        aiProcess_GenNormals |
        aiProcess_CalcTangentSpace |
        aiProcess_ConvertToLeftHanded |
        aiProcess_FlipUVs
    );

    if (!scene || !scene->HasMeshes())
    {
        CDebug::LogError("Assimp: failed to load skinned mesh");
        return E_FAIL;
    }

    const aiMesh* mesh = scene->mMeshes[0];
    if (!mesh->HasBones())
    {
        CDebug::LogError("Mesh has no bones.");
        return E_FAIL;
    }

    // ─────────────── 본 이름 및 바인드포즈 저장 ───────────────
    m_vBoneNames.clear();
    m_vBindPoseInverse.clear();

    for (UINT b = 0; b < mesh->mNumBones; ++b)
    {
        const aiBone* bone = mesh->mBones[b];
        m_vBoneNames.emplace_back(bone->mName.C_Str());

        // BindPose 역행렬
        aiMatrix4x4 m = bone->mOffsetMatrix;
        XMMATRIX xm = XMLoadFloat4x4(reinterpret_cast<const XMFLOAT4X4*>(&m));
        m_vBindPoseInverse.emplace_back(xm);
    }

    const UINT numBones = static_cast<UINT>(m_vBoneNames.size());

    // ─────────────── 버텍스 데이터 ───────────────
    m_vVertices.clear();
    m_vVertices.resize(mesh->mNumVertices);

    for (UINT i = 0; i < mesh->mNumVertices; ++i)
    {
        VertexSkinned v{};

        v.position = { mesh->mVertices[i].x * _scaleFactor, mesh->mVertices[i].y * _scaleFactor, mesh->mVertices[i].z * _scaleFactor };
        v.normal = mesh->HasNormals() ?
            XMFLOAT3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z) :
            XMFLOAT3(0, 0, 0);

        v.uv = mesh->HasTextureCoords(0) ?
            XMFLOAT2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y) :
            XMFLOAT2(0, 0);

        v.tangent = mesh->HasTangentsAndBitangents() ?
            XMFLOAT3(mesh->mTangents[i].x, mesh->mTangents[i].y, mesh->mTangents[i].z) :
            XMFLOAT3(0, 0, 0);

        ZeroMemory(v.boneIndices, sizeof(v.boneIndices));
        ZeroMemory(v.boneWeights, sizeof(v.boneWeights));

        m_vVertices[i] = v;
    }

    // ─────────────── 버텍스에 본 가중치 적용 ───────────────
    for (UINT b = 0; b < mesh->mNumBones; ++b)
    {
        const aiBone* bone = mesh->mBones[b];

        for (UINT w = 0; w < bone->mNumWeights; ++w)
        {
            UINT vertexId = bone->mWeights[w].mVertexId;
            float weight = bone->mWeights[w].mWeight;

            VertexSkinned& v = m_vVertices[vertexId];

            for (UINT k = 0; k < 4; ++k)
            {
                if (v.boneWeights[k] == 0.0f)
                {
                    v.boneIndices[k] = b;
                    v.boneWeights[k] = weight;
                    break;
                }
            }
        }
    }

    // ─────────────── 인덱스 데이터 ───────────────
    vector<UINT> indices;

    for (UINT f = 0; f < mesh->mNumFaces; ++f)
    {
        const aiFace& face = mesh->mFaces[f];
        if (face.mNumIndices != 3)
            continue;

        indices.push_back(face.mIndices[0]);
        indices.push_back(face.mIndices[1]);
        indices.push_back(face.mIndices[2]);
    }

    m_iIndexCount = static_cast<UINT>(indices.size());

    // ─────────────── GPU 버퍼 생성 ───────────────
    ID3D11Device* device = CGraphicDevice::GetInstance().Get_Device();

    // VertexBuffer
    D3D11_BUFFER_DESC vbDesc = {};
    vbDesc.ByteWidth = UINT(sizeof(VertexSkinned) * m_vVertices.size());
    vbDesc.Usage = D3D11_USAGE_DEFAULT;
    vbDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

    D3D11_SUBRESOURCE_DATA vbData = {};
    vbData.pSysMem = m_vVertices.data();

    if (FAILED(device->CreateBuffer(&vbDesc, &vbData, m_pVertexBuffer.GetAddressOf())))
        return E_FAIL;

    // IndexBuffer
    D3D11_BUFFER_DESC ibDesc = {};
    ibDesc.ByteWidth = UINT(sizeof(UINT) * indices.size());
    ibDesc.Usage = D3D11_USAGE_DEFAULT;
    ibDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

    D3D11_SUBRESOURCE_DATA ibData = {};
    ibData.pSysMem = indices.data();

    if (FAILED(device->CreateBuffer(&ibDesc, &ibData, m_pIndexBuffer.GetAddressOf())))
        return E_FAIL;

    return S_OK;
}

void CSkinnedMeshBuffer::Render()
{
    if (!m_pVertexBuffer)
        return;

    ID3D11DeviceContext* ctx = CGraphicDevice::GetInstance().Get_Context();

    /* ---------- IA(입력 어셈블러) 단계 ---------- */
    // 버텍스 버퍼
    UINT stride = sizeof(VertexSkinned);   // Skinned 정점 한 개의 크기
    UINT offset = 0;
    ctx->IASetVertexBuffers(0, 1, m_pVertexBuffer.GetAddressOf(), &stride, &offset);

    // 인덱스 버퍼(있을 때만)
    if (m_pIndexBuffer)
        ctx->IASetIndexBuffer(m_pIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);

    // 프리미티브 토폴로지
    ctx->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    /* ---------- Draw 호출 ---------- */
    if (m_pIndexBuffer && m_iIndexCount > 0)
        ctx->DrawIndexed(m_iIndexCount, 0, 0);
    else
        ctx->Draw(static_cast<UINT>(m_vVertices.size()), 0);
}

void CSkinnedMeshBuffer::OnDestroy()
{
    m_pVertexBuffer.Reset();
    m_pIndexBuffer.Reset();
    m_vVertices.clear();
    m_vBoneNames.clear();
    m_vBindPoseInverse.clear();
}


UINT CSkinnedMeshBuffer::Get_IndexCount() const
{
	return m_iIndexCount;
}

const _matrix& CSkinnedMeshBuffer::Get_BindPoseMatricesInverse(const _int _index)
{
    return m_vBindPoseInverse[_index];
}

const vector<string>& CSkinnedMeshBuffer::Get_BoneNames() const
{
    return m_vBoneNames;
}
