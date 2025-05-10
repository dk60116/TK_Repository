#include "CTransform.h"
#include "GameObject.h"
#include "CEngineEditor.h"
#include "CDebug.h"
#include "CHierachyWindow.h"

CTransform::CTransform()
	: m_bIsRootParent(true)
	, m_pParent(nullptr)
	, m_lChildList({})
	, m_vPosition(vector3::zero())
	, m_vScale(vector3::one())
	, m_vRotation(vector3::zero())
	, m_vQuaternion(quaternion::identity())
	, m_vEulerAngles(vector3::zero())
	, m_matWorld()
	, m_sDirections({})
{
	m_strName = L"Transform";
}

CTransform::~CTransform()
{
	OnDestroy();
}

void CTransform::Awake()
{
	__super::Awake();

	D3DXMatrixIdentity(&m_matWorld);
}

void CTransform::Start()
{
}

void CTransform::UpdateEditor()
{
	__super::UpdateEditor();

	UpdateWorld();
	UpdateDirections();
}

void CTransform::Update()
{
	__super::Update();

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
	if (!m_lChildList.empty())
	{
		for (TRAVERSAL_ITER(m_lChildList, it))
		{
			(*it)->OnDestroy();
		}
	}

	__super::OnDestroy();
}

CTransform* CTransform::Create()
{
	return new CTransform();
} 

void CTransform::UpdateWorld()
{
	_matrix matScale;
	D3DXMatrixScaling(&matScale, m_vScale.x, m_vScale.y, m_vScale.z);

	_matrix matRotation;
	D3DXQUATERNION quat = m_vQuaternion;
	D3DXMatrixRotationQuaternion(&matRotation, &quat);

	_matrix matTranslation;
	D3DXMatrixTranslation(&matTranslation, m_vPosition.x, m_vPosition.y, m_vPosition.z);

	m_matWorld = matScale * matRotation * matTranslation;

	if (m_pParent)
	{
		m_matWorld *= m_pParent->m_matWorld;
	}

	m_vWorldPos = vector3(m_matWorld._41, m_matWorld._42, m_matWorld._43);

	D3DXVECTOR3 dummyScale, dummyTranslation;
	D3DXQUATERNION worldQuat;
	D3DXMatrixDecompose(&dummyScale, &worldQuat, &dummyTranslation, &m_matWorld);

	D3DXVECTOR3 worldEuler;
	D3DXQuaternionToAxisAngle(&worldQuat, nullptr, &worldEuler.x);

	m_vWorldEulerAngles = quaternion::to_euler(worldQuat);

	if (m_vRotation != m_vWorldEulerAngles)
		m_vRotation = m_vWorldEulerAngles;
}

void CTransform::UpdateDirections()
{
	_matrix rotMatrix;
	D3DXQUATERNION quat = m_vQuaternion;
	D3DXMatrixRotationQuaternion(&rotMatrix, &quat);

	const D3DXVECTOR3 forward(0, 0, 1);
	const D3DXVECTOR3 up(0, 1, 0);
	const D3DXVECTOR3 right(1, 0, 0);

	D3DXVECTOR3 n_fwd, n_up, n_right;
	D3DXVec3TransformNormal(&n_fwd, &forward, &rotMatrix);
	D3DXVec3TransformNormal(&n_up, &up, &rotMatrix);
	D3DXVec3TransformNormal(&n_right, &right, &rotMatrix);

	m_sDirections.forward = vector3(n_fwd);
	m_sDirections.up = vector3(n_up);
	m_sDirections.right = vector3(n_right);

	m_sDirections.back = -m_sDirections.forward;
	m_sDirections.down = -m_sDirections.up;
	m_sDirections.left = -m_sDirections.right;
}

const wstring CTransform::getName()
{
	return m_pGameObject->getName();
}

void CTransform::SetParent(CTransform* _parent)
{
	UpdateWorld();

	_matrix world = m_matWorld;

	if (!_parent)
	{
		if (m_pParent)
		{
			m_pParent->m_lChildList.remove(this);
			Release();
		}

		D3DXVECTOR3 s, t;
		D3DXQUATERNION r;
		D3DXMatrixDecompose(&s, &r, &t, &world);

		m_vScale = vector3(s);
		m_vPosition = vector3(t);
		m_vQuaternion = quaternion(r);
		m_vEulerAngles = quaternion::to_euler(r);

		m_pParent = nullptr;
		m_bIsRootParent = true;
	}
	else
	{
		_matrix invParent;
		D3DXMatrixInverse(&invParent, nullptr, &_parent->getWorldMatrix());

		_matrix local = world * invParent;

		D3DXVECTOR3 s, t;
		D3DXQUATERNION r;
		D3DXMatrixDecompose(&s, &r, &t, &local);

		m_vScale = vector3(s);
		m_vPosition = vector3(t);
		m_vQuaternion = quaternion(r);
		m_vEulerAngles = quaternion::to_euler(r);

		if (m_pParent != _parent)
		{
			if (m_pParent)
			{
				m_pParent->m_lChildList.remove(this);
				Release();
			}
		}

		m_pParent = _parent;
		m_bIsRootParent = false;

		_parent->m_lChildList.push_back(this);
		AddRef();
	}

	CEngineEditor::GetInstance().getWindow<CHierachyWindow>()->BuildTree();
}

