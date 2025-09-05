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

	if (!m_pAnimator)
		m_pAnimator = m_pGameObject->AddComponent<CAnimator>();

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

void CBossMonster::Get_Damage(CWeapon* _weapon)
{
}

void CBossMonster::Add_Animation(const wstring& _name)
{
	if (!m_pAnimator)
		return;

	m_pAnimator->Add_Animation(_name, CResources::LoadOnScene<CAnimationClip>(m_strBossName + L'_' + _name + L" (Animation)"));
}
