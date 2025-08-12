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

    return S_OK;
}

void CRigidBody::Awake()
{
    if (auto col = m_pGameObject->GetComponent<CCollider>())
    {
        m_pCollider = col;
        m_pCollider->AddRef();
    }
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
