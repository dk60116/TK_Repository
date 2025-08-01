#include "cpch.h"
#include "Monster.h"

CMonster::CMonster()
	: m_strSkinnedMeshBufferName(L"")
	, m_pAnimator(nullptr)
{
}

CMonster::~CMonster()
{
}

HRESULT CMonster::Initialize()
{
	if (FAILED(__super::Initialize()))
		return E_FAIL;

	m_pGameObject->CreateSkinnedMeshHierachy(CResources::GetInstance().LoadSkinnedMeshBuffersOnScene(m_strSkinnedMeshBufferName), CResources::GetInstance().LoadSkinnedBonesOnScene(m_strSkinnedMeshBufferName));
	m_pAnimator = m_pGameObject->AddComponent<CAnimator>();

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
}
