#include "cpch.h"
#include "Player.h"
#include "WoodenSword.h"
#include "WoodenBow.h"
#include "DefaultArrow.h"

CPlayer::CPlayer()
	: m_pSkinnedMeshRenderer(nullptr)
	, m_pAnimator(nullptr)
	, m_pRootTransform(nullptr)
	, m_pRHandTransform(nullptr)
	, m_pLHandTransform(nullptr)
	, m_mWeapons({})
	, m_pArrowProto(nullptr)
	, m_qArrowPool()
	, m_vUsedArrows({})
	, m_pEquipArrow(nullptr)
	, m_pEquipWeapon(nullptr)
	, m_sPlayerStatus({})
	, m_eAnimationStatus(Idle)
	, m_fCrtMoveSpeed(0.f)
	, m_vMoveDirection({})
	, m_vPrevMoveDirectoin({})
	, m_fRotateDirection(0.f)
	, m_fPrevRotateDirection(0.f)
	, m_bNotMoveTurning(false)
	, m_bBackMove(false)
	, m_bLockOnMode(false)
	, m_bIsCombatMode(false)
	, m_bPrevLockOnMode(false)
	, m_bIsAttack(false)
	, m_bIsPrevAttack(false)
	, m_bSwordActionDuring(false)
	, m_bBowLoadDuring(false)
	, m_bPrevBowLoadDuring(false)
	, m_fSwordActionEndFrames()
	, m_fAttackComboNT(0.f)
	, m_fBowLoadingNT(0.f)
	, m_bShootReady(false)
	, m_iAttackComboDest(0)
	, m_bIsJump(false)
	, m_bIsPrevJump(false)
	, m_pFocusTransform(nullptr)
	, m_pNavAgent(nullptr)
	, m_pCollider(nullptr)
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

	CTexture* tex = CResources::LoadOnScene<CTexture>(L"Link_Texture (Texture)");

	m_pGameObject->CreateSkinnedMeshHierachy(CResources::LoadSkinnedMeshBuffersOnScene(L"Link_Model (MeshBuffer)"), CResources::LoadSkinnedBonesOnScene(L"Link_Model (MeshBuffer)"), 0.01f, vector3::up() * 180.f);

	m_pRootTransform = Get_Transform()->Get_Child(1);
	m_pLHandTransform = Get_Transform()->Find_ChildRecursive(L"LeftHand");
	m_pRHandTransform = Get_Transform()->Find_ChildRecursive(L"RightHand");

	m_pAnimator = m_pGameObject->AddComponent<CAnimator>();
	m_pAnimator->Add_Animation(L"Idle", CResources::LoadOnScene<CAnimationClip>(L"Link_Idle (Animation)"));
	m_pAnimator->Add_Animation(L"Walk", CResources::LoadOnScene<CAnimationClip>(L"Link_Walk (Animation)"));
	m_pAnimator->Add_Animation(L"LeftWalk", CResources::LoadOnScene<CAnimationClip>(L"Link_LeftWalk (Animation)"));
	m_pAnimator->Add_Animation(L"RightWalk", CResources::LoadOnScene<CAnimationClip>(L"Link_RightWalk (Animation)"));
	m_pAnimator->Add_Animation(L"BackWalk", CResources::LoadOnScene<CAnimationClip>(L"Link_BackWalk (Animation)"));
	m_pAnimator->Add_Animation(L"CombatBackWalk", CResources::LoadOnScene<CAnimationClip>(L"Link_CombatBackWalk (Animation)"));
	m_pAnimator->Add_Animation(L"Run", CResources::LoadOnScene<CAnimationClip>(L"Link_Run (Animation)"));
	m_pAnimator->Add_Animation(L"CombatRun", CResources::LoadOnScene<CAnimationClip>(L"Link_CombatRun (Animation)"));
	m_pAnimator->Add_Animation(L"CombatIdle", CResources::LoadOnScene<CAnimationClip>(L"Link_CombatIdle (Animation)"));
	m_pAnimator->Add_Animation(L"Jump", CResources::LoadOnScene<CAnimationClip>(L"Link_Jump (Animation)"));
	m_pAnimator->Add_Animation(L"SwordAttack1", CResources::LoadOnScene<CAnimationClip>(L"Link_SwordAttack1 (Animation)"));
	m_pAnimator->Add_Animation(L"SwordCombo", CResources::LoadOnScene<CAnimationClip>(L"Link_AttackCombo (Animation)"));
	m_pAnimator->Add_Animation(L"BowLoad", CResources::LoadOnScene<CAnimationClip>(L"Link_BowLoad (Animation)"));
	m_pAnimator->Add_Animation(L"BowAming", CResources::LoadOnScene<CAnimationClip>(L"Link_BowAming (Animation)"));

	CGameObject* swordObj = m_pGameObject->Get_Scene()->Add_GameObject(L"Wooden Sword");
	m_mWeapons.emplace(L"Sword", swordObj->AddComponent<CWoodenSword>());

	CGameObject* bowObj = m_pGameObject->Get_Scene()->Add_GameObject(L"Wooden Bow");
	m_mWeapons.emplace(L"Bow", bowObj->AddComponent<CWoodenBow>());

	for (TRAVERSAL_ITER(m_mWeapons, it))
		(*it).second->Get_GameObject()->SetActive(false);
	
	m_fSwordActionEndFrames[0] = 1.f;
	m_fSwordActionEndFrames[1] = 2.f;
	m_fSwordActionEndFrames[2] = 3.f;

	CGameManager::GetInstance().Set_Player(this);

	m_pArrowProto = m_pGameObject->Get_Scene()->Add_GameObject(L"Default Arrow");
	CDefaultArrow* defaultArrow = m_pArrowProto->AddComponent<CDefaultArrow>();

	m_pArrowProto->SetActive(false);

	return S_OK;
}

