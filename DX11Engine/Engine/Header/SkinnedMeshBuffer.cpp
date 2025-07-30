#include "epch.h"
#include "SkinnedMeshBuffer.h"

CSkinnedMeshBuffer::CSkinnedMeshBuffer()
    : CMeshBuffer{}
    , m_vBoneNames({})
    , m_vBoneOffsetMatrices({})
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

HRESULT CSkinnedMeshBuffer::Initialize(const wstring& _name, const wstring& _filePath, void* _desc)
{
    if (FAILED(CEngineResource::Initialize(_name, _filePath, _desc)))
        return E_FAIL;

    return S_OK;
}

HRESULT CSkinnedMeshBuffer::Initiailize_Custom(SkinnedBufferInitiaizeInfo _info, void* _desc)
{
    if (!(_info.buffer.size() > 0))
        return E_FAIL;

    if (_info.desc.vertexSize == 0 || _info.desc.vertextCount == 0)
        return E_FAIL;

    m_sInfo = {};
    m_sInfo = _info.desc;

    m_strResourceName = _info.meshName;

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
        CDebug::LogError(L"SkinnedBuffer load failed(Custom): " + m_strFilePath);
        return E_FAIL;
    }

    if (_info.boneNames.size() > 0)
        m_vBoneNames = _info.boneNames;

    if (_info.boneOffsetMatrices.size() > 0)
        m_vBoneOffsetMatrices = _info.boneOffsetMatrices;

    return hr;
}

void CSkinnedMeshBuffer::Render()
{
    if (!m_pVertexBuffer)
        return;

    ID3D11DeviceContext* context = CGraphicDevice::GetInstance().Get_Context();

    _uint stride = m_sInfo.vertexSize;
    _uint offset = 0;

    context->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);

    if (m_pIndexBuffer)
    {
        context->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
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

    m_vBoneNames.clear();
}

const _uint CSkinnedMeshBuffer::Get_BoneCount() const
{
    return static_cast<_uint>(m_vBoneNames.size());
}

const wstring& CSkinnedMeshBuffer::Get_BoneName(const _uint _index) const
{
    return m_vBoneNames[_index];
}

void CSkinnedMeshBuffer::FillBoneWeights(VertexSkinnedBuffer& _targetBuffer, const _uint _index, const _float _weight)
{
    for (_uint i = 0; i < 4; ++i)
    {
        if (_targetBuffer.boneWeights[i] == 0.f)
        {
            _targetBuffer.boneIndices[i] = _index;
            _targetBuffer.boneWeights[i] = _weight;
            return;
        }
    }

    // 4개 꽉 찼다면 가장 작은 weight을 대체
    _uint minIndex = 0;
    for (int i = 1; i < 4; ++i)
    {
        if (_targetBuffer.boneWeights[i] < _targetBuffer.boneWeights[minIndex])
            minIndex = i;
    }

    if (_targetBuffer.boneWeights[minIndex] < _weight)
    {
        _targetBuffer.boneIndices[minIndex] = _index;
        _targetBuffer.boneWeights[minIndex] = _weight;
    }
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
            _float weight = vw.mWeight;

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
        for (_uint k = 0; k < 4; ++k)
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
        _float sum = v.boneWeights[0] + v.boneWeights[1] + v.boneWeights[2] + v.boneWeights[3];
        if (sum > 0.0f)
        {
            for (_uint k = 0; k < 4; ++k)
                v.boneWeights[k] /= sum;
        }
    }
}