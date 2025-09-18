#include "cpch.h"
#include "BossMonster.h"
#include "Monster.h"
#include "MonsterController.h"
#include "MonsterPartCollision.h"
#include "Weapon.h"

CMonster::CMonster()
	: m_strMonsterName(L"")
	, m_vMeshRenderers({})
	, m_pBaseMap(nullptr)
	, m_pNormalMap(nullptr)
	, m_pAnimator(nullptr)
	, m_pController(nullptr)
	, m_pNavAgent(nullptr)
	, m_sOptions({})
	, m_sStatus({})
	, m_pHeadTF(nullptr)
	, m_pBodyTF(nullptr)
	, m_pBodyCollider(nullptr)
	, m_pRigidBody(nullptr)
	, m_mPartColList({})
{
	m_strName = L"Monster";
}

CMonster::~CMonster()
{
}

HRESULT CMonster::Initialize(void* _desc)
{
	m_bOnlyCloneComponent = true;

	if (FAILED(__super::Initialize(_desc)))
		return E_FAIL;

	m_pGameObject->SetLayer(L"Monster");

	wstring skinnedMeshBufferName = m_strMonsterName + L"_Model (MeshBuffer)";
	m_vMeshRenderers = m_pGameObject->CreateSkinnedMeshHierachy(CResources::LoadSkinnedMeshBuffersOnScene(skinnedMeshBufferName), CResources::LoadSkinnedBonesOnScene(skinnedMeshBufferName), m_sOptions.scaleFactor, vector3::up() * 180.f);
	m_pAnimator = m_pGameObject->AddComponent<CAnimator>();

	m_pBaseMap = CResources::LoadOnScene<CTexture>(m_strMonsterName + L"_BaseMap (Texture)");

	if (m_pBaseMap)
		m_pBaseMap->AddRef();

	if (!dynamic_cast<CBossMonster*>(this))
	{
		Add_Animation(L"Idle");
		Add_Animation(L"Walk");
		Add_Animation(L"WalkTurn_Left");
		Add_Animation(L"WalkTurn_Right");
		Add_Animation(L"LookAround");
		Add_Animation(L"Find");
		Add_Animation(L"Run");
		Add_Animation(L"Attack01");
		Add_Animation(L"GetHit_Front");
		Add_Animation(L"Death");
	}

	for (size_t i = 0; i < m_vMeshRenderers.size(); ++i)
		m_vMeshRenderers[i]->Get_Material()->Set_Texture(m_pBaseMap);

	return S_OK;
}

void CMonster::Awake()
{
	if (m_pHeadTF)
	{
		CMonsterPartCollision* headCol = m_pHeadTF->Get_GameObject()->AddComponent<CMonsterPartCollision>();
		headCol->Set_Monster(this);
		headCol->SetCenter(m_sOptions.headColliderCenter);
		headCol->SetSize(m_sOptions.headColliderSize);
		m_mPartColList.emplace(L"Head", headCol);
	}

	if (m_pBodyTF)
	{
		CMonsterPartCollision* bodyCol = m_pBodyTF->Get_GameObject()->AddComponent<CMonsterPartCollision>();
		bodyCol->Set_Monster(this);
		bodyCol->SetCenter(m_sOptions.bodyColliderCenter);
		bodyCol->SetSize(m_sOptions.bodyColliderSize);
		m_mPartColList.emplace(L"Body", bodyCol);
	}

	if (!dynamic_cast<CBossMonster*>(this))
	{
		if (!m_pController)
			m_pController = m_pGameObject->AddComponent<CMonsterController>();

		if (m_pController)
			m_pController->Set_Monster(this);
	}

	if (!m_pBodyCollider)
		m_pBodyCollider = m_pGameObject->AddComponent<CBoxCollider>();
	if (!m_pRigidBody)
		m_pRigidBody = m_pGameObject->AddComponent<CRigidBody>();

	m_pBodyCollider->Set_Center(m_sOptions.colliderCenter);
	m_pBodyCollider->Set_Size(m_sOptions.colliderSize);
	m_pBodyCollider->Set_SmoothTranslation(true);

	m_sStatus.crtHp = m_sStatus.maxHp;
}

void CMonster::Start()
{
	m_pRigidBody->SetUseGravity(true);
}

void CMonster::Update()
{
	if (CInput::GetKeyDown_Editor(L))
	{
		m_pRigidBody->SetUseGravity(true);
	}
}

void CMonster::OnEnable()
{
	m_pAnimator->SetLoop(true);
	m_pAnimator->Play(L"Idle");
}

void CMonster::OnCollisionEnter(CCollider* _other)
{
}

void CMonster::OnDestroy()
{
	Safe_Release(m_pBaseMap);
}

const wstring& CMonster::Get_MonsterName()
{
	return m_strMonsterName;
}

const CMonster::MonsterOptions& CMonster::Get_Option() const
{
	return m_sOptions;
}

CAnimator* CMonster::Get_Animator() const
{
	return m_pAnimator;
}

CMonsterController* CMonster::Get_Controller()
{
	return m_pController;
}

void CMonster::Change_State(const _uint _state)
{
	m_pController->ChangeState(CMonsterController::MonsterState(_state));
}

const CMonster::MonsterStatus& CMonster::Get_Status()
{
	return m_sStatus;
}

void CMonster::Get_Damage(CWeapon* _weapon)
{
	if (!_weapon)
		return;

	m_sStatus.crtHp -= _weapon->Get_Stat().attack;

	const vector3& myPos = Get_Transform()->Get_Position();
	vector3 weaponPos = _weapon->Get_Transform()->Get_Position();
	weaponPos.y = myPos.y;
	vector3 knockbackDir = (myPos - weaponPos).normalized();
	m_pRigidBody->AddForce(knockbackDir * _weapon->Get_Stat().knockbackPower);

	if (m_sStatus.crtHp <= 0)
	{
		m_sStatus.crtHp = 0;
		m_pController->ChangeState(CMonsterController::Death);
		m_pController->SetDead();
	}
	else
		m_pController->ChangeState(CMonsterController::GetHit);
}

void CMonster::Death()
{
	m_sStatus.isDead = true;
}

void CMonster::PlayTurn(const CMonsterController::TurnDir _dir)
{
	m_pAnimator->SetLoop(true);

	switch (_dir)
	{
	case CMonsterController::TurnDir::None:
		m_pAnimator->Play(L"Idle", 0.2f);
		return;
	case CMonsterController::TurnDir::Left:
		m_pAnimator->Play(L"WalkTurn_Left", 0.1f);
		return;
	case CMonsterController::TurnDir::Right:
		m_pAnimator->Play(L"WalkTurn_Right", 0.1f);
		return;
	}
}

CAnimationClip* CMonster::Add_Animation(const wstring _name)
{
	CAnimationClip* anim = CResources::LoadOnScene<CAnimationClip>(m_strMonsterName + L"_" + _name + L" (Animation)");
	m_pAnimator->Add_Animation(_name, anim);
	return anim;
}
