#include "epch.h"
#include "EditorCamera.h"

CEditorCamera::CEditorCamera()
	:CCamera{}
	, m_sOptions({})
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
}

CEditorCamera::~CEditorCamera()
{
}

CEditorCamera* CEditorCamera::Create()
{
	return new CEditorCamera();
}

void CEditorCamera::Update_Editor()
{
	if (CInput::GetInstance().GetKey(W))
		Get_Transform()->Add_PositionZ(1.f * DELTA_TIME);
	if (CInput::GetInstance().GetKey(A))
		Get_Transform()->Add_PositionX(-1.f * DELTA_TIME);
	if (CInput::GetInstance().GetKey(S))
		Get_Transform()->Add_PositionZ(-1.f * DELTA_TIME);
	if (CInput::GetInstance().GetKey(D))
		Get_Transform()->Add_PositionX(1.f * DELTA_TIME);
	if (CInput::GetInstance().GetKey(Q))
		Get_Transform()->Add_PositionY(-1.f * DELTA_TIME);
	if (CInput::GetInstance().GetKey(E))
		Get_Transform()->Add_PositionY(1.f * DELTA_TIME);

	const _bool isShift = CInput::GetInstance().GetKey(SHIFT);
	const _bool isCtrl = CInput::GetInstance().GetKey(CONTROL);

	if (m_bMoving)
	{
		m_fMoveTimeCur += DELTA_TIME;
		float t = m_fMoveTimeCur / m_fMoveTimeTotal;
		if (t > 1.f)
		{
			t = 1.f;
			m_bMoving = false;
		}

		_vector moveFrom = m_v3MoveFrom.toXMVector();
		_vector moveTo = m_v3MoveTo.toXMVector();
		float smoothT = t * t * (3.f - 2.f * t);

		_vector newPos = XMVectorLerp(moveFrom, moveTo, smoothT);
		Get_Transform()->Set_Position(vector3(newPos));
	}
	else
	{
		if (isShift)
			m_sOptions.crtMoveSpeed = m_sOptions.moveSpeed * 2.5f;
		else
			m_sOptions.crtMoveSpeed = m_sOptions.moveSpeed;

		CTransform& camTransform = *Get_Transform();

		const bool isRightMouseDown = CInput::GetInstance().GetMouseButton(1);
		const bool isMiddleMouseDown = CInput::GetInstance().GetMouseButton(2);

		if (isRightMouseDown || isMiddleMouseDown)
		{
			vector2Int currentMouse = CInput::GetInstance().GetMousePos();

			if (!m_bRMouseDowned)
			{
				m_v2PrevMosuePos = CInput::GetInstance().GetMousePos();
				m_bRMouseDowned = true;
				return;
			}

			_float horizontal = CInput::GetInstance().GetAxis(L"Horizontal");
			_float vertical = CInput::GetInstance().GetAxis(L"Vertical");

			if (!isCtrl)
			{
				camTransform.Add_Position(camTransform.Get_Directions().right * horizontal * DELTA_TIME * m_sOptions.crtMoveSpeed);
				camTransform.Add_Position(camTransform.Get_Directions().forward * vertical * DELTA_TIME * m_sOptions.crtMoveSpeed);

				if (CInput::GetInstance().GetKey(Q))
					camTransform.Add_Position(camTransform.Get_Directions().down * DELTA_TIME * m_sOptions.crtMoveSpeed);
				if (CInput::GetInstance().GetKey(E))
					camTransform.Add_Position(camTransform.Get_Directions().up * DELTA_TIME * m_sOptions.crtMoveSpeed);

				m_v2MouseDragDelta = (currentMouse - m_v2PrevMosuePos).to_vector2();
				m_v2PrevMosuePos = currentMouse;

				if (isRightMouseDown)
				{
					if (m_v2MouseDragDelta != vector2::zero())
					{
						m_fYaw += m_v2MouseDragDelta.x * DELTA_TIME * m_sOptions.rotateSpeed;
						m_fPitch += m_v2MouseDragDelta.y * DELTA_TIME * m_sOptions.rotateSpeed;

						camTransform.Set_EulerAngle(m_fPitch, m_fYaw, 0.f);
					}
				}
			}

			if (isShift)
				m_sOptions.crtDragSpeed = m_sOptions.dragSpeed * 2.f;
			else
				m_sOptions.crtDragSpeed = m_sOptions.dragSpeed;

			if (isMiddleMouseDown)
			{
				camTransform.Add_Position(camTransform.Get_Directions().left * m_v2MouseDragDelta.x * DELTA_TIME * m_sOptions.crtDragSpeed);
				camTransform.Add_Position(camTransform.Get_Directions().up * m_v2MouseDragDelta.y * DELTA_TIME * m_sOptions.crtDragSpeed);
			}
		}
		else
		{
			m_bRMouseDowned = false;
			m_v2MouseDragDelta = vector2::zero();
		}

		const float _wheel = CInput::GetInstance().GetAxis(L"Mouse ScrollWheel");

		if (_wheel != 0)
		{
			if (!isShift)
				camTransform.Add_Position(camTransform.Get_Directions().forward * _wheel * DELTA_TIME * m_sOptions.zoomSpeed);
		}
	}
}

void CEditorCamera::Update()
{
	__super::Update();
}
