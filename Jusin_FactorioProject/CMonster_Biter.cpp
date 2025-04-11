#include "CMonster_Biter.h"
#include "CMonsterManager.h"
#include "CRandom.h"

CMonster_Biter::CMonster_Biter()
	: m_iDirection(4)
	, m_pHpSlot()
{
}

CMonster_Biter::~CMonster_Biter()
{
}

void CMonster_Biter::Awake()
{
	CGameObject* renderObj = m_pGameObject->getScene()->AddObject(L"MonsterRenderer", Layer::Monster);
	m_pAnim = dynamic_cast<CAnimator*>(renderObj->AddComponent(new CAnimator()));
	m_pRender = dynamic_cast<CSpriteRenderer*>(renderObj->AddComponent(new CSpriteRenderer(m_pAnim)));
	renderObj->SetParent(m_pGameObject);
	renderObj->getTransform().SetScale(vector2(2.f, 1.5f) * 2.f);

	int anmCount = 16;
	float anmFrameLate = 0.05f;
	vector2 anmOffset = vector2::zero();

	wstring runSprite = L"Monster_Biter_Run";
	vector2Int runSize = vector2Int(199, 155);
	int runCount = 16;

	for (int i = 0; i < anmCount; ++i)
		m_pAnim->CreateAnimation(L"Run_" + to_wstring(i), runSprite, runCount, anmOffset, runSize, i, anmFrameLate);

	wstring atkSprite = L"Monster_Biter_Attack";
	vector2Int atkSize = vector2Int(178, 174);
	int atkCount = 8;

	for (int i = 0; i < anmCount; ++i)
		m_pAnim->CreateAnimation(L"Attack_" + to_wstring(i), atkSprite, atkCount, anmOffset, atkSize, i, anmFrameLate * 2.f);

	wstring deathSprite = L"Monster_Biter_Death";
	vector2Int deathSize = vector2Int(245, 192);
	
	m_pAnim->CreateAnimation(L"Death_" + to_wstring(0), deathSprite, anmCount, anmOffset, deathSize, 0, anmFrameLate, false);

	m_pAnim->SetAnimation(L"Attack_4");

	m_sStat.maxHp = 100;
	m_sStat.crtHp = m_sStat.maxHp;
	m_sStat.moveSpeed = CRandom::GetInstance().Range(5.f, 5.5f);
	m_pTargetObj = CSceneManager::GetInstance().getCurrentscene()->FindObjectOfType<CPlayer>(Layer::Player)->getObj();

	CMonsterManager::GetInstance().AddMonster(this);
}

void CMonster_Biter::Reset()
{
}

void CMonster_Biter::Update()
{
	Update_();

	if (m_bAlibe && m_bIsAttack && m_fAttackLockTime > 0.f)
	{
		m_fAttackLockTime -= DELTA_TIME;

		if (m_fAttackLockTime < 0.f)
		{
			m_bIsAttack = false;
			m_fAttackLockTime = 0.f;
			CSceneManager::GetInstance().getCurrentscene()->FindObjectOfType<CPlayer>(Layer::Player)->AddHp(-5.f);
		}
	}

	if (m_bAlibe && m_pTargetObj)
	{
		vector2 myPos = m_pGameObject->getTransform().getPosition();
		vector2 targetPos = m_pTargetObj->getTransform().getPosition();
		vector2 dir = targetPos - myPos;

		float angle = atan2f(dir.y, dir.x);
		angle += PI / 2.0f;
		if (angle < 0)
			angle += XM_2PI;

		m_iDirection = static_cast<int>((angle / XM_2PI) * 16.0f) % 16;

		float distance = dir.length();

		if (m_fAttackLockTime > 0.f)
		{
			m_pAnim->SetAnimation(L"Attack_" + to_wstring(m_iDirection));
			return;
		}

		if (distance > 1.5f && !m_bIsAttack)
		{
			vector2 moveDir = dir.normalize();
			myPos += moveDir * m_sStat.moveSpeed * DELTA_TIME;
			m_pGameObject->getTransform().SetPosition(myPos);

			m_pAnim->SetAnimation(L"Run_" + to_wstring(m_iDirection));
		}
		else
		{
			m_pAnim->SetAnimation(L"Attack_" + to_wstring(m_iDirection));
			m_bIsAttack = true;
			m_fAttackLockTime = 0.8f;
			m_sStat.moveSpeed = CRandom::GetInstance().Range(5.f, 5.5f);
		}
	}
}

void CMonster_Biter::FixedUpdate()
{
}

void CMonster_Biter::OnEnable()
{
	OnEnable_();
}

void CMonster_Biter::OnDisable()
{
}

void CMonster_Biter::Render(HDC _hdc)
{
}

void CMonster_Biter::OnCollisionEnter(CCollider* _other)
{
}

void CMonster_Biter::OnCollisionStay(CCollider* _other)
{
}

void CMonster_Biter::OnCollisionExit(CCollider* _other)
{
}

void CMonster_Biter::OnMouseEnter()
{
}

void CMonster_Biter::OnMouseOver()
{
}

void CMonster_Biter::OnMouseExit()
{
}

void CMonster_Biter::OnDead()
{
	m_pAnim->SetAnimation(L"Death_" + to_wstring(0));
	m_pSoundObj->SetClip(L"Monster_Biter_Death");
	m_pSoundObj->Play();
}
