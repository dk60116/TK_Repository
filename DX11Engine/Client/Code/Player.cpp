#include "cpch.h"
#include "Player.h"

CPlayer::CPlayer()
	: m_pSkinnedMeshRenderer(nullptr)
	, m_pAnimator(nullptr)
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

HRESULT CPlayer::Initialize()
{
	if (FAILED(__super::Initialize()))
		return E_FAIL;

	CTexture* tex = CResources::GetInstance().LoadOnScene<CTexture>(L"Link_Texture (Texture)");

	m_pGameObject->CreateSkinnedMeshHierachy(CResources::GetInstance().LoadSkinnedMeshBuffersOnScene(L"Link_Model (MeshBuffer)"), CResources::GetInstance().LoadSkinnedBonesOnScene(L"Link_Model (MeshBuffer)"));

	m_pAnimator = m_pGameObject->AddComponent<CAnimator>();
	m_pAnimator->Add_Animation(L"Idle", CResources::GetInstance().LoadOnScene<CAnimationClip>(L"Link_Idle (Animation)"));
	m_pAnimator->Add_Animation(L"Walk", CResources::GetInstance().LoadOnScene<CAnimationClip>(L"Link_Walk (Animation)"));
	m_pAnimator->Add_Animation(L"LeftWalk", CResources::GetInstance().LoadOnScene<CAnimationClip>(L"Link_LeftWalk (Animation)"));
	m_pAnimator->Add_Animation(L"RightWalk", CResources::GetInstance().LoadOnScene<CAnimationClip>(L"Link_RightWalk (Animation)"));
	m_pAnimator->Add_Animation(L"BackWalk", CResources::GetInstance().LoadOnScene<CAnimationClip>(L"Link_BackWalk (Animation)"));
	m_pAnimator->Add_Animation(L"CombatBackWalk", CResources::GetInstance().LoadOnScene<CAnimationClip>(L"Link_CombatBackWalk (Animation)"));
	m_pAnimator->Add_Animation(L"Run", CResources::GetInstance().LoadOnScene<CAnimationClip>(L"Link_Run (Animation)"));
	m_pAnimator->Add_Animation(L"CombatRun", CResources::GetInstance().LoadOnScene<CAnimationClip>(L"Link_CombatRun (Animation)"));
	m_pAnimator->Add_Animation(L"CombatIdle", CResources::GetInstance().LoadOnScene<CAnimationClip>(L"Link_CombatIdle (Animation)"));
	m_pAnimator->Add_Animation(L"SwordAttack1", CResources::GetInstance().LoadOnScene<CAnimationClip>(L"Link_SwordAttack1 (Animation)"));
	m_pAnimator->Add_Animation(L"AttackCombo", CResources::GetInstance().LoadOnScene<CAnimationClip>(L"Link_AttackCombo (Animation)"));

	m_pAnimator->SetLoop(true);

	CGameObject* swordObj = m_pGameObject->Get_Scene()->Add_GameObject(L"Wooden Sword");
	swordObj->CreateMeshHierachy(CResources::GetInstance().LoadMeshBuffersOnScene(L"WoodenSword (MeshBuffer)"));

	swordObj->Get_Transform()->SetParent(Get_Transform()->Find_ChildRecursive(L"RightHand"));

	//Get_Transform()->Get_Child(0)->Set_LocalScale(0.01f);
	//Get_Transform()->Get_Child(1)->Set_LocalScale(0.01f);
	//Get_Transform()->Get_Child(0)->Set_LocalEulerAnglesY(180.f);
	//Get_Transform()->Get_Child(1)->Set_LocalEulerAnglesY(180.f);

	//m_pAnimator->Set_PlaybackSpeed(0.1f);
	//m_pAnimator->Play(L"Run");

	return S_OK;
}

void CPlayer::Awake()
{
}

void CPlayer::Start()
{
}

void CPlayer::Update()
{
	if (CInput::GetInstance().GetKeyDown_Editor(N))
	{
		if (m_pAnimator)
			m_pAnimator->Play(L"Idle", 0.2f);
	}

	if (CInput::GetInstance().GetKeyDown_Editor(M))
	{
		if (m_pAnimator)
			m_pAnimator->Play(L"Run", 0.1f);
	}

	if (CInput::GetInstance().GetKeyDown_Editor(C))
	{
		if (m_pAnimator)
			m_pAnimator->Play(L"CombatIdle", 0.1f);
	}

	if (CInput::GetInstance().GetKeyDown_Editor(B))
	{
		if (m_pAnimator)
			m_pAnimator->Play(L"Link_SwordAttack1", 0.1f);
	}

	if (CInput::GetInstance().GetKeyDown_Editor(V))
	{
		if (m_pAnimator)
			m_pAnimator->Play(L"AttackCombo", 0.1f);
	}

	if (CInput::GetInstance().GetKeyDown_Editor(H))
	{
		if (m_pAnimator)
			m_pAnimator->Play(L"BackWalk", 0.1f);
	}

	if (CInput::GetInstance().GetKeyDown_Editor(X))
	{
		if (m_pAnimator)
			m_pAnimator->Stop();
	}

	if (CInput::GetInstance().GetKeyDown_Editor(Z))
	{
		if (m_pAnimator)
			m_pAnimator->Pause();
	}

	if (CInput::GetInstance().GetKeyDown_Editor(G))
	{
		if (m_pAnimator)
			m_pAnimator->Play();
	}

	PlayerControle();
}

