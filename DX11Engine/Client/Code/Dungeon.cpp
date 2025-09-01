#include "cpch.h"
#include "Dungeon.h"
#include "DungeonChapter.h"

#include "Wolf.h"
#include "Goblin.h"
#include "Troll.h"

#include "DungeonGate.h"
#include "FootSwitch.h"

CDungeon::CDungeon()
    : m_mMonsterProtoList({})
    , m_mDungonObjProtoList({})
    , m_vChapterList({})
    , m_bAttachedChapterColliders(false)
    , m_vGateList({})
{
}

CDungeon::~CDungeon()
{
}

CDungeon* CDungeon::Create()
{
    return new CDungeon();
}

CComponent* CDungeon::Clone() const
{
    CDungeon* clone = new CDungeon();

    return clone;
}

HRESULT CDungeon::Initialize()
{
    m_strMapName = L"Dungeon";
    m_sOptions.scaleFactor = 0.015f;

    if (FAILED(__super::Initialize()))
        return E_FAIL;

    SpawnMonsterPrototypes();
    SpawnDungonObjectPrototypes();
    SpawnDungeonChapters();

    return S_OK;
}

void CDungeon::Awake()
{
    __super::Awake();

    CCollisionManager::Set_CollisionFilter(L"DungeonChapter", L"Map", true);
}

void CDungeon::Start()
{
    __super::Start();

    CreateDungeonGates();
}

void CDungeon::Update()
{
    __super::Update();

    if (m_pGameObject->Get_Scene()->Get_PassedTime() > 0.1f)
    {
        for (TRAVERSAL_ITER(m_vChapterList, it))
            (*it)->Get_GameObject()->SetActive(false);
    }

    if (m_pGameObject->Get_Scene()->Get_PassedTime() > 0.2f)
    {
        for (TRAVERSAL_ITER(m_vChapterList, it))
            (*it)->Get_GameObject()->SetActive(true);
    }

    if (CInput::GetKeyDown_Editor(G))
    {
        CGameObject* gate = Get_Transform()->Find_ChildRecursive(L"door_1")->Get_GameObject();
        gate->SetActive(!gate->ActiveSelf());
    }
}

void CDungeon::LateUpdate()
{
    if (!m_bAttachedChapterColliders)
    {
        for (TRAVERSAL_ITER(m_vChapterList, it))
            (*it)->AttachColliders();

        CCollisionManager::Set_CollisionFilter(L"DungeonChapter", L"Map", false);

        m_bAttachedChapterColliders = true;
    }
}

void CDungeon::OnDestroy()
{
    __super::OnDestroy();
}

void CDungeon::SpawnMonsterPrototypes()
{
    CrateMonsterPrototype<CWolf>();
    CrateMonsterPrototype<CGoblin>();
    CrateMonsterPrototype<CTroll>();
}

void CDungeon::SpawnDungonObjectPrototypes()
{
    CreateDungonObjectPrototype<CDungeonGate>();
    CreateDungonObjectPrototype<CFootSwitch>();
}

void CDungeon::SpawnDungeonChapters()
{
    for (_uint i = 0; i < 6; ++i)
    {
        CGameObject* msObj = m_pGameObject->Get_Scene()->Add_GameObject(L"DungeonChapter_" + to_wstring(i));
        m_vChapterList.push_back(msObj->AddComponent<CDungeonChapter>());
    }

    {
        m_vChapterList[0]->SetBoundingBox({ pair(vector3(0.f, 5.f, -37.f), vector3(54.f, 10.f, 82.f)) });
        vector<vector3> wolfPos = { {18.f, 3.f, -90.f}, { 18.f, 3.f, -91.f}, { 18.f, 3.f, -89.f} };
        vector<_float> wolfRot = { -90.f, -90.f, -90.f };
        m_vChapterList[1]->AddMonsterSpawner({ m_mMonsterProtoList[L"Wolf"], wolfPos, wolfRot });
    }

    {
        m_vChapterList[1]->SetBoundingBox({ pair(vector3(-3.45f, 5.f, -93.45f), vector3(70.f, 10.f, 34.f)) });
    }

    {
        m_vChapterList[2]->SetBoundingBox({ pair(vector3(-63.7f, 1.2f, -19.2f), vector3(75.f, 10.f, 34.f)) });
    }

    {
        m_vChapterList[3]->SetBoundingBox({ pair(vector3(48.56f, 5.f, -21.72f), vector3(44.86f, 10.f, 22.83f)) });
    }

    {
        m_vChapterList[4]->SetBoundingBox({ pair(vector3(40.88f, 5.f, -71.4f), vector3(17.57f, 15.f, 78.1f)) });
    }

    {
        m_vChapterList[5]->SetBoundingBox({ pair(vector3(63.6f, 5.f, -71.4f), vector3(28.0f, 15.f, 78.1f)) });
    }
}

void CDungeon::CreateDungeonGates()
{
    for (size_t i = 0; i < 4; i++)
    {
        CGameObject* newObj = CGameObject::Instantiate(m_mDungonObjProtoList[L"Dungeon_Gate"]->Get_GameObject());
        newObj->Set_ObjectName(L"Gate (Clone) " + to_wstring(i));
        m_vGateList.push_back(newObj->GetComponent<CDungeonGate>());
        m_vGateList.back()->Get_GameObject()->SetActive(true);
    }

    m_vGateList[0]->Get_Transform()->Set_Position(-7.5f, 0.f, 3.8f);
    m_vGateList[1]->Get_Transform()->Set_Position(0.f, 0.f, 3.8f);
    m_vGateList[2]->Get_Transform()->Set_Position(7.5f, 0.f, 3.8f);

    m_vGateList[3]->Get_Transform()->Set_Position(26.27f, 0.f, -22.56f);
    m_vGateList[3]->Get_Transform()->Set_EulerAnglesY(90.f);
}
