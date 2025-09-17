#include "cpch.h"
#include "Dungeon.h"
#include "DungeonChapter.h"

#include "Wolf.h"
#include "Goblin.h"
#include "Troll.h"
#include "Dragon.h"

#include "DungeonGate.h"
#include "FootSwitch.h"
#include "DungeonChest.h"
#include "Ladder.h"
#include "MovingPlat.h"

CDungeon::CDungeon()
    : m_mMonsterProtoList({})
    , m_mDungonObjProtoList({})
    , m_vChapterList({})
    , m_bAttachedChapterColliders(false)
    , m_vLightList({})
    , m_vGateList({})
    , m_vFootSwitchList({})
    , m_vChestList({})
    , m_pDragon(nullptr)
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

    CMeshRenderer* urn1 = Get_Transform()->Find_ChildRecursive(L"urn_1_2")->Get_GameObject()->GetComponent<CMeshRenderer>();
    CMeshRenderer* urn2 = Get_Transform()->Find_ChildRecursive(L"urn_2_1")->Get_GameObject()->GetComponent<CMeshRenderer>();
    CMeshRenderer* firepit = Get_Transform()->Find_ChildRecursive(L"firepit_1_1")->Get_GameObject()->GetComponent<CMeshRenderer>();
    urn1->Get_Material()->Set_FloatValue(L"gSmoothness", 0.5f);
    urn2->Get_Material()->Set_FloatValue(L"gSmoothness", 0.5f);
    firepit->Get_Material()->Set_FloatValue(L"gSmoothness", 0.5f);

    CMeshRenderer* coal = Get_Transform()->Find_ChildRecursive(L"firepit_coal")->Get_GameObject()->GetComponent<CMeshRenderer>();
    coal->Get_Material()->Set_FloatValue(L"gCalcLight", 0.f);

    CreatePointLights();
    CreateMonsterPrototypes();
    CreateDungonObjectPrototypes();
    CreateDungeonChapters();
    CreateDragon();

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

    SpawnDungeonGates();
    SpawnDungeonFootSwitches();
    SpawnDungeonChest();
    SpawnDungeonLadder();
    SpawnMovingPlat();

    Get_Transform()->Find_ChildRecursive(L"door_1")->Get_GameObject()->SetActive(false);
}

