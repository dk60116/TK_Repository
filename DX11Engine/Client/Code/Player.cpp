#include "cpch.h"
#include "Player.h"
#include "Inventory.h"
#include "WoodenSword.h"
#include "WoodenBow.h"
#include "DefaultArrow.h"

CPlayer::CPlayer()
	: m_pController(nullptr)
	, m_pInventroy(nullptr)
	, m_pSkinnedMeshRenderer(nullptr)
	, m_pAnimator(nullptr)
	, m_pRootTransform(nullptr)
	, m_pRHandTransform(nullptr)
	, m_pLHandTransform(nullptr)
	, m_mWeapons({})
	, m_pArrowProto(nullptr)
	, m_mArrowPool()
	, m_vUsedArrows({})
	, m_pEquipArrow(nullptr)
	, m_pEquipWeapon(nullptr)
	, m_sPlayerStatus({})
	, m_pNavAgent(nullptr)
	, m_pCollider(nullptr)
	, m_pRigidBody(nullptr)
	, m_vPrevMousePos({})
	, m_vMouseDragDelta({})
{
	m_strName = L"Player";
}

CPlayer::~CPlayer()
{
}

CPlayer* CPlayer::Create()
{
	return new CPlayer();
}

CComponent* CPlayer::Clone() const
{
	CPlayer* clone = new CPlayer();

	return clone;
}

HRESULT CPlayer::Initialize()
{
	if (FAILED(__super::Initialize()))
		return E_FAIL;

	m_pGameObject->SetLayer(L"Player");
	m_pGameObject->SetTag(L"Player");

	m_pGameObject->CreateSkinnedMeshHierachy(CResources::LoadSkinnedMeshBuffersOnScene(L"Link_Model (MeshBuffer)"), CResources::LoadSkinnedBonesOnScene(L"Link_Model (MeshBuffer)"), 0.01f, vector3::up() * 180.f);

	m_pRootTransform = Get_Transform()->Get_Child(1);
	m_pLHandTransform = Get_Transform()->Find_ChildRecursive(L"LeftHand");
	m_pRHandTransform = Get_Transform()->Find_ChildRecursive(L"RightHand");

	m_pCollider = m_pGameObject->AddComponent<CBoxCollider>();
	m_pCollider->Set_Center(vector3(0.f, 0.85f, 0.2f));
	m_pCollider->Set_Size(vector3(0.5f, 1.7f, 0.5f));

	m_pRigidBody = m_pGameObject->AddComponent<CRigidBody>();

	m_pAnimator = m_pGameObject->AddComponent<CAnimator>();
	m_pAnimator->Add_Animation(L"Idle", CResources::LoadOnScene<CAnimationClip>(L"Link_Idle (Animation)"));
	m_pAnimator->Add_Animation(L"Walk", CResources::LoadOnScene<CAnimationClip>(L"Link_Walk (Animation)"));
	m_pAnimator->Add_Animation(L"LeftWalk", CResources::LoadOnScene<CAnimationClip>(L"Link_LeftWalk (Animation)"));
	m_pAnimator->Add_Animation(L"RightWalk", CResources::LoadOnScene<CAnimationClip>(L"Link_RightWalk (Animation)"));
	m_pAnimator->Add_Animation(L"BackWalk", CResources::LoadOnScene<CAnimationClip>(L"Link_BackWalk (Animation)"));
	m_pAnimator->Add_Animation(L"CombatBackWalk", CResources::LoadOnScene<CAnimationClip>(L"Link_CombatBackWalk (Animation)"));
	m_pAnimator->Add_Animation(L"Run", CResources::LoadOnScene<CAnimationClip>(L"Link_Run (Animation)"));
	m_pAnimator->Add_Animation(L"RunJump", CResources::LoadOnScene<CAnimationClip>(L"Link_RunJump (Animation)"));
	m_pAnimator->Add_Animation(L"CombatRun", CResources::LoadOnScene<CAnimationClip>(L"Link_CombatRun (Animation)"));
	m_pAnimator->Add_Animation(L"CombatIdle", CResources::LoadOnScene<CAnimationClip>(L"Link_CombatIdle (Animation)"));
	m_pAnimator->Add_Animation(L"Jump", CResources::LoadOnScene<CAnimationClip>(L"Link_Jump (Animation)"));
	m_pAnimator->Add_Animation(L"SwordAttack1", CResources::LoadOnScene<CAnimationClip>(L"Link_SwordAttack1 (Animation)"));
	m_pAnimator->Add_Animation(L"SwordCombo", CResources::LoadOnScene<CAnimationClip>(L"Link_AttackCombo (Animation)"));
	m_pAnimator->Add_Animation(L"BowLoad", CResources::LoadOnScene<CAnimationClip>(L"Link_BowLoad (Animation)"));
	m_pAnimator->Add_Animation(L"BowAiming", CResources::LoadOnScene<CAnimationClip>(L"Link_BowAiming (Animation)"));
	m_pAnimator->Add_Animation(L"LadderUp", CResources::LoadOnScene<CAnimationClip>(L"Link_LadderUp (Animation)"));
	m_pAnimator->Add_Animation(L"LadderDown", CResources::LoadOnScene<CAnimationClip>(L"Link_LadderDown (Animation)"));
	m_pAnimator->Add_Animation(L"LadderOutUp", CResources::LoadOnScene<CAnimationClip>(L"Link_LadderOutUp (Animation)"));

	CGameObject* swordObj = m_pGameObject->Get_Scene()->Add_GameObject(L"Wooden Sword");
	m_mWeapons.emplace(L"Sword", swordObj->AddComponent<CWoodenSword>());

	CGameObject* bowObj = m_pGameObject->Get_Scene()->Add_GameObject(L"Wooden Bow");
	m_mWeapons.emplace(L"Bow", bowObj->AddComponent<CWoodenBow>());

	for (TRAVERSAL_ITER(m_mWeapons, it))
		(*it).second->Get_GameObject()->SetActive(false);

	CGameManager::GetInstance().Set_Player(this);

	m_pArrowProto = m_pGameObject->Get_Scene()->Add_GameObject(L"Default Arrow");
	CDefaultArrow* defaultArrow = m_pArrowProto->AddComponent<CDefaultArrow>();

	m_pArrowProto->SetActive(false);

	if (!m_pController)
	{
		m_pController = m_pGameObject->AddComponent<CPlayerController>();

		if (m_pController)
			m_pController->Set_Player(this);
		else
			return E_FAIL;
	}

	if (!m_pInventroy)
	{
		m_pInventroy = m_pGameObject->AddComponent<CInventory>();

		CGameManager::GetInstance().Set_Inventory(m_pInventroy);
	}

	return S_OK;
}

