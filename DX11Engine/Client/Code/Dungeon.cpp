#include "cpch.h"
#include "Dungeon.h"
#include "DungeonChapter.h"
#include "Wolf.h"
#include "Goblin.h"
#include "Troll.h"

CDungeon::CDungeon()
    : m_mMonsterProtoList({})
    , m_vChapterList({})
    , m_bAttachedChapterColliders(false)
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
}

void CDungeon::Update()
{
    __super::Update();
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
}

void CDungeon::SpawnDungeonChapters()
{
    for (_uint i = 0; i < 2; ++i)
    {
        CGameObject* msObj = m_pGameObject->Get_Scene()->Add_GameObject(L"DungeonChapter_" + to_wstring(i));
        m_vChapterList.push_back(msObj->AddComponent<CDungeonChapter>());
    }

    {
        m_vChapterList[0]->SetBoundingBox({ pair(vector3(0.f, 5.f, -37.f), vector3(54.f, 10.f, 82.f)) });
        vector<vector3> wolfPos = { {18.f, 3.f, -90.f} };
        m_vChapterList[1]->AddMonsterSpawner({ m_mMonsterProtoList[L"Wolf"], wolfPos });
    }

    m_vChapterList[1]->SetBoundingBox({ pair(vector3(-3.45f, 5.f, -93.45f), vector3(64.3f, 10.f, 34.f)) });
}
