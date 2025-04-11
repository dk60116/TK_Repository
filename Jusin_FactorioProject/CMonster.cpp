#include "CMonster.h"
#include "CMonsterManager.h"

CMonster::CMonster()
	: m_pRender(nullptr)
	, m_pAnim(nullptr)
	, m_pSoundObj(nullptr)
	, m_pTargetObj(nullptr)
	, m_sStat({})
	, m_bAlibe(true)
	, m_bPrevAlibe(true)
	, m_bIsAttack(false)
	, m_pHpSlot(nullptr)
	, m_fAttackLockTime(0.f)
{
}

CMonster::~CMonster()
{
	OnDestroy();
}

void CMonster::Start()
{
	CGameObject* soundObj = m_pGameObject->getScene()->AddObject(L"Player Sound", Layer::Default);
	m_pSoundObj = dynamic_cast<CAudioSource*>(soundObj->AddComponent(new CAudioSource()));

	CGameObject* hpSlotObj = m_pGameObject->getScene()->AddObject(m_pGameObject->getName() + L"_HP_UI", Layer::UI);
	m_pHpSlot = dynamic_cast<CObjHpSlot*>(hpSlotObj->AddComponent(new CObjHpSlot(m_pGameObject)));
	hpSlotObj->SetParent(m_pGameObject);
}

void CMonster::LateUpdate()
{
	LateUpdate_();

	if (!m_bAlibe && m_bPrevAlibe)
	{
		m_pGameObject->SetLayer(Layer::Dead);
		m_pAnim->getObj()->SetLayer(Layer::Dead);

		OnDead();
	}

	m_bPrevAlibe = m_bAlibe;
}

void CMonster::OnDestroy()
{
	OnDestroy_();
	
	CMonsterManager::GetInstance().DeleteMonster(this);
}

void CMonster::AddHp(float _dmg)
{
	m_sStat.crtHp -= _dmg;

	if (m_sStat.crtHp > m_sStat.maxHp)
		m_sStat.crtHp = m_sStat.maxHp;
	else if (m_sStat.crtHp < 0.f)
		m_sStat.crtHp = 0.f;

	m_pHpSlot->UpdateFill(m_sStat.crtHp / m_sStat.maxHp);
	
	if (m_sStat.crtHp <= 0.f)
		m_bAlibe = false;
}
