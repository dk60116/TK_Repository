#include "epch.h"
#include "RigidBody.h"

CRigidBody::CRigidBody()
    : m_pCollider(nullptr)
    , m_sOptions({})
    , m_sFreezePosition({})
    , m_sFreezeRotation({})
    , m_bUseGravity(false)
    , m_bIsKinematic(false)
    , m_vVelocity({})
    , m_vGravityAcceleration({})
{
    m_strName = L"RigidBody";
}

CRigidBody::~CRigidBody()
{
}

CRigidBody* CRigidBody::Create()
{
    return new CRigidBody();
}

CComponent* CRigidBody::Clone() const
{
    CRigidBody* clone = new CRigidBody();

    clone->m_sOptions = this->m_sOptions;
    clone->m_sFreezePosition = this->m_sFreezePosition;
    clone->m_sFreezeRotation = this->m_sFreezeRotation;
    clone->m_bUseGravity = this->m_bUseGravity;
    clone->m_bIsKinematic = this->m_bIsKinematic;
    clone->m_vVelocity = this->m_vVelocity;

    return clone;
}

HRESULT CRigidBody::Initialize()
{
    if (FAILED(__super::Initialize()))
        return E_FAIL;

    if (CCollider* col = m_pGameObject->GetComponent<CCollider>())
    {
        m_pCollider = col;

        if (m_pCollider)
            m_pCollider->Set_RigidBody(this);

        if (m_pCollider)
            m_pCollider->AddRef();
    }

    return S_OK;
}

void CRigidBody::Awake()
{
}

void CRigidBody::Update()
{
    if (m_bUseGravity)
    {
        m_vGravityAcceleration += GRAVITY * DELTA_TIME;
    }

    CTransform* myTf = Get_Transform();

    myTf->Add_Position(m_vGravityAcceleration * DELTA_TIME);;
    myTf->Add_Position(m_vVelocity * DELTA_TIME);

    m_vVelocity = vector3::Lerp(m_vVelocity, vector3::zero(), DELTA_TIME * (1.f / m_sOptions.drag));
}

void CRigidBody::LateUpdate()
{
}

void CRigidBody::Render_Editor()
{
}

void CRigidBody::Render()
{
}

void CRigidBody::OnDestroy()
{
    Safe_Release(m_pCollider);
}

const _bool CRigidBody::UseGravity() const
{
    return m_bUseGravity;
}

void CRigidBody::SetUseGravity(const _bool _value)
{
    m_bUseGravity = _value;
}

const _bool CRigidBody::IsKinematic() const
{
    return m_bIsKinematic;
}

void CRigidBody::SetKinematic(const _bool _value)
{
    m_bIsKinematic = _value;
}

const vector3& CRigidBody::GetVelocity()
{
    return m_vVelocity;
}

const vector3 CRigidBody::GetVelocityDirection()
{
    vector3 result = ((m_vVelocity + m_vGravityAcceleration) - Get_Transform()->Get_Position()).normalized();

    return result;
}

const vector3& CRigidBody::Get_GA()
{
    return m_vGravityAcceleration;
}

void CRigidBody::AddForce(const vector3 _value)
{
    m_vVelocity += _value;
}

void CRigidBody::ResetVelocity()
{
    m_vVelocity = vector3::zero();
}

void CRigidBody::ResetGravity()
{
    m_vGravityAcceleration = vector3::zero();
}

void CRigidBody::SetVelocity(const vector3 _value)
{
    m_vVelocity = _value;
}

void CRigidBody::SetVelocitX(const _float _value)
{
    m_vVelocity.x = _value;
}

void CRigidBody::SetVelocitY(const _float _value)
{
    m_vVelocity.y = _value;
}

void CRigidBody::SetVelocitZ(const _float _value)
{
    m_vVelocity.z = _value;
}

void CRigidBody::AddVelocity(const vector3 _value)
{
    m_vVelocity += _value;
}

void CRigidBody::AddVelocitX(const _float _value)
{
    m_vVelocity.x += _value;
}

void CRigidBody::AddVelocitY(const _float _value)
{
    m_vVelocity.y += _value;
}

void CRigidBody::AddVelocitZ(const _float _value)
{
    m_vVelocity.z += _value;
}