void CTransform::SetPosition(const vector3 _world_pos)
{
	if (m_pParent)
	{
		_matrix invParent;
		D3DXMatrixInverse(&invParent, nullptr, &m_pParent->getWorldMatrix());

		_vec3 world = _world_pos.dVector();
		_vec3 local;
		D3DXVec3TransformCoord(&local, &world, &invParent);

		m_vPosition = vector3(local);
	}
	else
		m_vPosition = _world_pos;
}

void CTransform::SetPosition(const _float _x, const _float _y, const _float _z)
{
	vector3 pos = vector3(_x, _y, _z);

	SetPosition(pos);
}

void CTransform::AddPosition(const vector3 _world_delta)
{
	if (m_pParent)
	{
		_matrix invParent;
		D3DXMatrixInverse(&invParent, nullptr, &m_pParent->getWorldMatrix());

		D3DXVECTOR3 deltaWorld = _world_delta.dVector();
		D3DXVECTOR3 deltaLocal;
		D3DXVec3TransformNormal(&deltaLocal, &deltaWorld, &invParent);

		m_vPosition += vector3(deltaLocal);
	}
	else
		m_vPosition += _world_delta;
}

void CTransform::AddPosition(const _float _x, const _float _y, const _float _z)
{
	vector3 pos = vector3(_x, _y, _z);

	AddPosition(pos);
}

void CTransform::SetPositionX(const _float _x)
{
	vector3 current = getPosition();
	current.x = _x;
	SetPosition(current);
}

void CTransform::SetPositionY(const _float _y)
{
	vector3 current = getPosition();
	current.y = _y;
	SetPosition(current);
}

void CTransform::SetPositionZ(const _float _z)
{
	vector3 current = getPosition();
	current.z = _z;
	SetPosition(current);
}

void CTransform::AddPositionX(const _float _x)
{
	AddPosition(vector3(_x, 0.f, 0.f));
}

void CTransform::AddPositionY(const _float _y)
{
	AddPosition(vector3(0.f, _y, 0.f));
}

void CTransform::AddPositionZ(const _float _z)
{
	AddPosition(vector3(0.f, 0.f, _z));
}

void CTransform::EditRotation(const FieldType _xyz, const _float _delta)
{
	if (_xyz == FieldType::FLOAT_RX)
	{
		m_vRotation.x = _delta;
		SetEulerAngles(m_vRotation);
	}
	else if (_xyz == FieldType::FLOAT_RY)
	{
		m_vRotation.y = _delta;
		SetEulerAngles(m_vRotation);
	}
	else if (_xyz == FieldType::FLOAT_RZ)
	{
		m_vRotation.z = _delta;
		SetEulerAngles(m_vRotation);
	}
}


void CTransform::SetEulerAngles(const vector3 _world_euler_deg)
{
	D3DXQUATERNION worldQuat = quaternion::from_euler(_world_euler_deg).dQuaternion();

	if (m_pParent)
	{
		D3DXQUATERNION parentQuat = m_pParent->m_vQuaternion.dQuaternion();

		D3DXQUATERNION invParent;
		D3DXQuaternionInverse(&invParent, &parentQuat);

		D3DXQUATERNION localQuat;
		D3DXQuaternionMultiply(&localQuat, &worldQuat, &invParent);

		m_vQuaternion = quaternion(localQuat);
		m_vEulerAngles = quaternion::to_euler(localQuat);
	}
	else
	{
		m_vQuaternion = quaternion(worldQuat);
		m_vEulerAngles = _world_euler_deg;
	}
}

void CTransform::SetEulerAngles(const _float _x, const _float _y, const _float _z)
{
	vector3 angle = vector3(_x, _y, _z);

	SetEulerAngles(angle);
}

void CTransform::AddEulerAngles(const vector3 _delta)
{
	vector3 newEuler = m_vWorldEulerAngles + _delta;
	SetEulerAngles(newEuler);
}

void CTransform::AddEulerAngles(const _float _x, const _float _y, const _float _z)
{
	vector3 angle = vector3(_x, _y, _z);
	AddEulerAngles(angle);
}

void CTransform::SetEulerAnglesX(const _float _x)
{
	vector3 newEuler = m_vWorldEulerAngles;
	newEuler.x = _x;
	SetEulerAngles(newEuler);
}

