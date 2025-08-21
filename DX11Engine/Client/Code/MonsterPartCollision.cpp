#include "cpch.h"
#include "MonsterPartCollision.h"
#include "Monster.h"
#include "Weapon.h"

CMonsterPartCollision::CMonsterPartCollision()
    : m_pMonster(nullptr)
    , m_pCollider(nullptr)
{
}

CMonsterPartCollision::~CMonsterPartCollision()
{
}

CMonsterPartCollision* CMonsterPartCollision::Create()
{
    return new CMonsterPartCollision();
}

CMonsterPartCollision* CMonsterPartCollision::Clone() const
{
    CMonsterPartCollision* clone = new CMonsterPartCollision();

    return clone;
}

HRESULT CMonsterPartCollision::Initialize()
{
    if (FAILED(__super::Initialize()))
        return E_FAIL;

    m_pGameObject->SetLayer(CSceneManager::NameToLayer(L"MonsterBodyPart"));

    if (!m_pCollider)
    {
        m_pCollider = m_pGameObject->AddComponent<CBoxCollider>();
        m_pCollider->SetTrigger(true);
        m_pCollider->Set_Size(vector3::one() * 100.f);
    }

    return S_OK;
}

void CMonsterPartCollision::Awake()
{
}

void CMonsterPartCollision::OnTriggerEnter(CCollider* _other)
{
    if (_other->Get_GameObject()->GetLayer() == CSceneManager::NameToLayer(L"PlayerWeapon"))
    {
        CWeapon* wp = _other->Get_GameObject()->GetComponent<CWeapon>();

        if (!m_pMonster->Get_Controller()->IsDamaged())
        {
            m_pMonster->Get_Damage(wp);
        }
    }
}

void CMonsterPartCollision::OnTriggerExit(CCollider* _other)
{
}

void CMonsterPartCollision::OnDestroy()
{
    Safe_Release(m_pMonster);
}

void CMonsterPartCollision::Set_Monster(CMonster* _monster)
{
    if (_monster == m_pMonster)
        return;

    Safe_Release(m_pMonster);

    m_pMonster = _monster;

    if (m_pMonster)
        m_pMonster->AddRef();
}

void CMonsterPartCollision::SetCenter(const vector3& _center)
{
    m_pCollider->Set_Center(_center * 100.f);
}

void CMonsterPartCollision::SetSize(const vector3& _size)
{
    m_pCollider->Set_Size(_size * 100.f);
}

CBoxCollider* CMonsterPartCollision::Get_Collider()
{
    return m_pCollider;
}
