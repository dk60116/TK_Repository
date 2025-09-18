#include "cpch.h"
#include "Ladder.h"

CLadder::CLadder()
    : m_pBodyCollider(nullptr)
    , m_fBottomHeight(0.f)
    , m_fTopHeight(1.f)
    , m_fUpPlatHeight(1.f)
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

HRESULT CLadder::Initialize(void* _desc)
{
    m_strObjName = L"Ladder";

    m_sDescription.scaleFactor = 0.0175f;
    m_sDescription.isTrigger = true;
    m_sDescription.colliderSize = vector3(0.8f, 1.f, 0.3f);

    if (FAILED(__super::Initialize(_desc)))
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
        if (CInput::GetKey(W))
            CGameManager::GetInstance().Get_Player()->Get_Controller()->ChangeState(CPlayerController::Ladder, false, this);
    }
}

const vector3 CLadder::Get_HeightValues() const
{
    return vector3(m_fBottomHeight, m_fTopHeight, m_fUpPlatHeight);
}

void CLadder::Set_HeightValue(const _float _bottom, const _float _top, const _float _upPlat)
{
    m_fBottomHeight = _bottom;
    m_fTopHeight = _top;
    m_fUpPlatHeight = _upPlat;
}

void CLadder::OnDestroy()
{
    __super::OnDestroy();
}
