#include "cpch.h"
#include "BossMonster.h"

CBossMonster::CBossMonster()
{
}

CBossMonster::~CBossMonster()
{
}

HRESULT CBossMonster::Initialize()
{
	if (FAILED(__super::Initialize()))
		return E_FAIL;

	wstring skinnedMeshBufferName = m_strBossName + L"_Model (MeshBuffer)";
	m_vMeshRenderers = m_pGameObject->CreateSkinnedMeshHierachy(CResources::LoadSkinnedMeshBuffersOnScene(skinnedMeshBufferName), CResources::LoadSkinnedBonesOnScene(skinnedMeshBufferName));

	return S_OK;
}

void CBossMonster::Awake()
{
}

void CBossMonster::Start()
{
}

void CBossMonster::Update()
{
}

void CBossMonster::OnEnable()
{
}

void CBossMonster::OnCollisionEnter(CCollider* _other)
{
}

void CBossMonster::OnDestroy()
{
}
