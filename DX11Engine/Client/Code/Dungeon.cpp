#include "cpch.h"
#include "Dungeon.h"
#include "DungeonChapter.h"
#include "Wolf.h"
#include "Goblin.h"
#include "Troll.h"

CDungeon::CDungeon()
    : m_vMonsterProtoList({})
    , m_vChapterList({})
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
}

void CDungeon::Start()
{
    __super::Start();
}

void CDungeon::Update()
{
    __super::Update();
}

void CDungeon::OnDestroy()
{
    __super::OnDestroy();
}

void CDungeon::SpawnMonsterPrototypes()
{
    CGameObject* wolfObj = m_pGameObject->Get_Scene()->Add_GameObject(L"Prototype_Wolf");
    m_vMonsterProtoList.push_back(wolfObj->AddComponent<CWolf>());
    wolfObj->SetActive(false);
}

void CDungeon::SpawnDungeonChapters()
{
    for (_uint i = 0; i < 1; ++i)
    {
        CGameObject* msObj = m_pGameObject->Get_Scene()->Add_GameObject(L"DungeonChapter_" + to_wstring(i));
        m_vChapterList.push_back(msObj->AddComponent<CDungeonChapter>());
    }

    m_vChapterList[0]->SetBoundingBox({ pair(vector3(0.f, 5.f, -37.f), vector3(52.f, 10.f, 82.f)) });
}