void CPlayer::Awake()
{
	m_pNavAgent = m_pGameObject->AddComponent<EngineAI::CNavMeshAgent>();
	m_pCollider = m_pGameObject->AddComponent<CBoxCollider>();
	m_pCollider->Set_Center(vector3(0.f, 0.85f, 0.2f));
	m_pCollider->Set_Size(vector3(0.5f, 1.7f, 0.5f));

	m_sPlayerStatus.crtHp = m_sPlayerStatus.maxHp;

	for (_uint i = 0; i < 10; ++i)
	{
		CGameObject* arrowClone = CGameObject::Instantiate(m_pArrowProto);
		m_qArrowPool.push(arrowClone->GetComponent<CArrow>());
	}

	ChangeWeapon(L"Sword");
}

void CPlayer::Start()
{
	CGameManager::GetInstance().Get_PlayerHUD()->Update_Heart(m_sPlayerStatus.crtHp, m_sPlayerStatus.maxHp);
}

void CPlayer::Update()
{
	PlayerControle();

	if (CInput::GetKeyDown(Alpha1))
		ChangeWeapon(L"Sword");
	if (CInput::GetKeyDown(Alpha2))
		ChangeWeapon(L"Bow");

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
	//CDebug::Log("Enter");
}

void CPlayer::OnCollisionStay(CCollider* _other)
{
}