void CTransform::SetEulerAnglesY(const _float _y)
{
	vector3 newEuler = m_vWorldEulerAngles;
	newEuler.y = _y;
	SetEulerAngles(newEuler);
}

void CTransform::SetEulerAnglesZ(const _float _z)
{
	vector3 newEuler = m_vWorldEulerAngles;
	newEuler.z = _z;
	SetEulerAngles(newEuler);
}

void CTransform::AddEulerAnglesX(const _float _x)
{
	vector3 angle = vector3(_x, 0.f, 0.f);
	AddEulerAngles(angle);
}

void CTransform::AddEulerAnglesY(const _float _y)
{
	vector3 angle = vector3(0.f, _y, 0.f);
	AddEulerAngles(angle);
}

void CTransform::AddEulerAnglesZ(const _float _z)
{
	vector3 angle = vector3(0.f, 0.f, _z);
	AddEulerAngles(angle);
}

void CTransform::SetLocalPosition(const vector3& _pos)
{
	m_vPosition = _pos;
}

void CTransform::SetLocalPosition(const _float _x, const _float _y, const _float _z)
{
	m_vPosition = vector3(_x, _y, _z);
}

void CTransform::AddLocalPosition(const vector3& _pos)
{
	m_vPosition += _pos;
}

void CTransform::AddLocalPosition(const _float _x, const _float _y, const _float _z)
{
	m_vPosition += vector3(_x, _y, _z);
}

void CTransform::SetLocalPositionX(const _float _x)
{
	m_vPosition.x = _x;
}

void CTransform::SetLocalPositionY(const _float _y)
{
	m_vPosition.y = _y;
}

void CTransform::SetLocalPositionZ(const _float _z)
{
	m_vPosition.z = _z;
}

void CTransform::AddLocalPositionX(const _float _x)
{
	m_vPosition.x += _x;
}

void CTransform::AddLocalPositionY(const _float _y)
{
	m_vPosition.y += _y;
}

void CTransform::AddLocalPositionZ(const _float _z)
{
	m_vPosition.z += _z;
}

void CTransform::SetLocalScale(const vector3& _scale)
{
	m_vScale = _scale;
}

void CTransform::SetLocalScale(const _float _x, const _float _y, const _float _z)
{
	m_vScale = vector3(_x, _y, _z);
}

void CTransform::SetLocalScale(const _float _value)
{
	m_vScale = vector3::one() * _value;
}

void CTransform::AddLocalScale(const vector3& _scale)
{
	m_vScale += _scale;
}

void CTransform::AddLocalScale(const _float _x, const _float _y, const _float _z)
{
	m_vScale += vector3(_x, _y, _z);
}

void CTransform::SetLocalScaleX(const _float _x)
{
	m_vScale.x = _x;
}

void CTransform::SetLocalScaleY(const _float _y)
{
	m_vScale.y = _y;
}

void CTransform::SetLocalScaleZ(const _float _z)
{
	m_vScale.z = _z;
}

void CTransform::AddLocalScaleX(const _float _x)
{
	m_vScale.x += _x;
}

void CTransform::AddLocalScaleY(const _float _y)
{
	m_vScale.y += _y;
}

void CTransform::AddLocalScaleZ(const _float _z)
{
	m_vScale.z += _z;
}

void CTransform::SetLocalEulerAngles(const vector3& _rot)
{
	m_vEulerAngles = _rot;
	m_vQuaternion = quaternion::from_euler(m_vEulerAngles);
}

void CTransform::SetLocalEulerAngles(const _float _x, const _float _y, const _float _z)
{
	m_vEulerAngles = vector3(_x, _y, _z);
	m_vQuaternion = quaternion::from_euler(m_vEulerAngles);
}

void CTransform::SetLocalEulerAnglesX(const _float _x)
{
	m_vEulerAngles.x = _x;
	m_vQuaternion = quaternion::from_euler(m_vEulerAngles);
}

void CTransform::SetLocalEulerAnglesY(const _float _y)
{
	m_vEulerAngles.y = _y;
	m_vQuaternion = quaternion::from_euler(m_vEulerAngles);
}

void CTransform::SetLocalEulerAnglesZ(const _float _z)
{
	m_vEulerAngles.z = _z;
	m_vQuaternion = quaternion::from_euler(m_vEulerAngles);
}

void CTransform::AddLocalEulerAnglesX(const _float _x)
{
	m_vEulerAngles.x += _x;
	m_vQuaternion = quaternion::from_euler(m_vEulerAngles);
}

void CTransform::AddLocalEulerAnglesY(const _float _y)
{
	m_vEulerAngles.y += _y;
	m_vQuaternion = quaternion::from_euler(m_vEulerAngles);
}

