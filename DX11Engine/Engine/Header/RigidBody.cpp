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
    CTransform* myTf = Get_Transform();

    myTf->Add_Position(m_vVelocity * DELTA_TIME);

    if (m_bUseGravity)
    {
        m_vVelocity += GRAVITY * DELTA_TIME;
    }

    m_vVelocity = vector3::Lerp(m_vVelocity, vector3::zero(), DELTA_TIME);
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
    m_vVelocity.y = 0.f;
}
