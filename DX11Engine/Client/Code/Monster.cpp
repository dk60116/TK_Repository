#include "cpch.h"
#include "Monster.h"

CMonster::CMonster()
	: m_strSkinnedMeshBufferName(L"")
	, m_vMeshRenderers({})
	, m_pBaseMap(nullptr)
	, m_pAnimator(nullptr)
{
	m_strName = L"Wolf";
}

CMonster::~CMonster()
{
}

HRESULT CMonster::Initialize()
{
	if (FAILED(__super::Initialize()))
		return E_FAIL;

	m_vMeshRenderers = m_pGameObject->CreateSkinnedMeshHierachy(CResources::GetInstance().LoadSkinnedMeshBuffersOnScene(m_strSkinnedMeshBufferName), CResources::GetInstance().LoadSkinnedBonesOnScene(m_strSkinnedMeshBufferName));
	m_pAnimator = m_pGameObject->AddComponent<CAnimator>();

	m_pBaseMap = CResources::GetInstance().LoadOnScene<CTexture>(m_strName + L"_BaseMap (Texture)");
	m_pBaseMap->AddRef();

	for (size_t i = 0; i < m_vMeshRenderers.size(); ++i)
		m_vMeshRenderers[i]->Get_Material()->Set_Texture(m_pBaseMap);

	return S_OK;
}

void CMonster::Awake()
{
}

void CMonster::Start()
{
}

void CMonster::Update()
{
}

void CMonster::OnDestroy()
{
	Safe_Release(m_pBaseMap);
}

CAnimator* CMonster::Get_Animator()
{
	return m_pAnimator;
}
