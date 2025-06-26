#include "epch.h"
#include "Transform.h"

CTransform::CTransform()
	: m_bIsRootParent(false)
	, m_pParent(nullptr)
	, m_lChildList({})
	, m_vPosition({})
	, m_vScale(vector3::one())
	, m_vEulerAngles({})
	, m_vLocalPosition({})
	, m_vLocalEulerAngles({})
	, m_vQuaternion(quaternion::identity())
    , m_vLocalQuaternion(quaternion::identity())
	, m_vMatWorld(XMMatrixIdentity())
    , m_vMatLocalRotation(XMMatrixIdentity())
	, m_sDirections({})
{
}

CTransform::~CTransform()
{
}

CTransform* CTransform::Create()
{
	return new CTransform();
}

HRESULT CTransform::Initialize()
{
    if (FAILED(__super::Initialize()))
        return E_FAIL;

    Bind_Matrix();
    Bind_Direction();

    return S_OK;
}

void CTransform::Update()
{
    Bind_Matrix();
    Bind_Direction();
}

const CTransform::DIRECTIONS& CTransform::Get_Directions()
{
    return m_sDirections;
}

const _matrix& CTransform::Get_WorldMatrix() const
{
    return m_vMatWorld;
}

const _matrix CTransform::Get_InverseWorldMatrix() const
{
    return XMMatrixInverse(nullptr, m_vMatWorld);
}

vector3 CTransform::Get_Position() const
{
    return m_vPosition;
}

vector3 CTransform::Get_LocalPosition() const
{
    return m_vLocalPosition;
}

const vector3& CTransform::Get_EulerAngles()
{
    m_vEulerAngles = m_vQuaternion.to_euler();

    return m_vEulerAngles;
}

const vector3& CTransform::Get_LocalEulerAngles()
{
    if (m_pParent)
    {
        _vector parentInvRot = XMQuaternionInverse(XMLoadFloat4(reinterpret_cast<const _float4*>(&m_pParent->m_vQuaternion)));
        _vector localQuat = XMQuaternionMultiply(parentInvRot, XMLoadFloat4(reinterpret_cast<const _float4*>(&m_vQuaternion)));
        XMStoreFloat4(reinterpret_cast<_float4*>(&m_vLocalQuaternion), localQuat);
    }
    else
        m_vLocalQuaternion = m_vQuaternion;

    m_vLocalEulerAngles = m_vLocalQuaternion.to_euler();

    return m_vLocalEulerAngles;
}

void CTransform::Set_Position(const vector3& _pos)
{
    m_vPosition = _pos;
}

void CTransform::Set_Position(const _float _x, const _float _y, const _float _z)
{
    m_vPosition = vector3(_x, _y, _z);
}

void CTransform::Set_PositionX(const _float _x)
{
    m_vPosition.x = _x;
}

void CTransform::Set_PositionY(const _float _y)
{
    m_vPosition.y = _y;
}

void CTransform::Set_PositionZ(const _float _z)
{
    m_vPosition.z = _z;
}

void CTransform::Set_LocalPosition(const vector3& _pos)
{
    m_vLocalPosition = _pos;
}

void CTransform::Set_LocalPosition(const _float _x, const _float _y, const _float _z)
{
    m_vLocalPosition = vector3(_x, _y, _z);

    if (m_pParent)
    {
        _vector world = XMVector3TransformCoord(m_vLocalPosition.toXMVector(), m_pParent->m_vMatWorld);
        XMStoreFloat3(reinterpret_cast<XMFLOAT3*>(&m_vPosition), world);
    }
    else
        m_vPosition = m_vLocalPosition;
}

void CTransform::Set_LocalPositionX(const _float _x)
{
    m_vLocalPosition.x = _x;

    if (m_pParent)
    {
        _vector world = XMVector3TransformCoord(m_vLocalPosition.toXMVector(), m_pParent->m_vMatWorld);
        XMStoreFloat3(reinterpret_cast<XMFLOAT3*>(&m_vPosition), world);
    }
    else
        m_vPosition = m_vLocalPosition;
}

void CTransform::Set_LocalPositionY(const _float _y)
{
    m_vLocalPosition.x = _y;

    if (m_pParent)
    {
        _vector world = XMVector3TransformCoord(m_vLocalPosition.toXMVector(), m_pParent->m_vMatWorld);
        XMStoreFloat3(reinterpret_cast<XMFLOAT3*>(&m_vPosition), world);
    }
    else
        m_vPosition = m_vLocalPosition;
}

void CTransform::Set_LocalPositionZ(const _float _z)
{
    m_vLocalPosition.x = _z;

    if (m_pParent)
    {
        _vector world = XMVector3TransformCoord(m_vLocalPosition.toXMVector(), m_pParent->m_vMatWorld);
        XMStoreFloat3(reinterpret_cast<XMFLOAT3*>(&m_vPosition), world);
    }
    else
        m_vPosition = m_vLocalPosition;
}

void CTransform::Add_Position(const vector3& _value)
{
    m_vPosition += _value;
}

