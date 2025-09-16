#include "cpch.h"
#include "MeshEffect.h"

CMeshEffect::CMeshEffect()
	: m_strEffectName({})
	, m_sDescription({})
	, m_vMesheList({})
	, m_vTextureList({})
{
	m_strName = L"MeshEffect";
}

CMeshEffect::~CMeshEffect()
{
}

HRESULT CMeshEffect::Initialize()
{
	if (FAILED(__super::Initialize()))
		return E_FAIL;

	for (_uint i = 0; i < m_sDescription.meshCount; ++i)
	{
		const wstring meshName = m_strEffectName + L"_Mesh_" + to_wstring(i) + L" (MeshBuffer)";
		CMeshRenderer* mesh = m_pGameObject->CreateMeshHierachy(CResources::LoadMeshBuffersOnScene(meshName))[0];
		m_vMesheList.push_back(mesh);

		mesh->Set_Material(CResources::CloneOnGame<CMaterial>(L"UnlitMaterial (Material)"));
		const wstring texName = m_strEffectName + L"_Texture_" + to_wstring(i) + L" (Texture)";
		CTexture* texture = CResources::LoadOnScene<CTexture>(texName);
		m_vTextureList.push_back(texture);
		mesh->Get_Material()->Set_Texture(texture);
	}

	return S_OK;
}

void CMeshEffect::Awake()
{
}

void CMeshEffect::Start()
{
}

void CMeshEffect::Update()
{
}

void CMeshEffect::OnDestroy()
{
}
