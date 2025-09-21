#include "epch.h"
#include "ParticleSystem.h"

CParticleSystem::CParticleSystem()
    : m_vRenderers({})
    , m_sDescription({})
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

        if (buffer)
            m_vRenderers.back()->Get_MeshFilter()->Set_MeshBuffer(buffer);

        const wstring textureName = m_sDescription.particles[i].texture + L" (Texture)";

        CTexture* texture = CResources::LoadOnScene<CTexture>(textureName);

        if (!texture)
            texture = CResources::LoadOnGame<CTexture>(textureName);

        if (texture)
            m_vRenderers.back()->Get_Material()->Set_Texture(texture);
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

    m_vRenderers.clear();
}