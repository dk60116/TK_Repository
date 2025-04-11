#include "CStoneFurnace.h"
#include "CGameObject.h"
#include "CSceneManager.h"
#include "CInOutStructure.h"

CStoneFurnace::CStoneFurnace()
	: m_pInOut(nullptr)
	, m_pAnimator(nullptr)
	, m_pUpFireObj(nullptr)
	, m_pFireAnimObj(nullptr)
{
	m_pItem = CItemManager::GetInstance().GetItem(4);
}

CStoneFurnace::~CStoneFurnace()
{
	OnDestroy_();
}

void CStoneFurnace::Awake()
{
	Awake_();
}

void CStoneFurnace::Start()
{
	Start_();

	StructureStart();

	m_pInOut = dynamic_cast<CInOutStructure*>(m_pGameObject->AddComponent(new CInOutStructure()));
	m_pInOut->SetStructure(this);
	m_pInOut->getOption().taskSpeed = 5.f;
	m_pInOut->getOption().fuelSpeed = 2.f;
	m_pInOut->AddIOItemPair(vector2Int(1, 1), vector2Int(6, 1));
	m_pInOut->AddIOItemPair(vector2Int(2, 1), vector2Int(7, 1));
	m_pInOut->AddIOItemPair(vector2Int(3, 1), vector2Int(8, 1));

	CGameObject* shadow = m_pGameObject->getScene()->AddObject(L"StoneFurnace_Shadow", Layer::Shadow);
	shadow->AddComponent(new CSpriteRenderer(L"StoneFurnace_Shadow", vector2Int(82, 37), true));
	shadow->getTransform().SetChildPosition(vector2(1.25f, 0.4f));
	shadow->getTransform().SetScale(vector2(1.5f, 0.75f));
	shadow->GetComponent<CSpriteRenderer>()->SetAlpha(0.4f);
	shadow->SetParent(m_pGameObject);

	m_pUpFireObj = m_pGameObject->getScene()->AddObject(L"CStoneFurnace_UpFire", Layer::Structure);
	m_pUpFireObj->AddComponent(new CSpriteRenderer(L"StoneFurnace_Fire_Up", vector2Int(27, 18)));
	m_pUpFireObj->getTransform().SetChildPosition(vector2::up() * 0.8f);
	m_pUpFireObj->getTransform().SetScale(vector2(0.3f, 0.28f));
	m_pUpFireObj->SetParent(m_pGameObject);

	m_pFireAnimObj = m_pGameObject->getScene()->AddObject(L"CStoneFurnace_FireAnim", Layer::Structure);
	m_pAnimator = dynamic_cast<CAnimator*>(m_pFireAnimObj->AddComponent(new CAnimator()));
	m_pFireAnimObj->AddComponent(new CSpriteRenderer(m_pAnimator));
	m_pFireAnimObj->SetParent(m_pGameObject);
	m_pFireAnimObj->getTransform().SetChildPosition(vector2::down() * 0.5f);
	m_pFireAnimObj->getTransform().SetScale(vector2(0.3f, 0.4f));

	m_pAnimator->CreateAnimation(L"Fireing", L"StoneFurnace_Fire", 38, vector2::zero(), vector2Int(20, 25), 0);
	m_pAnimator->SetAnimation(L"Fireing");
}

void CStoneFurnace::Reset()
{
	Reset_();
}

void CStoneFurnace::Update()
{
	Update_();

	m_pUpFireObj->SetActive(m_pInOut->getTasking());
	m_pFireAnimObj->SetActive(m_pInOut->getTasking());
}

void CStoneFurnace::FixedUpdate()
{
	FixedUpdate_();
}

void CStoneFurnace::LateUpdate()
{
	LateUpdate_();
}

void CStoneFurnace::OnEnable()
{
	OnEnable_();
}

void CStoneFurnace::OnDisable()
{
	OnDisable_();
}

void CStoneFurnace::Render(HDC _hDC)
{
}

void CStoneFurnace::OnCollisionEnter(CCollider* _other)
{
	OnCollisionEnter_(_other);
}

void CStoneFurnace::OnCollisionStay(CCollider* _other)
{
	OnCollisionStay_(_other);
}

void CStoneFurnace::OnCollisionExit(CCollider* _other)
{
	OnCollisionExit_(_other);
}

void CStoneFurnace::OnDestroy()
{
	OnDestroy_();
}

void CStoneFurnace::OnMouseEnter()
{
}

void CStoneFurnace::OnMouseOver()
{
}

void CStoneFurnace::OnMouseExit()
{
}

void CStoneFurnace::OnOutputHandler()
{
}
