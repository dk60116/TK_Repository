#include "CEditorCamera.h"
#include "CTime.h"
#include "CInput.h"
#include "CGameObject.h"
#include "CTransform.h"

CEditorCamera::CEditorCamera()
	: m_fMoveSpeed(4.f)
	, m_fRotateSpeed(25.f)
	, m_fCrtMoveSpd(0.f)
	, m_bRMouseDowned(false)
	, m_fPitch(0.f)
	, m_fYaw(0.f)
	, m_pZoomObject(nullptr)
	, m_bMoving(false)
	, m_fMoveTimeTotal(0.25f)
	, m_fMoveTimeCur(0.f)
	, m_v3MoveFrom(vector3::zero())
	, m_v3MoveTo(vector3::zero())
	, m_bZoomDistToggle(false)
{
	m_strName = L"Editor Camera";
}

CEditorCamera::~CEditorCamera()
{
	OnDestroy();
}

CEditorCamera* CEditorCamera::Create()
{
	return new CEditorCamera();
}

void CEditorCamera::AwakeEditor()
{
	vector3& angles = getTransform().getLocalEulerAngles();

	m_fPitch = angles.x;
	m_fYaw = angles.y;
}

void CEditorCamera::UpdateEditor()
{
	float dt = CTime::GetInstance().Get_TimeDelta();

	__super::UpdateEditor();

	if (m_bMoving)
	{
		m_fMoveTimeCur += dt;
		float t = m_fMoveTimeCur / m_fMoveTimeTotal;
		if (t > 1.f)
		{
			t = 1.f;
			m_bMoving = false;
		}

		float smoothT = t * t * (3.f - 2.f * t);

		D3DXVECTOR3 newPos;
		D3DXVECTOR3 moveFrom = m_v3MoveFrom.dVector();
		D3DXVECTOR3 moveTo = m_v3MoveTo.dVector();
		D3DXVec3Lerp(&newPos, &moveFrom, &moveTo, smoothT);
		getTransform().SetPosition(newPos);
	}
	else
	{
		if (CInput::GetInstance().GetKey_Editor(SHIFT))
			m_fCrtMoveSpd = m_fMoveSpeed * 2.5f;
		else
			m_fCrtMoveSpd = m_fMoveSpeed;

		CTransform& camTransform = getTransform();

		bool isRightMouseDown = CInput::GetInstance().GetMouseButton_Editor(1);

		if (isRightMouseDown)
		{
			vector2Int currentMouse = CInput::GetInstance().GetMousePos();

			if (!m_bRMouseDowned)
			{
				m_v2PrevMosuePos = CInput::GetInstance().GetMousePos();
				m_bRMouseDowned = true;
				return;
			}

			if (CInput::GetInstance().GetKey_Editor(W))
				camTransform.AddLocalPosition(camTransform.getDirections().forward * dt * m_fCrtMoveSpd);
			if (CInput::GetInstance().GetKey_Editor(S))
				camTransform.AddLocalPosition(camTransform.getDirections().back * dt * m_fCrtMoveSpd);
			if (CInput::GetInstance().GetKey_Editor(A))
				camTransform.AddLocalPosition(camTransform.getDirections().left * dt * m_fCrtMoveSpd);
			if (CInput::GetInstance().GetKey_Editor(D))
				camTransform.AddLocalPosition(camTransform.getDirections().right * dt * m_fCrtMoveSpd);
			if (CInput::GetInstance().GetKey_Editor(Q))
				camTransform.AddLocalPosition(camTransform.getDirections().down * dt * m_fCrtMoveSpd);
			if (CInput::GetInstance().GetKey_Editor(E))
				camTransform.AddLocalPosition(camTransform.getDirections().up * dt * m_fCrtMoveSpd);

			m_v2MouseDragDelta = (currentMouse - m_v2PrevMosuePos).to_vector2();
			m_v2PrevMosuePos = currentMouse;

			if (m_v2MouseDragDelta != vector2::zero())
			{
				m_fYaw += m_v2MouseDragDelta.x * dt * m_fRotateSpeed;
				m_fPitch += m_v2MouseDragDelta.y * dt * m_fRotateSpeed;

				camTransform.SetLocalEulerAngles(m_fPitch, m_fYaw, 0.f);
			}
		}
		else
		{
			m_bRMouseDowned = false;
			m_v2MouseDragDelta = vector2::zero();
		}
	}
}

void CEditorCamera::GotoViewGameObject(CGameObject* _gameObject)
{
	float dist = (m_bZoomDistToggle && m_pZoomObject == _gameObject) ? 8.f : 3.f;
	vector3 targetPos = _gameObject->getTransform().getPosition() + getTransform().getDirections().back * dist;

	m_v3MoveFrom = getTransform().getPosition();
	m_v3MoveTo = targetPos;
	m_fMoveTimeCur = 0.f;
	m_bMoving = true;

	m_bZoomDistToggle = (m_pZoomObject == _gameObject) ? !m_bZoomDistToggle : false;
	m_pZoomObject = _gameObject;
}
