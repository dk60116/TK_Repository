#include "CEditorCamera.h"
#include "CTimeMgr.h"
#include "CInput.h"
#include "CTransform.h"

CEditorCamera::CEditorCamera()
	: m_fMoveSpeed(3.f)
	, m_fRotateSpeed(25.f)
	, m_bRMouseDowned(false)
	, m_fPitch(0.f)
	, m_fYaw(0.f)
{
}

CEditorCamera::~CEditorCamera()
{
}

void CEditorCamera::UpdateEditor()
{
	float delta = CTimeMgr::GetInstance().Get_TimeDelta();
	CTransform& camTransform = getTransform();

	bool isRightMouseDown = CInput::GetInstance().GetMouseButton(1);

	if (isRightMouseDown)
	{
		vector2Int currentMouse = CInput::GetInstance().GetMousePos();

		if (!m_bRMouseDowned)
		{
			m_v2PrevMosuePos = CInput::GetInstance().GetMousePos();
			m_bRMouseDowned = true;
			return;
		}

		if (CInput::GetInstance().GetKey(W))
			camTransform.AddLocalPosition(camTransform.getDirections().forward * delta * m_fMoveSpeed);
		if (CInput::GetInstance().GetKey(S))
			camTransform.AddLocalPosition(camTransform.getDirections().back * delta * m_fMoveSpeed);
		if (CInput::GetInstance().GetKey(A))
			camTransform.AddLocalPosition(camTransform.getDirections().left * delta * m_fMoveSpeed);
		if (CInput::GetInstance().GetKey(D))
			camTransform.AddLocalPosition(camTransform.getDirections().right * delta * m_fMoveSpeed);
		if (CInput::GetInstance().GetKey(Q))
			camTransform.AddLocalPosition(camTransform.getDirections().down * delta * m_fMoveSpeed);
		if (CInput::GetInstance().GetKey(E))
			camTransform.AddLocalPosition(camTransform.getDirections().up * delta * m_fMoveSpeed);

		m_v2MouseDragDelta = (currentMouse - m_v2PrevMosuePos).to_vector2();
		m_v2PrevMosuePos = currentMouse;

		if (m_v2MouseDragDelta != vector2::zero())
		{
			m_fYaw += m_v2MouseDragDelta.x * delta * m_fRotateSpeed;
			m_fPitch += m_v2MouseDragDelta.y * delta * m_fRotateSpeed;

			m_fPitch = clamp(m_fPitch, -89.f, 89.f);

			camTransform.SetLocalEulerAngles(m_fPitch, m_fYaw, 0.f);
		}
	}
	else
	{
		m_bRMouseDowned = false;
		m_v2MouseDragDelta = vector2::zero();
	}
}
