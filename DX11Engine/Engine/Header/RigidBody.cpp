#include "epch.h"
#include "RigidBody.h"

CRigidBody::CRigidBody()
{
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
        {
            m_pCollider->AddRef();
            m_pCollider->Set_RigidBody(this);
        }
    }

    return S_OK;
}

void CRigidBody::Awake()
{
}

void CRigidBody::Update()
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
