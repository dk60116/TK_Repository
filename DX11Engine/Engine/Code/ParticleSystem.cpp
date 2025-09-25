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

        _float4 baseCol = m_sDescription.particles[i].info.baseColor.f4Color();
        baseCol.w = 0.5f;
        m_vRenderers.back()->Get_Material()->Set_BaseColor(baseCol);

        m_vPlaying[i] = m_sDescription.particles[i].info.playOnAwake;
    }

    return S_OK;
}

void CParticleSystem::OnEnable()
{
    for (_uint i = 0; i < m_sDescription.count; ++i)
    {
        m_vCurrentTimes[i] = 0.f;
        m_vPlaying[i] = m_sDescription.particles[i].info.playOnAwake;
    }
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

                if (m_vCurrentTimes[i] > m_sDescription.particles[i].info.startLifeTime)
                {
                    m_vWaitTimes[i] = 0.f;
                    m_vCurrentTimes[i] = 0.f;

                    if (!m_sDescription.particles[i].info.loop)
                        m_vPlaying[i] = false;
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

        PARTICLEINFO& info = m_sDescription.particles[i].info;

        m_vRenderers[i]->Get_Material()->Set_FloatValue(L"lifeTime", info.startLifeTime);
        const _float4 startColor = info.startColor.f4Color();
        m_vRenderers[i]->Get_Material()->Set_Vector4Value(L"startColor", startColor);
        const _float4 endColor = info.endColor.f4Color();
        m_vRenderers[i]->Get_Material()->Set_Vector4Value(L"endColor", endColor);

        for (_uint j = 0; j < instanceCount; ++j)
        {
            _matrix W = XMMatrixTranslation(0.f, 0.f, 0.f);

            _matrix F = W;

            MeshInstanceData data = {};
            XMStoreFloat4(&data.row0, F.r[0]);
            XMStoreFloat4(&data.row1, F.r[1]);
            XMStoreFloat4(&data.row2, F.r[2]);
            XMStoreFloat4(&data.row3, F.r[3]);

            const _float random = CRandom::Range(0.f, 1.f);
            const _vector randomSeed = XMVectorSet(random, random, random, random);
            //const _vector randomSeed = XMVectorSet(0.1f, 0.1f, 0.1f, 0.1f);
            XMStoreFloat4(&data.randomSeed, randomSeed);

            const _vector startSizeMin = XMVectorSet(info.startSizeMin.x, info.startSizeMin.y, info.startSizeMin.z, info.startSizeMin.w);
            XMStoreFloat4(&data.startSizeMin, startSizeMin);
            const _vector startSizeMax = XMVectorSet(info.startSizeMax.x, info.startSizeMax.y, info.startSizeMax.z, info.startSizeMax.w);
            XMStoreFloat4(&data.startSizeMax, startSizeMax);
            const _vector endSizeMin = XMVectorSet(info.endSizeMin.x, info.endSizeMin.y, info.endSizeMin.z, info.endSizeMin.w);
            XMStoreFloat4(&data.endSizeMin, endSizeMin);
            const _vector endSizeMax = XMVectorSet(info.endSizeMax.x, info.endSizeMax.y, info.endSizeMax.z, info.endSizeMax.w);
            XMStoreFloat4(&data.endSizeMax, endSizeMax);

            const vector3& velValue = m_sDescription.particles[i].info.startVelocity;
            const _vector velocity = XMVectorSet(CRandom::Range(-velValue.x, velValue.x), CRandom::Range(-velValue.y, velValue.y), CRandom::Range(-velValue.z, velValue.z), 0.f);
            XMStoreFloat4(&data.startVelocity, velocity);

            inst.data[j] = data;
        }
    }

    _buffer.UpdateInstanceBuffer();

    m_vInstanceBuffers.push_back(&_buffer);
}