void CPlayer::OnDestroy()
{
}

void CPlayer::PlayerControle()
{
	m_bLockOnMode = CInput::GetInstance().GetKey(SHIFT);

	if (m_bLockOnMode != m_bPrevLockOnMode)
	{
		if (m_vMoveDirection == vector3::zero())
			PlayIdleAnimation();
		if (m_vMoveDirection.z > 0)
			PlayRunAnimation();
		if (m_vMoveDirection.z < 0)
			PlayBackWalkAnimation();
	}

	m_vMoveDirection = vector3::zero();
	m_fRotateDirection = 0.f;

	if (CInput::GetInstance().GetKey(W))
	{
		m_vMoveDirection += vector3::forward();
	}
	if (CInput::GetInstance().GetKey(S))
	{
		m_vMoveDirection += vector3::back();
	}

	if ((m_vMoveDirection != m_vPrevMoveDirectoin && m_vMoveDirection != vector3::zero()))
	{
		if (m_pAnimator)
		{
			if (m_vMoveDirection.z > 0)
				PlayRunAnimation();
			else
				PlayBackWalkAnimation();
			m_pAnimator->Pause();
		}
	}

	if (!m_bLockOnMode)
		PlayerControle_NoneLockOn();
	else
		PlayerControle_LockOn();

	if (m_vMoveDirection != m_vPrevMoveDirectoin && m_vMoveDirection == vector3::zero())
	{
		if (m_pAnimator)
		{
			PlayIdleAnimation();
		}
	}

	m_bBackMove = m_vMoveDirection.z < 0.f;

	if (m_bBackMove)
		m_fCrtMoveSpeed = m_sPlayerStatus.moveSpeed * m_sPlayerStatus.backWalkRatio;
	else
		m_fCrtMoveSpeed = m_sPlayerStatus.moveSpeed;

	if (CInput::GetInstance().GetKeyUp(W) || CInput::GetInstance().GetKeyUp(S))
	{
		if (m_fRotateDirection != 0.f)
		{
			if (m_pAnimator)
				m_pAnimator->Play(L"Walk", 0.1f);
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
}

void CPlayer::PlayerControle_NoneLockOn()
{
	if (CInput::GetInstance().GetKey(A))
	{
		m_fRotateDirection -= 1.f;
	}
	if (CInput::GetInstance().GetKey(D))
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
			if (m_pAnimator)
			{
				m_pAnimator->Play(L"Walk", 0.1f);
				m_bNotMoveTurning = true;
			}
		}
		else
			m_bNotMoveTurning = false;
	}
	else
		m_bNotMoveTurning = false;

	if (m_fRotateDirection != m_fPrevRotateDirection && m_vMoveDirection == vector3::zero())
	{
		if (!CInput::GetInstance().GetKey(A) == !CInput::GetInstance().GetKey(D))
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
	if (CInput::GetInstance().GetKey(A))
	{
		m_vMoveDirection.x -= 1.f;
	}
	if (CInput::GetInstance().GetKey(D))
	{
		m_vMoveDirection.x += 1.f;
	}

	if (m_vMoveDirection.x != m_vPrevMoveDirectoin.x)
	{
		if (m_vMoveDirection.x < 0.f)
		{
			m_pAnimator->Play(L"LeftWalk", 0.1f);
			CDebug::LogError("Left");
		}
		else if (m_vMoveDirection.x > 0.f)
		{
			m_pAnimator->Play(L"RightWalk", 0.1f);
			CDebug::LogError("Right");
		}
	}
}

void CPlayer::PlayIdleAnimation()
{
	if (m_pAnimator)
	{
		if (!m_bLockOnMode)
			m_pAnimator->Play(L"Idle", 0.1f);
		else
			m_pAnimator->Play(L"CombatIdle", 0.1f);
	}
}

void CPlayer::PlayRunAnimation()
{
	if (m_pAnimator)
	{
		if (!m_bLockOnMode)
			m_pAnimator->Play(L"Run", 0.1f);
		else
			m_pAnimator->Play(L"CombatRun", 0.1f);
	}
}

void CPlayer::PlayBackWalkAnimation()
{
	if (m_pAnimator)
	{
		if (!m_bLockOnMode)
			m_pAnimator->Play(L"BackWalk", 0.1f);
		else
			m_pAnimator->Play(L"CombatBackWalk", 0.1f);
	}
}