void CPlayer::OnCollisionExit(CCollider* _other)
{
	//CDebug::Log("Exit");
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

void CPlayer::Set_Focus(CTransform* _transform)
{
	m_pFocusTransform = _transform;
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

CWeapon* CPlayer::ChangeWeapon(const wstring _name)
{
	if (m_pEquipWeapon)
		m_pEquipWeapon->Get_GameObject()->SetActive(false);

	m_pEquipWeapon = m_mWeapons[_name];

	m_pEquipWeapon->Get_GameObject()->SetActive(true);

	m_bSwordActionDuring = false;
	m_bBowLoadDuring = false;

	return m_pEquipWeapon;
}

void CPlayer::PlayerControle()
{
	m_bLockOnMode = CInput::GetKey(SHIFT);
	m_bIsAttack = CInput::GetMouseButtonDown(0);
	m_bIsJump = CInput::GetKeyDown(SPACE);

	if (m_bIsJump && !m_bIsPrevJump)
	{
		PlayJumpAnimation();
	}

	if (m_pEquipWeapon->Get_WeaponType() == CWeapon::WeaponType::Sword)
	{
		if (m_bIsAttack && !m_bIsPrevAttack)
		{
			PlaySwordAnimation();
			m_bSwordActionDuring = true;
			m_fAttackComboNT = 0.f;
			return;
		}

		if (m_bSwordActionDuring)
			PlayerControle_AttackCombo();
	}
	else if (m_pEquipWeapon->Get_WeaponType() == CWeapon::WeaponType::Bow)
	{
		if (m_bIsAttack && !m_bIsPrevAttack)
		{
			PlayBowLoadAnimatoin();
			m_bBowLoadDuring = true;
			m_fAttackComboNT = 0.f;
			m_bShootReady = false;
			CGameManager::GetInstance().Get_PlayerCamera()->ChangeMode(CPlayerCamera::PlayerCamMode::BowAiming);
			CGameManager::GetInstance().Get_PlayerHUD()->OnOffBowCrossHair(true);
			m_pEquipArrow = m_qArrowPool.front();
			m_qArrowPool.pop();
			m_pEquipArrow->Pop();
			return;
		}

		if (m_bBowLoadDuring)
			PlayerControle_BowAction();
		else
		{
			_float lerpValue = Lerp(m_pRootTransform->Get_LocalEulerAngles().y, -180.f, DELTA_TIME * 5.f);
			m_pRootTransform->Set_LocalEulerAnglesY(lerpValue);
		}
	}

	if (m_bLockOnMode != m_bPrevLockOnMode)
	{
		if (m_vMoveDirection == vector3::zero())
			PlayIdleAnimation();
		else
			PlayMoveAnimation();
	}

	m_vMoveDirection = vector3::zero();
	m_fRotateDirection = 0.f;

	if (CInput::GetKey(W))
	{
		m_vMoveDirection += vector3::forward();
	}
	if (CInput::GetKey(S))
	{
		m_vMoveDirection += vector3::back();
	}

	if ((m_vMoveDirection.z != m_vPrevMoveDirectoin.z && m_vMoveDirection != vector3::zero()))
	{
		PlayMoveAnimation();
		m_pAnimator->Pause();
	}

	if (!m_bLockOnMode)
		PlayerControle_NoneLockOn();
	else
		PlayerControle_LockOn();

	if (m_vMoveDirection != m_vPrevMoveDirectoin && m_vMoveDirection == vector3::zero())
	{
		PlayIdleAnimation();
	}

	m_bBackMove = m_vMoveDirection.z < 0.f;

	if (m_bBackMove)
		m_fCrtMoveSpeed = m_sPlayerStatus.moveSpeed * m_sPlayerStatus.backWalkRatio;
	else
		m_fCrtMoveSpeed = m_sPlayerStatus.moveSpeed;

	if (m_bSwordActionDuring)
		m_fCrtMoveSpeed = m_sPlayerStatus.moveSpeed * 0.3f;

	if (m_vMoveDirection != m_vPrevMoveDirectoin)
	{
		PlayMoveAnimation();
	}

	if (m_bLockOnMode != m_bPrevLockOnMode)
	{
		if (!CInput::GetKey(SHIFT))
		{
			if (m_fRotateDirection != 0.f)
				PlayMoveAnimation();
		}
	}

	if (m_vMoveDirection != vector3::zero())
	{
		vector3 moveDir = m_vMoveDirection.normalized();
		Get_Transform()->Add_Position(Get_Transform()->Get_Directions().forward * moveDir.z * m_fCrtMoveSpeed * DELTA_TIME);
		Get_Transform()->Add_Position(Get_Transform()->Get_Directions().right * moveDir.x * m_fCrtMoveSpeed * DELTA_TIME);

		if (m_pAnimator)
			m_pAnimator->Play();
	}

	m_vPrevMoveDirectoin = m_vMoveDirection;
	m_bPrevLockOnMode = m_bLockOnMode;
	m_bIsPrevJump = m_bIsJump;
	m_bPrevBowLoadDuring = m_bBowLoadDuring;
}

void CPlayer::PlayerControle_NoneLockOn()
{
	if (CInput::GetKey(A))
	{
		m_fRotateDirection -= 1.f;
	}
	if (CInput::GetKey(D))
	{
		m_fRotateDirection += 1.f;
	}

	if (m_fRotateDirection != 0.f)
	{
		if (m_fRotateDirection != 0.f) 
			Get_Transform()->Add_EulerAnglesY(m_fRotateDirection * m_sPlayerStatus.turnSpeed * DELTA_TIME);
	}

	if (m_fRotateDirection != m_fPrevRotateDirection && m_vMoveDirection == vector3::zero())
	{
		if (m_fRotateDirection != 0.f)
		{
			PlayMoveAnimation();
			m_bNotMoveTurning = true;
		}
		else
			m_bNotMoveTurning = false;
	}
	else
		m_bNotMoveTurning = false;

	if (m_fRotateDirection != m_fPrevRotateDirection && m_vMoveDirection == vector3::zero())
	{
		if (!CInput::GetKey(A) == !CInput::GetKey(D))
		{
			if (m_pAnimator)
			{
				PlayIdleAnimation();
			}
		}
	}

	m_fPrevRotateDirection = m_fRotateDirection;
}

void CPlayer::PlayerControle_LockOn()
{
	if (m_pFocusTransform)
	{
		if (vector3::Distance(m_pFocusTransform->Get_Position(), Get_Transform()->Get_Position()) > 1.5f)
		{
			quaternion targetQ = Get_Transform()->LookQuaternion(m_pFocusTransform->Get_Position(), CTransform::X | CTransform::Z);
			Get_Transform()->Set_Quaternion(targetQ.Slerp(Get_Transform()->Get_Quaternion(), targetQ, DELTA_TIME * m_sPlayerStatus.focusTurnRatio));
			Get_Transform()->Set_EulerAnglesX(0.f);
			Get_Transform()->Set_EulerAnglesZ(0.f);
		}
	}

	if (CInput::GetKey(A))
	{
		m_vMoveDirection.x -= 1.f;
	}
	if (CInput::GetKey(D))
	{
		m_vMoveDirection.x += 1.f;
	}

	if (m_vMoveDirection.x != m_vPrevMoveDirectoin.x)
	{
		if (m_vMoveDirection.z <= 0.f)
		{
			PlayMoveAnimation();
		}
	}
}

void CPlayer::PlayerControle_AttackCombo()
{
	m_fAttackComboNT += DELTA_TIME;

	_float dest = m_fSwordActionEndFrames[m_iAttackComboDest];

	if (m_fAttackComboNT > dest * 0.3f)
	{
		m_pEquipWeapon->OnOffCollider(true);
	}

	if (m_fAttackComboNT >= dest)
	{
		m_fAttackComboNT = 0.f;
		m_bSwordActionDuring = false;

		if (m_vMoveDirection == vector3::zero())
			PlayIdleAnimation(0.25f);
		else
			PlayMoveAnimation(0.25f);

		m_pEquipWeapon->OnOffCollider(false);
	}
}

void CPlayer::PlayerControle_BowAction()
{
	m_fBowLoadingNT += DELTA_TIME;

	_float dest = 1.f;

	if (m_fBowLoadingNT >= dest)
	{
		m_fBowLoadingNT = 0.f;
		m_pAnimator->SetLoop(true);
		m_pAnimator->Play(L"BowAming", 0.2f);
		m_bShootReady = true;
	}
	else
	{
		_float lerpYValue = Lerp(m_pRootTransform->Get_LocalEulerAngles().y, -90.f, DELTA_TIME * 10.f);
		m_pRootTransform->Set_LocalEulerAnglesY(lerpYValue);
	}

	if (abs(m_vMouseDragDelta.x) > 0.f)
		Get_Transform()->Add_EulerAnglesY(m_vMouseDragDelta.x * DELTA_TIME * m_sPlayerStatus.bowAimDragSpeed);
	if (abs(m_vMouseDragDelta.y) > 0.f)
		CGameManager::GetInstance().Get_PlayerCamera()->AddBowYValue(m_vMouseDragDelta.y * DELTA_TIME * -0.3f);

	if (CInput::GetMouseButtonUp(0))
	{
		m_fBowLoadingNT = 0.f;
		m_bBowLoadDuring = false;

		PlayIdleAnimation(0.25f);
		CGameManager::GetInstance().Get_PlayerCamera()->ChangeMode(CPlayerCamera::PlayerCamMode::Default);
		CGameManager::GetInstance().Get_PlayerHUD()->OnOffBowCrossHair(false);

		if (m_bShootReady)
		{
			m_pEquipArrow->Shoot();
			m_bShootReady = false;
		}
	}
}

void CPlayer::PlayIdleAnimation(const _float _blending)
{
	if (m_bIsJump || m_bSwordActionDuring)
		return;

	if (m_pAnimator)
	{
		if (!m_bLockOnMode)
		{
			m_pAnimator->Play(L"Idle", _blending);
		}
		else
		{
			m_pAnimator->Play(L"CombatIdle", _blending);
		}
	}

	//CDebug::Log("Idle");
}

void CPlayer::PlayMoveAnimation(const _float _blending)
{
	if (m_bIsJump || m_bSwordActionDuring)
		return;

	if (m_pAnimator)
	{
		m_pAnimator->SetLoop(true);

		if (!m_bLockOnMode)
		{
			if (m_vMoveDirection.z > 0.f)
				m_pAnimator->Play(L"Run", _blending);
			else if (m_vMoveDirection.z < 0.f)
				m_pAnimator->Play(L"BackWalk", _blending);
			else if (m_fRotateDirection != 0)
				m_pAnimator->Play(L"Walk", _blending);
		}
		else
		{
			if (m_vMoveDirection.z > 0.f)
				m_pAnimator->Play(L"CombatRun", _blending);
			else
			{
				if (m_vMoveDirection.x < 0.f)
					m_pAnimator->Play(L"LeftWalk", _blending);
				else if (m_vMoveDirection.x > 0.f)
					m_pAnimator->Play(L"RightWalk", _blending);
				else if (m_vMoveDirection.z < 0.f)
					m_pAnimator->Play(L"CombatBackWalk", _blending);
				else
					PlayIdleAnimation(_blending);
			}
		}
	}

	//CDebug::Log("CombatRun");
}

void CPlayer::PlayJumpAnimation(const _float _blending)
{
	if (m_bSwordActionDuring)
		return;

	if (m_pAnimator)
	{
		m_pAnimator->SetLoop(false);
		m_pAnimator->Play(L"Jump", _blending);
	}

	//CDebug::Log("Jump");
}

void CPlayer::PlaySwordAnimation()
{
	if (m_bIsJump || m_bSwordActionDuring)
		return;

	if (m_pAnimator)
	{
		m_pAnimator->SetLoop(true);
		m_pAnimator->Play(L"SwordCombo", 0.1f);
	}
}

void CPlayer::PlayBowLoadAnimatoin()
{
	if (m_bIsJump || m_bBowLoadDuring)
		return;

	if (m_pAnimator)
	{
		m_pAnimator->SetLoop(false);
		m_pAnimator->Play(L"BowLoad", 0.1f);
	}
}
