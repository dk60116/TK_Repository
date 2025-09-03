#include "cpch.h"
#include "Dungeon.h"
#include "DungeonChapter.h"

#include "Wolf.h"
#include "Goblin.h"
#include "Troll.h"

#include "DungeonGate.h"
#include "FootSwitch.h"
#include "DungeonChest.h"
#include "Ladder.h"

CDungeon::CDungeon()
    : m_mMonsterProtoList({})
    , m_mDungonObjProtoList({})
    , m_vChapterList({})
    , m_bAttachedChapterColliders(false)
    , m_vGateList({})
    , m_vFootSwitchList({})
    , m_vChestList({})
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
    CreateDungeonFootSwitches();
    CreateDungeonChest();
    CreateDungeonLadder();

    Get_Transform()->Find_ChildRecursive(L"door_1")->Get_GameObject()->SetActive(false);
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
    CreateDungonObjectPrototype<CDungeonChest>();
    CreateDungonObjectPrototype<CLadder>();
}

void CDungeon::SpawnDungeonChapters()
{
    for (_uint i = 0; i < 11; ++i)
    {
        CGameObject* msObj = m_pGameObject->Get_Scene()->Add_GameObject(L"DungeonChapter_" + to_wstring(i));
        m_vChapterList.push_back(msObj->AddComponent<CDungeonChapter>());
    }

    {
        m_vChapterList[0]->SetBoundingBox({ pair(vector3(124.5f, 5.f, -59.3f), vector3(31.f, 15.f, 39.5f)) });
    }

    {
        m_vChapterList[1]->SetBoundingBox({ pair(vector3(93.f, 5.f, -56.4f), vector3(32.f, 15.f, 46.f)) });
    }

    {
        m_vChapterList[2]->SetBoundingBox({ pair(vector3(93.6f, 5.f, -18.6f), vector3(45.f, 15.f, 31.f)) });
    }

    {
        m_vChapterList[3]->SetBoundingBox({ pair(vector3(93.2f, -1.85f, -121.4f), vector3(44.25f, 23.f, 85.5f)) });
    }

    {
        m_vChapterList[4]->SetBoundingBox({ pair(vector3(63.6f, 5.f, -71.4f), vector3(28.0f, 15.f, 78.1f)) });
    }

    {
        m_vChapterList[5]->SetBoundingBox({ pair(vector3(48.56f, 5.f, -21.72f), vector3(44.86f, 10.f, 22.83f)) });
    }

    {
        m_vChapterList[6]->SetBoundingBox({ pair(vector3(40.88f, 5.f, -71.4f), vector3(17.57f, 15.f, 78.1f)) });
    }

    {
        m_vChapterList[7]->SetBoundingBox({ pair(vector3(0.f, 5.f, -37.f), vector3(54.f, 10.f, 82.f)) });
        vector<vector3> wolfPos = { {18.f, 3.f, -90.f}, { 18.f, 3.f, -91.f}, { 18.f, 3.f, -89.f} };
        vector<_float> wolfRot = { -90.f, -90.f, -90.f };
        m_vChapterList[7]->AddMonsterSpawner({ m_mMonsterProtoList[L"Wolf"], wolfPos, wolfRot });
    }

    {
        m_vChapterList[8]->SetBoundingBox({ pair(vector3(-3.45f, 5.f, -93.45f), vector3(70.f, 10.f, 34.f)) });
    }

    {
        m_vChapterList[9]->SetBoundingBox({ pair(vector3(-63.7f, 1.6f, -19.2f), vector3(75.f, 10.f, 34.f)) });
    }

    {
        m_vChapterList[10]->SetBoundingBox({ pair(vector3(-57.f, -3.9f, -71.f), vector3(46.f, 16.5f, 67.75f)) });
    }
}

void CDungeon::CreateDungeonGates()
{
    for (size_t i = 0; i < 5; i++)
    {
        CGameObject* newObj = CGameObject::Instantiate(m_mDungonObjProtoList[L"Dungeon_Gate"]->Get_GameObject());
        newObj->Set_ObjectName(L"Gate (Clone) " + to_wstring(i));
        m_vGateList.push_back(newObj->GetComponent<CDungeonGate>());
        m_vGateList.back()->Get_GameObject()->SetActive(true);
    }

    m_vGateList[0]->Get_Transform()->Set_Position(-7.5f, 0.f, 3.8f);
    m_vGateList[1]->Get_Transform()->Set_Position(0.f, 0.f, 3.8f);
    m_vGateList[2]->Get_Transform()->Set_Position(7.5f, 0.f, 3.8f);

    m_vGateList[3]->Get_Transform()->Set_Position(108.8f, 0.f, -45.f);
    m_vGateList[3]->Get_Transform()->Set_EulerAnglesY(90.f);   

    m_vGateList[4]->Get_Transform()->Set_Position(78.75f, 0.f, -45.f);
    m_vGateList[4]->Get_Transform()->Set_EulerAnglesY(90.f);
    m_vGateList[4]->SetLock();
}

void CDungeon::CreateDungeonFootSwitches()
{
    for (size_t i = 0; i < 1; i++)
    {
        CGameObject* newObj = CGameObject::Instantiate(m_mDungonObjProtoList[L"Dungeon_FootSwitchPlat"]->Get_GameObject());
        newObj->Set_ObjectName(L"FootSwitch (Clone) " + to_wstring(i));
        m_vFootSwitchList.push_back(newObj->GetComponent<CFootSwitch>());
        m_vFootSwitchList.back()->Get_GameObject()->SetActive(true);
    }

    m_vFootSwitchList[0]->Get_Transform()->Set_Position(vector3(112.52f, 0.2f, -45.f));
    
    {
        vector<CDungeonGate*> gates = { m_vGateList[3] };
        m_vFootSwitchList[0]->Set_Gate(gates);
    }
}

void CDungeon::CreateDungeonChest()
{
    for (size_t i = 0; i < 1; i++)
    {
        CGameObject* newObj = CGameObject::Instantiate(m_mDungonObjProtoList[L"Dungeon_Chest"]->Get_GameObject());
        newObj->Set_ObjectName(L"Dungeon Chest (Clone) " + to_wstring(i));
        m_vChestList.push_back(newObj->GetComponent<CDungeonChest>());
        m_vChestList.back()->Get_GameObject()->SetActive(true);
    }

    m_vChestList[0]->Get_Transform()->Set_Position(vector3(93.85f, -2.98f, -146.f));
    m_vChestList[0]->Get_Transform()->Set_EulerAnglesY(180.f);
    m_vChestList[0]->Set_Item(L"DungeonKey");
}

void CDungeon::CreateDungeonLadder()
{
    for (size_t i = 0; i < 2; i++)
    {
        CGameObject* newObj = CGameObject::Instantiate(m_mDungonObjProtoList[L"Ladder"]->Get_GameObject());
        newObj->Set_ObjectName(L"Dungeon Ladder (Clone) " + to_wstring(i));
        m_vLadderList.push_back(newObj->GetComponent<CLadder>());
        m_vLadderList.back()->Get_GameObject()->SetActive(true);
    }

    m_vLadderList[0]->Get_Transform()->Set_Position(vector3(93.85f, -10.53f, -122.85f));
    m_vLadderList[0]->Get_Transform()->Set_EulerAnglesY(-90.f);

    m_vLadderList[1]->Get_Transform()->Set_Position(vector3(93.85f, -2.34f, -122.85f));
    m_vLadderList[1]->Get_Transform()->Set_EulerAngles(0.f, 90.f, -180.f);
}