void CPlayer::Awake()
{
	Get_Transform()->Set_Position(127.5f, 0.f, -67.470f);
	Get_Transform()->Set_Position(vector3::zero());

	m_sPlayerStatus.crtHp = m_sPlayerStatus.maxHp;
}

void CPlayer::Start()
{
	ChangeArrow(L"DefaultArrow");

	for (_uint i = 0; i < 10; ++i)
	{
		CGameObject* arrowClone = CGameObject::Instantiate(m_pArrowProto);
		m_mArrowPool[m_strCrtArrow].push(arrowClone->GetComponent<CArrow>());
	}

	CGameManager::GetInstance().Get_PlayerHUD()->Update_Heart(m_sPlayerStatus.crtHp, m_sPlayerStatus.maxHp);

	m_pRigidBody->SetUseGravity(true);
}

void CPlayer::Update()
{
	static wstring weaponName = L"";

	if (CInput::GetKeyDown(Alpha1))
	{
		weaponName = L"Sword";
		if (m_pInventroy->HasItem(weaponName))
			ChangeWeapon(weaponName);
	}
	if (CInput::GetKeyDown(Alpha2))
	{
		weaponName = L"Bow";
		if (m_pInventroy->HasItem(weaponName))
			ChangeWeapon(weaponName);
	}

	vector2Int currentMouse = CInput::GetMousePos();
	m_vMouseDragDelta = (currentMouse - m_vPrevMousePos).to_vector2();

	if (CInput::GetKeyDown(P))
	{
		RecoverHp(1);
	}

	if (CInput::GetKeyDown(O))
	{
		GetDamage(1);
	}

	if (CInput::GetKeyDown_Editor(L))
	{
		m_pRigidBody->SetUseGravity(!m_pRigidBody->UseGravity());
	}
}

void CPlayer::LateUpdate()
{
	m_vPrevMousePos = CInput::GetMousePos();
}

void CPlayer::OnDestroy()
{
}

void CPlayer::OnCollisionEnter(CCollider* _other)
{
}

void CPlayer::OnCollisionStay(CCollider* _other)
{
}

void CPlayer::OnCollisionExit(CCollider* _other)
{
}

CPlayerController* CPlayer::Get_Controller()
{
	return m_pController;
}

CAnimator* CPlayer::Get_Animator() const
{
	return m_pAnimator;
}

CRigidBody* CPlayer::Get_RigidBody() const
{
	return m_pRigidBody;
}

const CPlayer::PlayerStatus& CPlayer::Get_Status()
{
	return m_sPlayerStatus;
}

CTransform* CPlayer::Get_Root() const
{
	return m_pRootTransform;
}

CTransform* CPlayer::Get_Hand(HandType _hand)
{
	switch (_hand)
	{
	case CPlayer::HandType::Left:
		return m_pLHandTransform;
	case CPlayer::HandType::Right:
		return m_pRHandTransform;
	}

	return nullptr;
}

void CPlayer::RecoverHp(const _uint _value)
{
	m_sPlayerStatus.crtHp += _value;
	m_sPlayerStatus.crtHp = min(m_sPlayerStatus.crtHp, m_sPlayerStatus.maxHp);
	CGameManager::GetInstance().Get_PlayerHUD()->Update_Heart(m_sPlayerStatus.crtHp, m_sPlayerStatus.maxHp);
}

