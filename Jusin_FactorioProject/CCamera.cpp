#include "CCamera.h"
#include "CScreen.h"
#include "CInput.h"
#include "CGameObject.h"
#include "CSceneManager.h"
#include "CInput.h"

CCamera::CCamera()
	: m_pTarget(nullptr)
	, m_v2Offset(vector2::zero())
	, m_fSize(1.f)
{
}

CCamera::~CCamera()
{
	OnDestroy();
}

void CCamera::Awake()
{
	Awake_();
}

void CCamera::Start()
{
	Start_();
}

void CCamera::Reset()
{
	Reset_();
}

void CCamera::Update()
{
	Update_();

	if (m_pTarget && m_pTarget->isActive() && m_pTarget && m_pTarget->isEnable())
	{
		getTransform().SetPosition(m_pTarget->getTransform().getPosition() + (m_v2Offset));
	}
	else
	{
		float speed = 20.f;

		vector2 dir = vector2::zero();

		if (CInput::GetInstance().GetKey(KEY_CODE::W))
			dir.y -= 1;
		if (CInput::GetInstance().GetKey(KEY_CODE::S))
			dir.y += 1;
		if (CInput::GetInstance().GetKey(KEY_CODE::A))
			dir.x -= 1;
		if (CInput::GetInstance().GetKey(KEY_CODE::D))
			dir.x += 1;

		getTransform().SetPosition(getTransform().getPosition() + dir.normalize() * speed *DELTA_TIME);
	}
}

void CCamera::FixedUpdate()
{
	FixedUpdate_();
}

void CCamera::LateUpdate()
{
	LateUpdate_();
}

void CCamera::OnEnable()
{
	OnEnable_();
}

void CCamera::OnDisable()
{
	OnDisable_();
}

void CCamera::Render(HDC _hDC)
{
}

void CCamera::OnCollisionEnter(CCollider* _other)
{
}

void CCamera::OnCollisionStay(CCollider* _other)
{
}

void CCamera::OnCollisionExit(CCollider* _other)
{
}

void CCamera::OnDestroy()
{
	OnDestroy_();
}

void CCamera::OnMouseEnter()
{
}

void CCamera::OnMouseOver()
{
}

void CCamera::OnMouseExit()
{
}

const vector2& CCamera::getMove()
{
	vector2 result = (getTransform().getPosition()) - (CScreen::GetInstance().getScreenVector().toVector2() * 0.5f) - m_v2Offset * UNIT * 2;

	return result;
}

const vector2 CCamera::GetCameraPosToWord()
{
	return getTransform().getPosition();
}

const vector2 CCamera::GetMousePosToWorld()
{
	return (CInput::GetInstance().GetMousePos().toVector2() + getMove()) / (UNIT * m_fSize) + getTransform().getPosition() - (getTransform().getPosition() * 0.02f * (1.f / m_fSize));
}
