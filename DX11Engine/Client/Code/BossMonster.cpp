#include "cpch.h"
#include "BossMonster.h"

CBossMonster::CBossMonster()
	: m_strBossName(L"")
	, m_sDesc({})
	, m_vMeshRenderers({})
	, m_pBaseMap(nullptr)
	, m_pAnimator(nullptr)
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
	m_vMeshRenderers = m_pGameObject->CreateSkinnedMeshHierachy(CResources::LoadSkinnedMeshBuffersOnScene(skinnedMeshBufferName), CResources::LoadSkinnedBonesOnScene(skinnedMeshBufferName), m_sDesc.scaleFactor);

	m_pBaseMap = CResources::LoadOnScene<CTexture>(m_strBossName + L"_BaseMap (Texture)");

	for (TRAVERSAL_ITER(m_vMeshRenderers, it))
		(*it)->Get_Material()->Set_Texture(m_pBaseMap);

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

const wstring& CBossMonster::Get_MonsterName()
{
	return m_strName;
}

CAnimator* CBossMonster::Get_Animator() const
{
	return m_pAnimator;
}

void CBossMonster::Change_State(const _uint _state)
{
}

void CBossMonster::Get_Damage(CWeapon* _weapon)
{
}
