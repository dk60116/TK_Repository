#include "epch.h"
#include "Transform.h"

CTransform::CTransform()
	: m_bIsRootParent(false)
	, m_pParent(nullptr)
	, m_lChildList({})
	, m_vPosition({})
	, m_vRotation({})
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

    return S_OK;
}

void CTransform::Update()
{
    Bind_Matrix();
    Bind_Direction();
}

const CTransform::DIRECTIONS& CTransform::Get_Direction()
{
    return m_sDirections;
}

const _matrix CTransform::Get_InverseWorldMatrix()
{
    return XMMatrixInverse(nullptr, m_vMatWorld);
}

void CTransform::Bind_Matrix()
{
    _fmatrix matScale = XMMatrixScaling(m_vScale.x, m_vScale.y, m_vScale.z);
    m_vMatLocalRotation = XMMatrixRotationQuaternion(XMLoadFloat4(reinterpret_cast<const _float4*>(&m_vQuaternion)));
    _fmatrix matTranslation = XMMatrixTranslation(m_vPosition.x, m_vPosition.y, m_vPosition.z);
    _fmatrix matLocal = matScale * m_vMatLocalRotation * matTranslation;

    if (m_pParent)
        m_vMatWorld = matLocal * m_pParent->m_vMatWorld;
    else
        m_vMatWorld = matLocal;
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
