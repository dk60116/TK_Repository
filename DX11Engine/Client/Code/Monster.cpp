#include "cpch.h"
#include "Monster.h"
#include "MonsterController.h"

CMonster::CMonster()
	: m_strSkinnedMeshBufferName(L"")
	, m_fSkinnedMeshScaleFactor(0.01f)
	, m_vMeshRenderers({})
	, m_pBaseMap(nullptr)
	, m_pAnimator(nullptr)
	, m_pController(nullptr)
	, m_pNavAgent(nullptr)
	, m_sOptions({})
	, m_sStatus({})
	, m_pHeadTF(nullptr)
	, m_pBodyCollider(nullptr)
	, m_pHeadCollider(nullptr)
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

	m_pGameObject->SetLayer(L"Monster");

	if (!m_pController)
		m_pController = m_pGameObject->AddComponent<CMonsterController>();
	
	if (m_pController)
	{
		m_pController->AddRef();
		m_pController->Set_Monster(this);
	}

	m_vMeshRenderers = m_pGameObject->CreateSkinnedMeshHierachy(CResources::LoadSkinnedMeshBuffersOnScene(m_strSkinnedMeshBufferName), CResources::LoadSkinnedBonesOnScene(m_strSkinnedMeshBufferName), m_fSkinnedMeshScaleFactor, vector3::up() * 180.f);
	m_pAnimator = m_pGameObject->AddComponent<CAnimator>();

	m_pBaseMap = CResources::LoadOnScene<CTexture>(m_strName + L"_BaseMap (Texture)");
	m_pBaseMap->AddRef();

	Add_Animation(L"Idle");
	Add_Animation(L"Walk");
	Add_Animation(L"LookAround");
	Add_Animation(L"Find");
	Add_Animation(L"Run");
	Add_Animation(L"Attack01");

	for (size_t i = 0; i < m_vMeshRenderers.size(); ++i)
		m_vMeshRenderers[i]->Get_Material()->Set_Texture(m_pBaseMap);

	return S_OK;
}

void CMonster::Awake()
{
	//m_pNavAgent = m_pGameObject->AddComponent<EngineAI::CNavMeshAgent>();
	m_pBodyCollider = m_pGameObject->AddComponent<CBoxCollider>();

	CDebug::LogError("Add Collider");

	m_pBodyCollider->Set_Center(m_sOptions.colliderCenter);
	m_pBodyCollider->Set_Size(m_sOptions.colliderSize);

	if (m_pHeadTF)
	{
		m_pHeadTF->Get_GameObject()->SetLayer(L"MonsterHead");
		m_pHeadCollider = m_pHeadTF->Get_GameObject()->AddComponent<CSphereCollider>();
		m_pHeadCollider->Set_Center(m_sOptions.colliderCenter);
		m_pHeadCollider->Set_Size(m_sOptions.headColliderSize);
	}
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
	Safe_Release(m_pController);
}

CAnimator* CMonster::Get_Animator()
{
	return m_pAnimator;
}

void CMonster::Change_State(const _uint _state)
{
	m_pController->ChangeState(CMonsterController::MonsterState(_state));
}

const CMonster::MonsterStatus& CMonster::Get_Status()
{
	return m_sStatus;
}

CAnimationClip* CMonster::Add_Animation(const wstring _name)
{
	CAnimationClip* anim = CResources::LoadOnScene<CAnimationClip>(m_strName + L"_" + _name + L" (Animation)");
	m_pAnimator->Add_Animation(_name, anim);
	return anim;
}