void CTransform::AddLocalEulerAnglesZ(const _float _z)
{
	m_vEulerAngles.z += _z;
	m_vQuaternion = quaternion::from_euler(m_vEulerAngles);
}

void CTransform::SetQuaternion(const quaternion& _q)
{
	m_vQuaternion = _q;
}

void CTransform::AddLocalXAxis(_float _angle)
{
	_float angle_rad = D3DXToRadian(_angle);

	D3DXQUATERNION xRot;
	D3DXVECTOR3 euler = vector3::right().dVector();
	D3DXQuaternionRotationAxis(&xRot, &euler, angle_rad);

	m_vQuaternion = quaternion(xRot) * m_vQuaternion;
}

void CTransform::AddLocalYAxis(_float _angle)
{
	_float angle_rad = D3DXToRadian(_angle);

	D3DXQUATERNION yRot;
	D3DXVECTOR3 euler = vector3::up().dVector();
	D3DXQuaternionRotationAxis(&yRot, &euler, angle_rad);

	m_vQuaternion = quaternion(yRot) * m_vQuaternion;
}

void CTransform::AddLocalZAxis(_float _angle)
{
	_float angle_rad = D3DXToRadian(_angle);

	D3DXQUATERNION zRot;
	D3DXVECTOR3 euler = vector3::forward().dVector();
	D3DXQuaternionRotationAxis(&zRot, &euler, angle_rad);

	m_vQuaternion = quaternion(zRot) * m_vQuaternion;
}

void CTransform::RotateLocalAxis(const vector3& local_axis, _float angle_deg)
{
	_float angle_rad = D3DXToRadian(angle_deg);

	D3DXMATRIX rotMatrix;
	D3DXQUATERNION q = m_vQuaternion;
	D3DXMatrixRotationQuaternion(&rotMatrix, &q);

	D3DXVECTOR3 local = local_axis;
	D3DXVECTOR3 axis_world;
	D3DXVec3TransformNormal(&axis_world, &local, &rotMatrix);

	D3DXQUATERNION qRot;
	D3DXQuaternionRotationAxis(&qRot, &axis_world, angle_rad);

	m_vQuaternion = quaternion(qRot) * m_vQuaternion;
}

void CTransform::AddLocalEulerAngles(const vector3& _rot)
{
	m_vEulerAngles += _rot;
	m_vQuaternion = quaternion::from_euler(m_vEulerAngles);
}

void CTransform::AddLocalEulerAngles(const _float _x, const _float _y, const _float _z)
{
	m_vEulerAngles += vector3(_x, _y, _z);
	m_vQuaternion = quaternion::from_euler(m_vEulerAngles);
}

void CTransform::LookAt(const vector3 _target)
{
	if (!CEngineEditor::GetInstance().isPlaying() && !CEngineEditor::GetInstance().isNextFrame())
		return;

	vector3 dir = (_target - m_vWorldPos).normalized();

	_float pitch = asinf(-dir.y);
	_float yaw = atan2f(dir.x, dir.z);
	_float roll = 0.0f;

	vector3 euler = vector3(D3DXToDegree(pitch), D3DXToDegree(yaw), D3DXToDegree(roll));

	SetLocalEulerAngles(euler);
}

void CTransform::LookAt(CTransform& _target)
{
	if (!CEngineEditor::GetInstance().isPlaying() && !CEngineEditor::GetInstance().isNextFrame())
		return;

	LookAt(_target.getTransform().getPosition());
}

void CTransform::LookAt(const vector3 _target, const vector3 _front)
{
	if (!CEngineEditor::GetInstance().isPlaying() && !CEngineEditor::GetInstance().isNextFrame())
		return;

	vector3 world_target_dir = (_target - m_vWorldPos).normalized();

	_matrix rotMatrix;
	D3DXVECTOR3 vFrom = _front.dVector();
	D3DXVECTOR3 vTo = world_target_dir.dVector();

	D3DXQUATERNION qRot;
	D3DXVECTOR3 upVector = vector3::up();
	D3DXQuaternionRotationAxis(&qRot, &upVector, 0);

	if (D3DXVec3LengthSq(&vFrom) > 0.0001f && D3DXVec3LengthSq(&vTo) > 0.0001f)
	{
		D3DXVec3Normalize(&vFrom, &vFrom);
		D3DXVec3Normalize(&vTo, &vTo);

		D3DXVECTOR3 axis;
		D3DXVec3Cross(&axis, &vFrom, &vTo);
		_float angle = acosf(D3DXVec3Dot(&vFrom, &vTo));

		if (fabs(angle) > 0.0001f)
		{
			D3DXQuaternionRotationAxis(&qRot, &axis, angle);
		}
	}

	m_vQuaternion = quaternion(qRot);
}