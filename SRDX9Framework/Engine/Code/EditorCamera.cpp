#include "EditorCamera.h"
#include "ETime.h"
#include "Input.h"
#include "GameObject.h"
#include "CTransform.h"
#include "EngineEditor.h"
#include "CDebug.h"

CEditorCamera::CEditorCamera()
	: m_sOptions({})
	, m_bRMouseDowned(false)
	, m_fPitch(0.f)
	, m_fYaw(0.f)
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
	float dt = CTime::GetInstance().Get_DeltaTime();

	__super::UpdateEditor();

	const _bool isShift = CInput::GetInstance().GetKey_Editor(SHIFT);
	const _bool isCtrl = CInput::GetInstance().GetKey_Editor(CONTROL);

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
		if (isShift)
			m_sOptions.crtMoveSpeed = m_sOptions.moveSpeed * 2.5f;
		else
			m_sOptions.crtMoveSpeed = m_sOptions.moveSpeed;

		CTransform& camTransform = getTransform();

		const bool isRightMouseDown = CInput::GetInstance().GetMouseButton_Editor(1);
		const bool isMiddleMouseDown = CInput::GetInstance().GetMouseButton_Editor(2);

		if (isRightMouseDown || isMiddleMouseDown)
		{
			vector2Int currentMouse = CInput::GetInstance().GetMousePos();

			if (!m_bRMouseDowned)
			{
				m_v2PrevMosuePos = CInput::GetInstance().GetMousePos();
				m_bRMouseDowned = true;
				return;
			}

			_float horizontal = CInput::GetInstance().GetAxis_Editor(L"Horizontal");
			_float vertical = CInput::GetInstance().GetAxis_Editor(L"Vertical");

			if (!isCtrl)
			{
				camTransform.AddLocalPosition(camTransform.getDirections().right * horizontal * dt * m_sOptions.crtMoveSpeed);
				camTransform.AddLocalPosition(camTransform.getDirections().forward * vertical * dt * m_sOptions.crtMoveSpeed);
				
				if (CInput::GetInstance().GetKey_Editor(Q))
					camTransform.AddLocalPosition(camTransform.getDirections().down * dt * m_sOptions.crtMoveSpeed);
				if (CInput::GetInstance().GetKey_Editor(E))
					camTransform.AddLocalPosition(camTransform.getDirections().up * dt * m_sOptions.crtMoveSpeed);

				m_v2MouseDragDelta = (currentMouse - m_v2PrevMosuePos).to_vector2();
				m_v2PrevMosuePos = currentMouse;

				if (isRightMouseDown)
				{
					if (m_v2MouseDragDelta != vector2::zero())
					{
						m_fYaw += m_v2MouseDragDelta.x * dt * m_sOptions.rotateSpeed;
						m_fPitch += m_v2MouseDragDelta.y * dt * m_sOptions.rotateSpeed;

						camTransform.SetLocalEulerAngles(m_fPitch, m_fYaw, 0.f);
					}
				}
			}

			if (isShift)
				m_sOptions.crtDragSpeed = m_sOptions.dragSpeed * 2.f;
			else
				m_sOptions.crtDragSpeed = m_sOptions.dragSpeed;

			if (isMiddleMouseDown)
			{
				getTransform().AddPosition(getTransform().getDirections().left * m_v2MouseDragDelta.x * dt * m_sOptions.crtDragSpeed);
				getTransform().AddPosition(getTransform().getDirections().up * m_v2MouseDragDelta.y * dt * m_sOptions.crtDragSpeed);
			}
		}
		else
		{
			m_bRMouseDowned = false;
			m_v2MouseDragDelta = vector2::zero();
		}

		const float _wheel = CInput::GetInstance().GetAxis_Editor(L"Mouse ScrollWheel");

		if (_wheel != 0)
		{
			if (!isShift)
				getTransform().AddPosition(getTransform().getDirections().forward * _wheel * dt * m_sOptions.zoomSpeed);
		}
	}
}

void CEditorCamera::GotoViewGameObject(CGameObject* _gameObject)
{
	float dist = (m_bZoomDistToggle && CEngineEditor::GetInstance().getSelectedGameObject() == _gameObject) ? 8.f : 3.f;
	vector3 targetPos = _gameObject->getTransform().getPosition() + getTransform().getDirections().back * dist;

	m_v3MoveFrom = getTransform().getPosition();
	m_v3MoveTo = targetPos;
	m_fMoveTimeCur = 0.f;
	m_bMoving = true;

	m_bZoomDistToggle = (CEngineEditor::GetInstance().getSelectedGameObject() == _gameObject) ? !m_bZoomDistToggle : true;
}
