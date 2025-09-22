#include "epch.h"
#include "ParticleSystem.h"

CParticleSystem::CParticleSystem()
    : m_vPlaying({})
    , m_vRenderers({})
    , m_sDescription({})
    , m_vInstanceBuffers({})
    , m_vWaitTimes({})
    , m_vCurrentTimes({})
{
}

CParticleSystem::~CParticleSystem()
{
}

CParticleSystem* CParticleSystem::Create()
{
    return new CParticleSystem();
}

CComponent* CParticleSystem::Clone() const
{
    CParticleSystem* clone = new CParticleSystem();

    return clone;
}

HRESULT CParticleSystem::Initialize(void* _desc)
{
    if (FAILED(__super::Initialize(_desc)))
        return E_FAIL;

    if (_desc)
        m_sDescription = *static_cast<PARTICLEDESC*>(_desc);
    else
    {
        m_sDescription = {};

        for (_uint i = 0; i < m_sDescription.count; ++i)
            m_sDescription.particles.push_back({});
    }

    m_vPlaying.resize(m_sDescription.count);
    m_vWaitTimes.resize(m_sDescription.count);
    m_vCurrentTimes.resize(m_sDescription.count);

    for (_uint i = 0; i < m_sDescription.count; ++i)
    {
        CGameObject* partObj = m_pGameObject->Get_Scene()->Add_GameObject(L"Particle_" + to_wstring(i));
        partObj->Get_Transform()->SetParent(Get_Transform());
        CMeshRenderer* render = partObj->AddComponent<CMeshRenderer>();
        m_vRenderers.push_back(render);
        m_vRenderers.back()->AddRef();

        m_vRenderers.back()->SetNoneCull(true);

        const wstring materialName = m_sDescription.particles[i].material + L" (Material)";

        CMaterial* mat = CResources::CloneOnScene<CMaterial>(materialName);

        if (!mat)
            mat = CResources::CloneOnGame<CMaterial>(materialName);

        if (mat)
            m_vRenderers.back()->Set_Material(mat);

        const wstring bufferName = m_sDescription.particles[i].buffer + L" (Mesh Buffer)";

        CMeshBuffer* buffer = CResources::LoadOnScene<CMeshBuffer>(bufferName);

        if (!buffer)
            buffer = CResources::LoadOnGame<CMeshBuffer>(bufferName);

        const _uint instanceCount = m_sDescription.particles[i].info.maxCount;

        CMeshBuffer* instanceBuffer = CreateInstanceBuffer(buffer, instanceCount, D3D11_USAGE_DYNAMIC);

        m_vRenderers.back()->Get_MeshFilter()->Set_MeshBuffer(instanceBuffer);
        SetStartValue(*instanceBuffer, instanceCount);

        const wstring textureName = m_sDescription.particles[i].texture + L" (Texture)";

        CTexture* texture = CResources::LoadOnScene<CTexture>(textureName);

        if (!texture)
            texture = CResources::LoadOnGame<CTexture>(textureName);

        if (texture)
            m_vRenderers.back()->Get_Material()->Set_Texture(texture);

        _float4 startCol = m_sDescription.particles[i].info.startColor.f4Color();
        startCol.w = 0.5f;
        m_vRenderers.back()->Get_Material()->Set_BaseColor(startCol);

        m_vPlaying[i] = m_sDescription.particles[i].info.playOnAwake;
    }

    return S_OK;
}

void CParticleSystem::Update()
{
    for (size_t i = 0; i < m_vCurrentTimes.size(); ++i)
    {
        if (m_vPlaying[i])
        {
            m_vWaitTimes[i] += DELTA_TIME;

            if (m_vWaitTimes[i] >= m_sDescription.particles[i].info.startDelay)
            {
                m_vCurrentTimes[i] += DELTA_TIME;

                if (m_vCurrentTimes[i] > m_sDescription.particles[i].info.lifeTime)
                {
                    m_vWaitTimes[i] = 0.f;
                    m_vCurrentTimes[i] = 0.f;
                }
            }
        }
    }

    for (_uint i = 0; i < m_sDescription.count; ++i)
    {
        const _uint instanceCount = m_sDescription.particles[i].info.maxCount;

        auto& inst = m_vInstanceBuffers[i]->Get_InstancingDesc();
       
        m_vRenderers[i]->Get_Material()->Set_FloatValue(L"time", m_vCurrentTimes[i]);
    }
}