void CDungeon::Update()
{
    __super::Update();

    CullingLights();

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

void CDungeon::CreateMonsterPrototypes()
{
    CrateMonsterPrototype<CWolf>();
    CrateMonsterPrototype<CGoblin>();
    CrateMonsterPrototype<CTroll>();
}

void CDungeon::CreateDungonObjectPrototypes()
{
    CreateDungonObjectPrototype<CDungeonGate>();
    CreateDungonObjectPrototype<CFootSwitch>();
    CreateDungonObjectPrototype<CDungeonChest>();
    CreateDungonObjectPrototype<CLadder>();
    CreateDungonObjectPrototype<CMovingPlat>();
}

void CDungeon::CreateDungeonChapters()
{
    for (_uint i = 0; i < 15; ++i)
    {
        CGameObject* msObj = m_pGameObject->Get_Scene()->Add_GameObject(L"DungeonChapter_" + to_wstring(i));
        m_vChapterList.push_back(msObj->AddComponent<CDungeonChapter>());
    }

    {
        m_vChapterList[0]->SetBoundingBox({ pair(vector3(124.5f, 5.f, -59.3f), vector3(31.f, 15.f, 39.5f)) });
    }

    {
        m_vChapterList[1]->SetBoundingBox({ pair(vector3(93.f, 5.f, -56.4f), vector3(32.f, 15.f, 46.f)) });

        vector<vector3> goblinPos = { {93.75f, 3.f, -71.65f} };
        vector<_float> goblinRot = { 0.f };
        m_vChapterList[1]->AddMonsterSpawner({ m_mMonsterProtoList[L"Goblin"], goblinPos, goblinRot });
    }

    {
        m_vChapterList[2]->SetBoundingBox({ pair(vector3(93.6f, 5.f, -18.6f), vector3(45.f, 15.f, 31.f)) });
    }

    {
        m_vChapterList[3]->SetBoundingBox({ pair(vector3(93.2f, -1.85f, -121.4f), vector3(44.25f, 23.f, 85.5f)) });

        vector<vector3> goblinPos = { {93.8f, -2.9f, -143.7f} };
        vector<_float> goblinRot = { 0.f };
        m_vChapterList[3]->AddMonsterSpawner({ m_mMonsterProtoList[L"Goblin"], goblinPos, goblinRot });
    }

    {
        m_vChapterList[4]->SetBoundingBox({ pair(vector3(63.6f, 9.8f, -71.4f), vector3(28.0f, 20.f, 78.1f)) });
    }

    {
        m_vChapterList[5]->SetBoundingBox({ pair(vector3(48.56f, 5.f, -21.72f), vector3(44.86f, 15.f, 22.83f)) });
    }

    {
        m_vChapterList[6]->SetBoundingBox({ pair(vector3(40.88f, 6.5f, -71.4f), vector3(17.57f, 15.f, 78.1f)) });
    }

    {
        m_vChapterList[7]->SetBoundingBox({ pair(vector3(0.f, 5.f, -37.f), vector3(54.f, 10.f, 82.f)) });
        vector<vector3> wolfPos = { {18.f, 3.f, -90.f}, { 18.f, 3.f, -91.f}, { 18.f, 3.f, -89.f} };
        vector<_float> wolfRot = { -90.f, -90.f, -90.f };
        m_vChapterList[7]->AddMonsterSpawner({ m_mMonsterProtoList[L"Wolf"], wolfPos, wolfRot });
    }

    {
        m_vChapterList[8]->SetBoundingBox({ pair(vector3(-3.45f, 7.3f, -93.45f), vector3(70.f, 15.f, 34.f)) });
    }

    {
        m_vChapterList[9]->SetBoundingBox({ pair(vector3(-63.7f, 1.6f, -30.45f), vector3(75.f, 15.f, 58.f)) });
    }

    {
        m_vChapterList[10]->SetBoundingBox({ pair(vector3(-57.f, -3.9f, -71.f), vector3(46.f, 16.5f, 67.75f)) });
    }

    {
        m_vChapterList[11]->SetBoundingBox({ pair(vector3(-61.f, -3.9f, -126.85f), vector3(40.f, 16.5f, 45.f)) });
    }

    {
        m_vChapterList[12]->SetBoundingBox({ pair(vector3(-102.45f, -3.9f, -123.f), vector3(45.f, 16.5f, 50.f)) });
    }

    {
        m_vChapterList[13]->SetBoundingBox({ pair(vector3(-106.45f, -3.9f, -75.35f), vector3(55.f, 16.5f, 48.f)) });
    }

    {
        m_vChapterList[14]->SetBoundingBox({ pair(vector3(-119.25f, 0.75f, -18.45f), vector3(40.f, 16.5f, 50.f)) });
    }
}

void CDungeon::CreateDragon()
{
    CGameObject* dragonObj = m_pGameObject->Get_Scene()->Add_GameObject(L"Dragon");
    m_pDragon = dragonObj->AddComponent<CDragon>();
}

void CDungeon::CreatePointLights()
{
    CScene* scene = m_pGameObject->Get_Scene();

    for (_uint i = 0; i < 134; ++i)
    {
        CGameObject* newObj = scene->Add_GameObject(L"PointLight" + to_wstring(i));
        CLight* newPointLight = newObj->AddComponent<CLight>();
        newPointLight->Set_Type(CLight::Type::Point);
        newPointLight->Set_Range(15.f);
        newPointLight->Set_Color(ColorValue(180, 100, 0, 255));
        m_vLightList.push_back(newPointLight);
    }

    m_vLightList[0]->Get_Transform()->Set_Position(127.5f, 1.f, -72.4f);
    m_vLightList[1]->Get_Transform()->Set_Position(132.75f, 1.f, -67.55f);
    m_vLightList[2]->Get_Transform()->Set_Position(127.5f, 1.f, -62.25f);
    m_vLightList[3]->Get_Transform()->Set_Position(112.45f, 4.f, -45.f);
    m_vLightList[4]->Get_Transform()->Set_Position(97.5f, 4.5f, -26.25f);
    m_vLightList[5]->Get_Transform()->Set_Position(90.f, 4.5f, -26.25f);
    m_vLightList[6]->Get_Transform()->Set_Position(101.f, 3.5f, -41.f);
    m_vLightList[7]->Get_Transform()->Set_Position(93.7f, 6.48f, -22.55f);
    m_vLightList[7]->Set_Color(ColorValue(0, 0, 200));
    m_vLightList[8]->Get_Transform()->Set_Position(86.25f, 3.5f, -41.f);
    m_vLightList[9]->Get_Transform()->Set_Position(101.25f, 8.72f, -55.2f);
    m_vLightList[10]->Get_Transform()->Set_Position(86.25f, 8.72f, -55.2f);
    m_vLightList[11]->Get_Transform()->Set_Position(101.25f, 8.f, -65.f);
    m_vLightList[12]->Get_Transform()->Set_Position(86.25f, 8.f, -65.f);
    m_vLightList[13]->Get_Transform()->Set_Position(101.25f, 8.f, -77.5f);
    m_vLightList[14]->Get_Transform()->Set_Position(86.25f, 8.f, -77.5f);
    m_vLightList[15]->Get_Transform()->Set_Position(101.f, 4.4f, -109.f);
    m_vLightList[16]->Get_Transform()->Set_Position(86.25f, 4.4f, -109.f);
    m_vLightList[17]->Get_Transform()->Set_Position(93.5f, -4.f, -116.5f);
    m_vLightList[18]->Get_Transform()->Set_Position(101.f, 4.4f, -123.7f);
    m_vLightList[19]->Get_Transform()->Set_Position(86.25f, 4.4f, -123.7f);
    m_vLightList[20]->Get_Transform()->Set_Position(97.49f, 1.75f, -131.f);
    m_vLightList[21]->Get_Transform()->Set_Position(90.f, 1.75f, -131.f);
    m_vLightList[22]->Get_Transform()->Set_Position(93.7f, 0.77f, -152.35f);
    m_vLightList[22]->Set_Color(ColorValue(0, 0, 200));
    m_vLightList[23]->Get_Transform()->Set_Position(80.f, 5.f, -41.f);
    m_vLightList[24]->Get_Transform()->Set_Position(80.f, 5.f, -48.75);
    m_vLightList[25]->Get_Transform()->Set_Position(67.5, 2.5, -22.5f);
    m_vLightList[26]->Get_Transform()->Set_Position(77.65f, 5.f, -48.75f);
    m_vLightList[27]->Get_Transform()->Set_Position(63.75f, 4.7f, -33.8f);
    m_vLightList[28]->Get_Transform()->Set_Position(70.f, 5.f, -56.2f);
    m_vLightList[29]->Get_Transform()->Set_Position(57.35f, 5.f, -56.2f);
    m_vLightList[30]->Get_Transform()->Set_Position(56.2f, 5.3f, -77.2f);
    m_vLightList[31]->Get_Transform()->Set_Position(56.2f, 5.3f, -87.75f);
    m_vLightList[32]->Get_Transform()->Set_Position(60.f, 11.f, -107.65f);
    m_vLightList[33]->Get_Transform()->Set_Position(52.35f, 8.77f, -101.25);
    m_vLightList[34]->Get_Transform()->Set_Position(47.65f, 11.f, -101.25f);
    m_vLightList[35]->Get_Transform()->Set_Position(47.65f, 12.f, -71.25f);
    m_vLightList[36]->Get_Transform()->Set_Position(34.8f, 12.f, -71.25f);
    m_vLightList[37]->Get_Transform()->Set_Position(47.65f, 5.f, -48.75f);
    m_vLightList[38]->Get_Transform()->Set_Position(34.8f, 5.f, -48.75f);
    m_vLightList[39]->Get_Transform()->Set_Position(41.25f, 4.55f, -33.7f);
    m_vLightList[40]->Get_Transform()->Set_Position(55.2f, 5.f, -26.25f);
    m_vLightList[41]->Get_Transform()->Set_Position(55.2f, 5.f, -18.75f);
    m_vLightList[42]->Get_Transform()->Set_Position(27.35f, 5.f, -26.25f);
    m_vLightList[43]->Get_Transform()->Set_Position(27.35f, 5.f, -18.75f);
    m_vLightList[44]->Get_Transform()->Set_Position(25.2f, 5.f, -18.75f);
    m_vLightList[45]->Get_Transform()->Set_Position(25.2f, 5.f, -26.25f);
    m_vLightList[46]->Get_Transform()->Set_Position(12.3f, 8.65f, -33.75f);
    m_vLightList[47]->Get_Transform()->Set_Position(11.3f, 4.45f, -48.7f);
    m_vLightList[48]->Get_Transform()->Set_Position(10.15f, 8.65f, -33.75f);
    m_vLightList[49]->Get_Transform()->Set_Position(-12.35f, 8.65f, -33.75f);
    m_vLightList[50]->Get_Transform()->Set_Position(-11.25f, 4.45f, -48.7f);
    m_vLightList[51]->Get_Transform()->Set_Position(10.1f, 5.f, -11.25f);
    m_vLightList[52]->Get_Transform()->Set_Position(-10.15f, 5.f, -11.25f);
    m_vLightList[53]->Get_Transform()->Set_Position(5.2f, 5.2f, -56.2f);
    m_vLightList[54]->Get_Transform()->Set_Position(-5.27f, 5.2f, -56.2f);
    m_vLightList[55]->Get_Transform()->Set_Position(16.1f, 11.68f, -57.35f);
    m_vLightList[56]->Get_Transform()->Set_Position(16.2f, 11.68f, -57.35f);
    m_vLightList[57]->Get_Transform()->Set_Position(17.6f, 11.68f, -58.9f);
    m_vLightList[58]->Get_Transform()->Set_Position(-17.6f, 11.68f, -58.9f);
    m_vLightList[59]->Get_Transform()->Set_Position(12.35f, 11.68f, -63.75f);
    m_vLightList[60]->Get_Transform()->Set_Position(-12.35f, 11.68f, -63.75f);
    m_vLightList[61]->Get_Transform()->Set_Position(5.25f, 5.5f, -56.2f);
    m_vLightList[62]->Get_Transform()->Set_Position(-5.25f, 5.5f, -56.2f);
    m_vLightList[63]->Get_Transform()->Set_Position(3.75f, 13.55f, -71.25);
    m_vLightList[64]->Get_Transform()->Set_Position(-3.75f, 13.55f, -71.25);
    m_vLightList[65]->Get_Transform()->Set_Position(-3.75f, 13.55f, -71.25);
    m_vLightList[66]->Get_Transform()->Set_Position(0.f, 9.58f, -67.5f);
    m_vLightList[66]->Set_Color(ColorValue(0, 0, 200));
    m_vLightList[67]->Get_Transform()->Set_Position(22.5f, 4.f, -75.f);
    m_vLightList[68]->Get_Transform()->Set_Position(-22.5f, 4.f, -75.f);
    m_vLightList[69]->Get_Transform()->Set_Position(3.8f, 7.7f, -86.25f);
    m_vLightList[70]->Get_Transform()->Set_Position(-3.8f, 7.7f, -86.25f);
    m_vLightList[71]->Get_Transform()->Set_Position(3.8f, 7.7f, -93.7f);
    m_vLightList[72]->Get_Transform()->Set_Position(-3.8f, 7.7f, -93.7f);
    m_vLightList[73]->Get_Transform()->Set_Position(-32.65f, 11.f, -93.75f);
    m_vLightList[74]->Get_Transform()->Set_Position(-32.65f, 11.f, -93.75f);
    m_vLightList[75]->Get_Transform()->Set_Position(-32.65f, 11.f, -103.7f);
    m_vLightList[75]->Set_Color(ColorValue::white());
    m_vLightList[76]->Get_Transform()->Set_Position(-116.4f, -1.f, -132.35f);
    m_vLightList[77]->Get_Transform()->Set_Position(-117.35f, -1.f, -131.2f);
    m_vLightList[78]->Get_Transform()->Set_Position(-116.f, -1.f, -130.f);
    m_vLightList[79]->Get_Transform()->Set_Position(-108.7f, -1.35f, -123.73f);
    m_vLightList[80]->Get_Transform()->Set_Position(-108.7f, -1.35f, -108.7f);
    m_vLightList[81]->Get_Transform()->Set_Position(-93.74f, -1.05f, -107.65f);
    m_vLightList[82]->Get_Transform()->Set_Position(-108.76f, -1.05f, -108.76f);
    m_vLightList[83]->Get_Transform()->Set_Position(-70.2f, -4.f, -131.2f);
    m_vLightList[84]->Get_Transform()->Set_Position(-70.2f, -4.f, -131.2f);
    m_vLightList[85]->Get_Transform()->Set_Position(-63.1f, -4.24f, -130.7f);
    m_vLightList[86]->Get_Transform()->Set_Position(-56.28f, -4.24f, -130.7f);
    m_vLightList[87]->Get_Transform()->Set_Position(-49.85f, -4.24f, -131.24f);
    m_vLightList[88]->Get_Transform()->Set_Position(-49.85f, -4.24f, -123.75f);
    m_vLightList[89]->Get_Transform()->Set_Position(-55.15f, -4.f, -86.23f);
    m_vLightList[90]->Get_Transform()->Set_Position(-57.35f, -4.f, -86.23f);
    m_vLightList[91]->Get_Transform()->Set_Position(-55.15f, -4.f, -75.f);
    m_vLightList[92]->Get_Transform()->Set_Position(-55.15f, -4.f, -63.73f);
    m_vLightList[93]->Get_Transform()->Set_Position(-81.23f, -5.f, -75.f);
    m_vLightList[94]->Get_Transform()->Set_Position(-100.f, -4.f, -63.78f);
    m_vLightList[95]->Get_Transform()->Set_Position(-101.33f, -4.f, -62.64f);
    m_vLightList[96]->Get_Transform()->Set_Position(-101.33f, -4.f, -63.78f);
    m_vLightList[97]->Get_Transform()->Set_Position(-101.33f, -4.f, -64.85f);
    m_vLightList[98]->Get_Transform()->Set_Position(-122.68f, -4.f, -63.78f);
    m_vLightList[99]->Get_Transform()->Set_Position(-124.87f, -4.f, -64.85f);
    m_vLightList[99]->Get_Transform()->Set_Position(-124.87f, -4.f, -63.78f);
    m_vLightList[100]->Get_Transform()->Set_Position(-123.68f, -4.f, -62.64f);
    m_vLightList[101]->Get_Transform()->Set_Position(-123.68f, -4.f, -64.85f);
    m_vLightList[102]->Get_Transform()->Set_Position(-117.74f, -6.58f, -71.21f);
    m_vLightList[103]->Get_Transform()->Set_Position(-107.22f, -6.58f, -71.21f);
    m_vLightList[104]->Get_Transform()->Set_Position(-124.84f, -1.f, -86.2f);
    m_vLightList[105]->Get_Transform()->Set_Position(-122.62f, -1.f, -86.28f);
    m_vLightList[106]->Get_Transform()->Set_Position(-102.35f, -1.f, -86.28f);
    m_vLightList[107]->Get_Transform()->Set_Position(-100.1f, -1.f, -86.2f);
    m_vLightList[108]->Get_Transform()->Set_Position(-123.8f, -1.f, -87.35f);
    m_vLightList[109]->Get_Transform()->Set_Position(-123.7f, -1.f, -85.15f);
    m_vLightList[110]->Get_Transform()->Set_Position(-123.7f, -1.f, -87.35f);
    m_vLightList[111]->Get_Transform()->Set_Position(-101.2f, -1.f, -87.4f);
    m_vLightList[112]->Get_Transform()->Set_Position(-101.28f, -1.f, -85.15f);
    m_vLightList[113]->Get_Transform()->Set_Position(-55.15f, 1.9f, -48.76f);
    m_vLightList[114]->Get_Transform()->Set_Position(-57.36f, 1.9f, -48.76f);
    m_vLightList[115]->Get_Transform()->Set_Position(-27.34f, 4.9f, -26.25f);
    m_vLightList[116]->Get_Transform()->Set_Position(-27.34f, 4.9f, -18.75f);
    m_vLightList[117]->Get_Transform()->Set_Position(-48.74f, 4.9f, -32.65f);
    m_vLightList[118]->Get_Transform()->Set_Position(-63.73f, 4.9f, -32.65f);
    m_vLightList[119]->Get_Transform()->Set_Position(-75.f, -2.f, -30.f);
    m_vLightList[120]->Get_Transform()->Set_Position(-75.f, -2.f, -15.f);
    m_vLightList[121]->Get_Transform()->Set_Position(-42.34f, 4.9f, -11.26f);
    m_vLightList[122]->Get_Transform()->Set_Position(-92.65f, 5.7f, -11.26f);
    m_vLightList[123]->Get_Transform()->Set_Position(-100.18f, 1.9f, -18.75);
    m_vLightList[124]->Get_Transform()->Set_Position(-100.18f, 1.9f, -26.26f);
    m_vLightList[125]->Get_Transform()->Set_Position(-109.88f, 1.9f, -26.26f);
    m_vLightList[126]->Get_Transform()->Set_Position(-109.88f, 1.9f, -11.22f);
    m_vLightList[127]->Get_Transform()->Set_Position(-116.3f, 1.5f, -18.75);
    m_vLightList[128]->Get_Transform()->Set_Position(-122.64f, 1.9f, -11.22f);
    m_vLightList[129]->Get_Transform()->Set_Position(-122.64f, 1.9f, -26.28f);
    m_vLightList[130]->Get_Transform()->Set_Position(-56.2f, -4.f, -100.18f);
    m_vLightList[131]->Get_Transform()->Set_Position(-93.7f, -0.83f, -137.62f);
    m_vLightList[132]->Get_Transform()->Set_Position(-93.76f, -0.83f, -139.84f);
    m_vLightList[133]->Get_Transform()->Set_Position(-94.86f, -0.83f, -138.7f);

    for (TRAVERSAL_ITER(m_vLightList, it))
        (*it)->Get_GameObject()->Set_Static(CGameObject::TransformStatic);      
}

void CDungeon::SpawnDungeonGates()
{
    if (!m_mDungonObjProtoList[L"Dungeon_Gate"])
        return;

    for (_uint i = 0; i < 19; ++i)
    {
        CGameObject* newObj = CGameObject::Instantiate(m_mDungonObjProtoList[L"Dungeon_Gate"]->Get_GameObject());
        newObj->Set_ObjectName(L"Gate (Clone) " + to_wstring(i));
        m_vGateList.push_back(newObj->GetComponent<CDungeonGate>());
        m_vGateList.back()->Get_GameObject()->SetActive(true);
    }

    m_vGateList[0]->Get_Transform()->Set_Position(108.8f, 0.f, -45.f);
    m_vGateList[0]->Get_Transform()->Set_EulerAnglesY(90.f);   

    m_vGateList[1]->Get_Transform()->Set_Position(78.75f, 0.f, -45.f);
    m_vGateList[1]->Get_Transform()->Set_EulerAnglesY(90.f);
    m_vGateList[1]->SetLock();

    m_vGateList[2]->Get_Transform()->Set_Position(71.25f, 0.f, -22.5f);
    m_vGateList[2]->Get_Transform()->Set_EulerAnglesY(90.f);

    m_vGateList[3]->Get_Transform()->Set_Position(56.28f, 0.f, -22.5f);
    m_vGateList[3]->Get_Transform()->Set_EulerAnglesY(90.f);

    m_vGateList[4]->Get_Transform()->Set_Position(45.f, 0.f, -41.2f);
    m_vGateList[5]->Get_Transform()->Set_Position(37.45f, 0.f, -41.2f);

    m_vGateList[6]->Get_Transform()->Set_Position(48.77f, 5.95f, -97.5f);
    m_vGateList[6]->Get_Transform()->Set_EulerAnglesY(90.f);

    m_vGateList[7]->Get_Transform()->Set_Position(22.47f, 3.f, -71.26f);
    m_vGateList[8]->Get_Transform()->Set_Position(15.f, 3.f, -78.72f);
    m_vGateList[9]->Get_Transform()->Set_Position(-15.f, 3.f, -78.72f);
    m_vGateList[10]->Get_Transform()->Set_Position(-22.47f, 3.f, -71.26f);
    
    m_vGateList[11]->Get_Transform()->Set_Position(26.27f, 0.f, -22.47f);
    m_vGateList[11]->Get_Transform()->Set_EulerAnglesY(90.f);
    m_vGateList[12]->Get_Transform()->Set_Position(-26.27f, 0.f, -22.47f);
    m_vGateList[12]->Get_Transform()->Set_EulerAnglesY(90.f);

    m_vGateList[13]->Get_Transform()->Set_Position(7.5f, 0.f, 3.8f);
    m_vGateList[14]->Get_Transform()->Set_Position(0.f, 0.f, 3.8f);
    m_vGateList[15]->Get_Transform()->Set_Position(-7.5f, 0.f, 3.8f);
    m_vGateList[16]->Get_Transform()->Set_Position(22.5f, 0.f, -11.27f);
    m_vGateList[17]->Get_Transform()->Set_Position(-22.5f, 0.f, -11.27f);
}

void CDungeon::SpawnDungeonFootSwitches()
{
    if (!m_mDungonObjProtoList[L"Dungeon_FootSwitchPlat"])
        return;

    for (size_t i = 0; i < 3; ++i)
    {
        CGameObject* newObj = CGameObject::Instantiate(m_mDungonObjProtoList[L"Dungeon_FootSwitchPlat"]->Get_GameObject());
        newObj->Set_ObjectName(L"FootSwitch (Clone) " + to_wstring(i));
        m_vFootSwitchList.push_back(newObj->GetComponent<CFootSwitch>());
        m_vFootSwitchList.back()->Get_GameObject()->SetActive(true);
    }
    
    {
        m_vFootSwitchList[0]->Get_Transform()->Set_Position(vector3(112.52f, 0.2f, -45.f));
        vector<CDungeonGate*> gates = { m_vGateList[0] };
        m_vFootSwitchList[0]->Set_Gate(gates);
    }

    {
        m_vFootSwitchList[1]->Get_Transform()->Set_Position(vector3(67.5f, 0.2f, -22.5f));
        vector<CDungeonGate*> gates = { m_vGateList[2] };
        m_vFootSwitchList[1]->Set_Gate(gates);
    }

    {
        m_vFootSwitchList[2]->Get_Transform()->Set_Position(vector3(52.45f, 6.015f, -101.27f));
        vector<CDungeonGate*> gates = { m_vGateList[6] };
        m_vFootSwitchList[2]->Set_Gate(gates);
    }
}

void CDungeon::SpawnDungeonChest()
{
    if (!m_mDungonObjProtoList[L"Dungeon_Chest"])
        return;

    for (size_t i = 0; i < 3; ++i)
    {
        CGameObject* newObj = CGameObject::Instantiate(m_mDungonObjProtoList[L"Dungeon_Chest"]->Get_GameObject());
        newObj->Set_ObjectName(L"Dungeon Chest (Clone) " + to_wstring(i));
        m_vChestList.push_back(newObj->GetComponent<CDungeonChest>());
        m_vChestList.back()->Get_GameObject()->SetActive(true);
    }

    {
        m_vChestList[0]->Get_Transform()->Set_Position(vector3(93.85f, -2.98f, -146.f));
        m_vChestList[0]->Get_Transform()->Set_EulerAnglesY(180.f);
        m_vChestList[0]->Set_Item(L"DungeonKey");
    }

    {
        m_vChestList[1]->Get_Transform()->Set_Position(vector3(93.72f, 3.1f, -24.5f));
        m_vChestList[1]->Get_Transform()->Set_LocalScale(1.5f);
        m_vChestList[1]->Set_Item(L"Sword");
    }

    {
        m_vChestList[2]->Get_Transform()->Set_Position(vector3(67.4f, 6.f, -105.f));
        m_vChestList[2]->Get_Transform()->Set_EulerAnglesY(90.f);
        m_vChestList[2]->Get_Transform()->Set_LocalScale(1.5f);
        m_vChestList[2]->Set_Item(L"Bow");
    }
}

void CDungeon::SpawnDungeonLadder()
{
    if (!m_mDungonObjProtoList[L"Ladder"])
        return;

    for (size_t i = 0; i < 3; ++i)
    {
        CGameObject* newObj = CGameObject::Instantiate(m_mDungonObjProtoList[L"Ladder"]->Get_GameObject());
        newObj->Set_ObjectName(L"Dungeon Ladder (Clone) " + to_wstring(i));
        m_vLadderList.push_back(newObj->GetComponent<CLadder>());
        m_vLadderList.back()->Get_GameObject()->SetActive(true);
    }

    m_vLadderList[0]->Get_Transform()->Set_Position(vector3(93.85f, -10.53f, -122.85f));
    m_vLadderList[0]->Get_Transform()->Set_EulerAnglesY(-90.f);
    m_vLadderList[0]->Set_HeightValue(-10.f, -1.3f, 0.1f);

    m_vLadderList[1]->Get_Transform()->Set_Position(vector3(93.83f, -0.027f, -122.85f));
    m_vLadderList[1]->Get_Transform()->Set_EulerAngles(0.f, 90.f, -180.f);

    m_vLadderList[2]->Get_Transform()->Set_Position(vector3(-33.f, 6.7f, -103.65f));
    m_vLadderList[2]->Set_HeightValue(5.8f, 11.8f, 12.1f);
}

void CDungeon::SpawnMovingPlat()
{
    if (!m_mDungonObjProtoList[L"MovingPlat"])
        return;

    for (size_t i = 0; i < 2; ++i)
    {
        CGameObject* newObj = CGameObject::Instantiate(m_mDungonObjProtoList[L"MovingPlat"]->Get_GameObject());
        newObj->Set_ObjectName(L"Moving Plat (Clone) " + to_wstring(i));
        m_vMovingPlatList.push_back(newObj->GetComponent<CMovingPlat>());
        m_vMovingPlatList.back()->Get_GameObject()->SetActive(true);
    }

    {
        m_vMovingPlatList[0]->Get_Transform()->Set_Position(-37.25f, 10.4f, -103.8f);
        m_vMovingPlatList[0]->AddRout(vector3(-37.25f, 10.4f, -103.8f));
        m_vMovingPlatList[0]->AddRout(vector3(-46.f, -3.5f, -94.45f));
    }

    {
        m_vMovingPlatList[1]->Get_Transform()->Set_Position(54.f, 3.5f, -90.8f);
        m_vMovingPlatList[1]->AddRout(vector3(54.f, 3.5f, -90.8f));
        m_vMovingPlatList[1]->AddRout(vector3(54.f, 12.f, -90.8f));
    }
}

void CDungeon::CullingLights()
{
    vector3 camPos = CGameManager::GetInstance().Get_PlayerCamera()->Get_Transform()->Get_Position();

    sort(m_vLightList.begin(), m_vLightList.end(),
        [&camPos](const auto& a, const auto& b)
        {
            _float distA = vector3::Distance(a->Get_Transform()->Get_Position(), camPos);
            _float distB = vector3::Distance(b->Get_Transform()->Get_Position(), camPos);
            return distA < distB;
        });

    _uint count = 0;

#ifdef _DEBUG
    count = 15;
#elif
    count = 40;
#endif

    for (_uint i = 0; i < m_vLightList.size(); ++i)
        m_vLightList[i]->SetEnabled(i < count);
}
