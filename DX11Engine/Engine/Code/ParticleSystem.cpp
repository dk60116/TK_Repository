#include "epch.h"
#include "ParticleSystem.h"

CParticleSystem::CParticleSystem()
    : m_vRenderers({})
    , m_sDescription({})
    , m_vInstanceBuffers({})
    , m_fCurrentTime(0.f)
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

        if (instanceBuffer)
        {
            auto& inst = instanceBuffer->Get_InstancingDesc();
            inst.count = instanceCount;

            for (_uint j = 0; j < inst.count; ++j)
            {
                _matrix W = XMMatrixTranslation(static_cast<float>(j) * 0.3f, 0.f, 0.f);

                MeshInstanceData data;
                XMStoreFloat4(&data.row0, W.r[0]);
                XMStoreFloat4(&data.row1, W.r[1]);
                XMStoreFloat4(&data.row2, W.r[2]);
                XMStoreFloat4(&data.row3, W.r[3]);

                inst.data[j] = data;
            }

            instanceBuffer->UpdateInstanceBuffer();
        }

        if (buffer)
        {
            m_vInstanceBuffers.push_back(instanceBuffer);
            m_vRenderers.back()->Get_MeshFilter()->Set_MeshBuffer(instanceBuffer);
        }

        const wstring textureName = m_sDescription.particles[i].texture + L" (Texture)";

        CTexture* texture = CResources::LoadOnScene<CTexture>(textureName);

        if (!texture)
            texture = CResources::LoadOnGame<CTexture>(textureName);

        if (texture)
            m_vRenderers.back()->Get_Material()->Set_Texture(texture);

        m_vRenderers.back()->Get_Material()->Set_BaseColor(ColorValue(255, 255, 255, 125).f4Color());
    }

    return S_OK;
}

void CParticleSystem::Update()
{
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