void CTransform::Add_Position(const _float _x, const _float _y, const _float _z)
{
    m_vPosition += vector3(_x, _y, _z);
}

void CTransform::Add_PositionX(const _float _value)
{
    m_vPosition.x += _value;
}

void CTransform::Add_PositionY(const _float _value)
{
    m_vPosition.y += _value;
}

void CTransform::Add_PositionZ(const _float _value)
{
    m_vPosition.z += _value;
}

void CTransform::Set_EulerAngle(const vector3& _rot)
{
    m_vQuaternion = _rot.to_quaternion();
}

void CTransform::Set_EulerAngle(const _float _x, const _float _y, const _float _z)
{
    vector3 v = vector3(_x, _y, _z);
    m_vQuaternion = v.to_quaternion();
}

void CTransform::Set_EulerAngleX(const _float _x)
{
    vector3 euler = Get_EulerAngles();
    euler.x = _x;
    m_vQuaternion = euler.to_quaternion();
}

void CTransform::Set_EulerAngleY(const _float _y)
{
    vector3 euler = Get_EulerAngles();
    euler.y = _y;
    m_vQuaternion = euler.to_quaternion();
}

void CTransform::Set_EulerAngleZ(const _float _z)
{
    vector3 euler = m_vQuaternion.to_euler();
    euler.z = _z;
    m_vQuaternion = euler.to_quaternion();
}

void CTransform::Add_EulerAngle(const vector3& _rot)
{
    vector3 euler = Get_EulerAngles();
    euler += _rot;
    m_vQuaternion = euler.to_quaternion();
}

void CTransform::Add_EulerAngle(const _float _x, const _float _y, const _float _z)
{
    vector3 euler = Get_EulerAngles();

    euler += vector3(_x, _y, _z);

    m_vQuaternion = euler.to_quaternion();
}

void CTransform::Add_EulerAngleX(const _float _value)
{
    vector3 euler = Get_EulerAngles();

    euler.x += _value;

    m_vQuaternion = euler.to_quaternion();
}

void CTransform::Add_EulerAngleY(const _float _value)
{
    vector3 euler = Get_EulerAngles();

    euler.y += _value;

    m_vQuaternion = euler.to_quaternion();
}

void CTransform::Add_EulerAngleZ(const _float _value)
{
    vector3 euler = Get_EulerAngles();

    euler.z += _value;

    m_vQuaternion = euler.to_quaternion();
}

void CTransform::Set_LocalEulerAngle(const vector3& _rot)
{
}

void CTransform::Set_LocalEulerAngle(const _float _x, const _float _y, const _float _z)
{
}

void CTransform::Set_LocalEulerAngleX(const _float _x)
{
}

void CTransform::Set_LocalEulerAngleY(const _float _y)
{
}

void CTransform::Set_LocalEulerAngleZ(const _float _z)
{
}

void CTransform::Bind_Matrix()
{
    _matrix matScale = XMMatrixScaling(m_vScale.x, m_vScale.y, m_vScale.z);
    m_vMatLocalRotation = XMMatrixRotationQuaternion(XMLoadFloat4(reinterpret_cast<const _float4*>(&m_vQuaternion)));
    _matrix matTranslation = XMMatrixTranslation(m_vPosition.x, m_vPosition.y, m_vPosition.z);
    _matrix matLocal = matScale * m_vMatLocalRotation * matTranslation;

    if (m_pParent)
    {
        _matrix invParentWorld = XMMatrixInverse(nullptr, m_pParent->m_vMatWorld);

        m_vMatWorld = matLocal * m_pParent->m_vMatWorld;
        _vector v = XMVector3TransformCoord(m_vPosition.toXMVector(), invParentWorld);
        m_vLocalPosition = vector3(XMVectorGetX(v), XMVectorGetY(v), XMVectorGetZ(v));
    }
    else
    {
        m_vMatWorld = matLocal;
        m_vLocalPosition = m_vLocalPosition;
    }
}

void CTransform::Bind_Direction()
{
    XMMATRIX rotOnly = m_vMatLocalRotation;
    XMVECTOR forward = XMVector3TransformNormal(XMVectorSet(0.f, 0.f, 1.f, 0.f), rotOnly);
    XMVECTOR right = XMVector3TransformNormal(XMVectorSet(1.f, 0.f, 0.f, 0.f), rotOnly);
    XMVECTOR up = XMVector3TransformNormal(XMVectorSet(0.f, 1.f, 0.f, 0.f), rotOnly);

    XMFLOAT3 f, r, u;

    XMStoreFloat3(&f, forward);
    XMStoreFloat3(&r, right);
    XMStoreFloat3(&u, up);

    m_sDirections.forward = vector3(f.x, f.y, f.z).normalized();
    m_sDirections.back = -m_sDirections.forward;
    m_sDirections.right = vector3(r.x, r.y, r.z).normalized();
    m_sDirections.left = -m_sDirections.right;
    m_sDirections.up = vector3(u.x, u.y, u.z).normalized();
    m_sDirections.down = -m_sDirections.up;
}
