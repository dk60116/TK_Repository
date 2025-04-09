#include "CTransform.h"
#include "CDebug.h"

CTransform::CTransform()
	: m_pParent(nullptr)
	, m_v3Position(vector3::zero())
	, m_v3Scale(vector3::one())
	, m_v3EulerAngles(vector3::zero())
	, m_matWorld()
	, m_sDirections({})
{
}

CTransform::~CTransform()
{
}

void CTransform::Awake()
{
	CComponent::Awake();

	D3DXMatrixIdentity(&m_matWorld);
}

void CTransform::Start()
{
}

void CTransform::Update()
{
	CComponent::Update();

	UpdateWorld();
	UpdateDirections();
}

void CTransform::FixedUpdate()
{
}

void CTransform::LateUpdate()
{
}

void CTransform::Render()
{
}

void CTransform::OnEnable()
{
}

void CTransform::OnDisable()
{
}

void CTransform::OnDestroy()
{
}

void CTransform::UpdateWorld()
{
	D3DXMATRIX matScale;
	D3DXMatrixScaling(&matScale, m_v3Scale.x, m_v3Scale.y, m_v3Scale.z);

	D3DXMATRIX matRotation;
	D3DXMatrixRotationYawPitchRoll
	(
		&matRotation,
		D3DXToRadian(m_v3EulerAngles.y),
		D3DXToRadian(m_v3EulerAngles.x),
		D3DXToRadian(m_v3EulerAngles.z)
	);

	D3DXMATRIX matTranslation;
	D3DXMatrixTranslation(&matTranslation, m_v3Position.x, m_v3Position.y, m_v3Position.z);

	m_matWorld = matScale * matRotation * matTranslation;

	if (m_pParent)
	{
		m_matWorld *= m_pParent->m_matWorld;
	}
}

void CTransform::UpdateDirections()
{
	D3DXMATRIX rotMatrix;

	D3DXMatrixRotationYawPitchRoll
	(
		&rotMatrix,
		D3DXToRadian(m_v3EulerAngles.y),
		D3DXToRadian(m_v3EulerAngles.x),
		D3DXToRadian(m_v3EulerAngles.z)
	);

	const D3DXVECTOR3 forward(0, 0, 1);
	const D3DXVECTOR3 up(0, 1, 0);
	const D3DXVECTOR3 right(1, 0, 0);

	D3DXVECTOR3 n_fwd;
	D3DXVECTOR3 n_up;
	D3DXVECTOR3 n_right;

	D3DXVec3TransformNormal(&n_fwd, &forward, &rotMatrix);
	D3DXVec3TransformNormal(&n_up, &up, &rotMatrix);
	D3DXVec3TransformNormal(&n_right, &right, &rotMatrix);

	m_sDirections.forward = vector3(n_fwd.x, n_fwd.y, n_fwd.z);
	m_sDirections.up = vector3(n_up.x, n_up.y, n_up.z);
	m_sDirections.right = vector3(n_right.x, n_right.y, n_right.z);

	m_sDirections.back = -m_sDirections.forward;
	m_sDirections.down = -m_sDirections.up;
	m_sDirections.left = -m_sDirections.right;
}

void CTransform::SetPosition(const vector3 _pos)
{
	m_v3Position = _pos;
}

void CTransform::SetPosition(const _float _x, const _float _y, const _float _z)
{
	m_v3Position = vector3(_x, _y, _z);
}

void CTransform::SetScale(const vector3 _scale)
{
	m_v3Scale = _scale;
}

void CTransform::SetScale(const _float _x, const _float _y, const _float _z)
{
	m_v3Scale = vector3(_x, _y, _z);
}

void CTransform::AddPosition(const vector3 _pos)
{
	m_v3Position += _pos;
}

void CTransform::SetRotation(const vector3 _rot)
{
	m_v3EulerAngles = _rot;
}

void CTransform::SetRotation(const _float _x, const _float _y, const _float _z)
{
	m_v3EulerAngles = vector3(_x, _y, _z);
}

void CTransform::AddRotation(const vector3 _rot)
{
	m_v3EulerAngles += _rot;
}