void CParticleSystem::Render_Editor()
{
}

void CParticleSystem::Render()
{
}

void CParticleSystem::OnDestroy()
{
    for (TRAVERSAL_ITER(m_vRenderers, it))
        Safe_Release((*it));

    for (TRAVERSAL_ITER(m_vInstanceBuffers, it))
        Safe_Release((*it));

    m_vRenderers.clear();
    m_vInstanceBuffers.clear();
}

void CParticleSystem::Play()
{
    for (TRAVERSAL_ITER(m_vPlaying, it))
        (*it) = true;
}

CMeshBuffer* CParticleSystem::CreateInstanceBuffer(CMeshBuffer* _origin, _uint _count, D3D11_USAGE _usage)
{
    if (!_origin || _count == 0)
        return nullptr;

    CMeshBuffer* instanced = new CMeshBuffer();

    instanced->m_sInfo = _origin->m_sInfo;
    instanced->m_sAABB = _origin->m_sAABB;
    instanced->m_strResourceName = _origin->m_strResourceName + L" [Inst]";

    instanced->m_pVertexBuffer = _origin->m_pVertexBuffer;
    if (instanced->m_pVertexBuffer) instanced->m_pVertexBuffer->AddRef();

    instanced->m_pIndexBuffer = _origin->m_pIndexBuffer;
    if (instanced->m_pIndexBuffer) instanced->m_pIndexBuffer->AddRef();

    instanced->m_pVertexSysMem = nullptr;
    instanced->m_pIndexSysMem = nullptr;

    if (FAILED(instanced->CreateInstanceBuffer(_count, _usage)))
    {
        Safe_Release(instanced);
        return nullptr;
    }

    instanced->AddRef();

    instanced->UpdateInstanceBuffer();

    return instanced;
}

void CParticleSystem::SetStartValue(CMeshBuffer& _buffer, const _uint _count)
{
    auto& inst = _buffer.Get_InstancingDesc();
    inst.count = _count;

    for (_uint i = 0; i < m_sDescription.count; ++i)
    {
        const _uint instanceCount = _buffer.Get_InstancingDesc().count;

        //m_vRenderers[i]->Get_Material()->Set_FloatValue(L"lifeTime", m_sDescription.particles[i].info.lifeTime);
        //const _float4 startColor = m_sDescription.particles[i].info.startColor.f4Color();
        //m_vRenderers[i]->Get_Material()->Set_Vector4Value(L"startColor", startColor);
        //const _float4 endColor = m_sDescription.particles[i].info.endColor.f4Color();
        //m_vRenderers[i]->Get_Material()->Set_Vector4Value(L"endColor", endColor);

        for (_uint j = 0; j < instanceCount; ++j)
        {
            _matrix S = {};

            if (!m_sDescription.particles[i].info.size3D)
            {
                const _float sizeMin = m_sDescription.particles[i].info.startSizeMin;
                const _float sizeMax = m_sDescription.particles[i].info.startSizeMax;
                const _float randomSize = CRandom::Range(sizeMin, sizeMax);
                S = XMMatrixScaling(randomSize, randomSize, randomSize);
            }
            else
            {
                const vector3 sizeMin = m_sDescription.particles[i].info.startSizeMin3D;
                const vector3 sizeMax = m_sDescription.particles[i].info.startSizeMax3D;
                S = XMMatrixScaling(CRandom::Range(sizeMin.x, sizeMax.x), CRandom::Range(sizeMin.y, sizeMax.y), CRandom::Range(sizeMin.z, sizeMax.z));
            }

            _matrix W = XMMatrixTranslation(0.f, 0.f, 0.f);

            _matrix F = S * W;

            MeshInstanceData data = {};
            XMStoreFloat4(&data.row0, F.r[0]);
            XMStoreFloat4(&data.row1, F.r[1]);
            XMStoreFloat4(&data.row2, F.r[2]);
            XMStoreFloat4(&data.row3, F.r[3]);

            const vector3& velValue = m_sDescription.particles[i].info.startVelocity;
            const _vector velocity = XMVectorSet(CRandom::Range(-velValue.x, velValue.x), CRandom::Range(-velValue.y, velValue.y), CRandom::Range(-velValue.z, velValue.z), 0.f);
            XMStoreFloat3(&data.startVelocity, velocity);

            inst.data[j] = data;
        }
    }

    _buffer.UpdateInstanceBuffer();

    m_vInstanceBuffers.push_back(&_buffer);
}