void CPlayer::GetDamage(const _uint _damage)
{
	m_sPlayerStatus.crtHp -= _damage;
	m_sPlayerStatus.crtHp = max(m_sPlayerStatus.crtHp, 0);
	CGameManager::GetInstance().Get_PlayerHUD()->Update_Heart(m_sPlayerStatus.crtHp, m_sPlayerStatus.maxHp);
}

CWeapon* CPlayer::Get_EqupWeapon() const
{
	return m_pEquipWeapon;
}

CWeapon* CPlayer::ChangeWeapon(const wstring _name)
{
	if (!(m_pController->Get_State() == CPlayerController::Idle || m_pController->Get_State() == CPlayerController::Move))
		return m_pEquipWeapon;

	if (m_pEquipWeapon)
		m_pEquipWeapon->Get_GameObject()->SetActive(false);

	m_pEquipWeapon = m_mWeapons[_name];

	m_pEquipWeapon->Get_GameObject()->SetActive(true);

	return m_pEquipWeapon;
}

void CPlayer::ChangeArrow(const wstring _name)
{
	m_strCrtArrow = _name;
}

queue<CArrow*>& CPlayer::Get_ArrowContainer(const wstring _name)
{
	return m_mArrowPool[_name];
}

void CPlayer::PlayIdleAnimation(const _bool _combat, const _float _blending)
{
	if (m_pAnimator)
	{
		m_pAnimator->SetLoop(true);

		if (!_combat)
		{
			m_pAnimator->Play(L"Idle", _blending);
		}
		else
		{
			m_pAnimator->Play(L"CombatIdle", _blending);
		}
	}
}

void CPlayer::PlayMoveAnimation(const vector3& _dir, const _float _rot, const _float _blending)
{
	if (m_pAnimator)
	{
		m_pAnimator->SetLoop(true);

		if (true)
		{
			if (_dir.z > 0.f)
			{
				m_pAnimator->Play(L"Run", _blending);
			}
			else if (_dir.z < 0.f)
			{
				m_pAnimator->Play(L"BackWalk", _blending);
			}
			else if (_rot != 0)
			{
				m_pAnimator->Play(L"Walk", _blending);
			}
		}
		else
		{
			if (_dir.z > 0.f)
			{
				m_pAnimator->Play(L"CombatRun", _blending);
			}
			else
			{
				if (_dir.x < 0.f)
				{
					m_pAnimator->Play(L"LeftWalk", _blending);
				}
				else if (_dir.x > 0.f)
				{
					m_pAnimator->Play(L"RightWalk", _blending);
				}
				else if (_dir.z < 0.f)
				{
					m_pAnimator->Play(L"CombatBackWalk", _blending);
				}
				else
				{
					PlayIdleAnimation(_blending);
				}
			}
		}
	}
}

void CPlayer::PlayJumpAnimation(const vector3& _dir, const _float _blending)
{
	if (m_pAnimator)
	{
		m_pAnimator->SetLoop(false);

		if (_dir.z == 0)
			m_pAnimator->Play(L"Jump", _blending);
		else
			m_pAnimator->Play(L"RunJump", _blending);
	}
}

void CPlayer::PlaySwordAnimation()
{
	if (m_pAnimator)
	{
		m_pAnimator->SetLoop(false);
		m_pAnimator->Play(L"SwordCombo", 0.1f);
	}
}

void CPlayer::PlayBowLoadAnimatoin()
{
	if (m_pAnimator)
	{
		m_pAnimator->SetLoop(false);
		m_pAnimator->Play(L"BowLoad", 0.1f);
	}
}

void CPlayer::PlayBowAimingAnimation()
{
	if (m_pAnimator)
	{
		m_pAnimator->SetLoop(true);
		m_pAnimator->Play(L"BowAiming", 0.2f);
	}
}

void CPlayer::PlayLadderAnimation(const _byte _dir, const _float _blending)
{
	m_pAnimator->SetLoop(true);

	if (_dir == 1)
	{
		m_pAnimator->Play(L"LadderUp", _blending);
	}
	else if (_dir == -1)
	{
		m_pAnimator->Play(L"LadderDown", _blending);
	}
	else
	{
		m_pAnimator->Play(L"LadderUp", _blending);
	}
}

void CPlayer::PlayLadderOutAnimation(const _float _blending)
{
	m_pAnimator->SetLoop(false);

	m_pAnimator->Play(L"LadderOutUp", _blending);
}

void CPlayer::PopArrow()
{
	m_pEquipArrow = m_mArrowPool[m_strCrtArrow].front();
	m_pEquipArrow->Pop();
	m_mArrowPool[m_strCrtArrow].pop();
}

void CPlayer::ReturnArrow()
{
	m_pEquipArrow->Return();
	m_pEquipArrow = nullptr;
}

void CPlayer::ShootArrow()
{
	m_pEquipArrow->Shoot();
}

void CPlayer::OnOffGravity(const _bool _on)
{
	if (!m_pRigidBody)
		return;

	m_pRigidBody->SetUseGravity(_on);
}

const vector3& CPlayer::Get_GA()
{
	return m_pRigidBody->Get_GA();
}
