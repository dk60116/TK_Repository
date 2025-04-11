#include "CCollider_Rect.h"
#include "CSceneManager.h"
#include "CPhysics.h"

CCollider_Rect::CCollider_Rect(const vector2 _size, const vector2 _pivot)
{
	m_eShape = RECT;
	m_v2Size = _size;
	m_v2Pivot = _pivot;
}

CCollider_Rect::~CCollider_Rect()
{
}

void CCollider_Rect::Awake()
{
	Awake_();
}

void CCollider_Rect::Start()
{
	Start_();
}

void CCollider_Rect::Reset()
{
	Reset_();
}

void CCollider_Rect::Update()
{
	Update_();
}

void CCollider_Rect::FixedUpdate()
{
	FixedUpdate_();
}

void CCollider_Rect::LateUpdate()
{
	LateUpdate_();
}

void CCollider_Rect::OnEnable()
{
	OnEnable_();
}

void CCollider_Rect::OnDisable()
{
	OnDisable_();
}

void CCollider_Rect::Render(HDC _hDC)
{
	return;

	vector2 camPos = CSceneManager::GetInstance().getCurrentscene()->getMainCamera()->getMove();
	float size = CSceneManager::GetInstance().getCurrentscene()->getMainCamera()->getSize();

	vector2 myScale = m_v2Size;
	vector2 myPos = CSceneManager::GetInstance().getCurrentscene()->getMainCamera()->getTransform().getPosition() - getTransform().getPosition() - m_v2Pivot;
	myPos *= ((size)*UNIT);

	DrawTPRectangle(_hDC,
		vector2::zero() - (camPos + myPos) - vector2::one(),
		vector2::one() * myScale * UNIT * size + (vector2::one() * 2),
		&getTransform(),
		m_sColor);
}

void CCollider_Rect::OnCollisionEnter(CCollider* _other)
{
}

void CCollider_Rect::OnCollisionStay(CCollider* _other)
{
}

void CCollider_Rect::OnCollisionExit(CCollider* _other)
{
}

void CCollider_Rect::OnDestroy()
{
	OnDestroy_();

	CPhysics::GetInstance().DeleteCollider(this, m_pGameObject->getLayer());

	m_lEnteredColliderList.clear();
}

void CCollider_Rect::OnMouseEnter()
{
}

void CCollider_Rect::OnMouseOver()
{
}

void CCollider_Rect::OnMouseExit()
{
}
