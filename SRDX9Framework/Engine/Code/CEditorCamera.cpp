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
	__super::UpdateEditor();

	if (CInput::GetInstance().GetKey_Editor(SHIFT))
		m_fCrtMoveSpd = m_fMoveSpeed * 2.5f;
	else
		m_fCrtMoveSpd = m_fMoveSpeed;

	float delta = CTime::GetInstance().Get_TimeDelta();
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
			camTransform.AddLocalPosition(camTransform.getDirections().forward * delta * m_fCrtMoveSpd);
		if (CInput::GetInstance().GetKey_Editor(S))
			camTransform.AddLocalPosition(camTransform.getDirections().back * delta * m_fCrtMoveSpd);
		if (CInput::GetInstance().GetKey_Editor(A))
			camTransform.AddLocalPosition(camTransform.getDirections().left * delta * m_fCrtMoveSpd);
		if (CInput::GetInstance().GetKey_Editor(D))
			camTransform.AddLocalPosition(camTransform.getDirections().right * delta * m_fCrtMoveSpd);
		if (CInput::GetInstance().GetKey_Editor(Q))
			camTransform.AddLocalPosition(camTransform.getDirections().down * delta * m_fCrtMoveSpd);
		if (CInput::GetInstance().GetKey_Editor(E))
			camTransform.AddLocalPosition(camTransform.getDirections().up * delta * m_fCrtMoveSpd);

		m_v2MouseDragDelta = (currentMouse - m_v2PrevMosuePos).to_vector2();
		m_v2PrevMosuePos = currentMouse;

		if (m_v2MouseDragDelta != vector2::zero())
		{
			m_fYaw += m_v2MouseDragDelta.x * delta * m_fRotateSpeed;
			m_fPitch += m_v2MouseDragDelta.y * delta * m_fRotateSpeed;

			camTransform.SetLocalEulerAngles(m_fPitch, m_fYaw, 0.f);
		}
	}
	else
	{
		m_bRMouseDowned = false;
		m_v2MouseDragDelta = vector2::zero();
	}
}

void CEditorCamera::GotoViewGameObject(CGameObject* _gameObject)
{
	m_pGameObject->getTransform().SetPosition(_gameObject->getTransform().getPosition());

	if (m_pZoomObject)
	{
		if (m_pZoomObject == _gameObject)
		{
			m_bZoomDistToggle = !m_bZoomDistToggle;
		}
		else
			m_bZoomDistToggle = false;
	}
	else
		m_bZoomDistToggle = false;

	_float distance = 0.f;

	if (!m_bZoomDistToggle)
		distance = 3.f;
	else
		distance = 6.f;

	m_pGameObject->getTransform().AddPosition(m_pGameObject->getTransform().getDirections().back * distance);

	m_pZoomObject = _gameObject;
}
