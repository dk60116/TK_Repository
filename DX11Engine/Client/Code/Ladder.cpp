#include "cpch.h"
#include "Ladder.h"

CLadder::CLadder()
    : m_pBodyCollider(nullptr)
{
}

CLadder::~CLadder()
{
}

CLadder* CLadder::Create()
{
    return new CLadder();
}

CComponent* CLadder::Clone() const
{
    CLadder* clone = new CLadder();

    return clone;
}

HRESULT CLadder::Initialize()
{
    m_strObjName = L"Ladder";

    m_sDescription.scaleFactor = 0.0175f;
    m_sDescription.isTrigger = true;

    if (FAILED(__super::Initialize()))
        return S_OK;

    return S_OK;
}

void CLadder::Awake()
{
    __super::Awake();

    m_pBodyCollider = Get_Transform()->Get_Child()->Get_GameObject()->AddComponent<CBoxCollider>();
    m_pBodyCollider->Get_GameObject()->SetLayer(L"Map");
    m_pBodyCollider->Set_Center(vector3::up() * 2.25f * (1.f / m_sDescription.scaleFactor));
    m_pBodyCollider->Set_Size(vector3(0.2f, 4.5f, 1.f) * (1.f / m_sDescription.scaleFactor));
}

void CLadder::Start()
{
    __super::Start();
}

void CLadder::Update()
{
    __super::Update();
}

void CLadder::OnTriggerEnter(CCollider* _other)
{
    __super::OnTriggerEnter(_other);

    if (_other->Get_GameObject()->CompareTag(L"Player"))
    {
        CGameManager::GetInstance().Get_Player()->Get_Controller()->ChangeState(CPlayerController::Ladder, false, this);
    }
}

void CLadder::OnDestroy()
{
    __super::OnDestroy();
}
