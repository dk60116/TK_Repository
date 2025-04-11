#include "CPlayer.h"
#include "CInput.h"
#include "CGameObject.h"
#include "CSceneManager.h"
#include "CResourceTile.h"
#include "CCanvas.h"
#include "CInventoryUI.h"
#include "CSpriteRender_Level.h"
#include "CMapManager.h"
#include "CStoneFurnace.h"
#include "CPhysics.h"
#include "CCollider_Rect.h"
#include "CInOutStructure.h"
#include "CBeltManager.h"
#include "CBurnerDrill.h"
#include "CDropItem.h"
#include "CMonsterManager.h"

CPlayer::CPlayer()
    : m_sStat({})
    , m_pPlayerAnim(nullptr)
    , m_pPlayerMaskAnim(nullptr)
    , m_pShadowAnim(nullptr)
    , m_pMouseOverTile(nullptr)
    , m_pStayTile(nullptr)
    , m_pOverStructure(nullptr)
    , m_v2Direction(vector2::zero())
    , m_v2PrevDirection(vector2::zero())
    , m_v2MiningDirection(vector2::zero())
    , m_bIsWalk(false)
    , m_bIsMining(false)
    , m_bCaptuering(false)
    , m_pInventory(nullptr)
    , m_pHpBar(nullptr)
    , m_pHpGauge(nullptr)
    , m_pCollectingGaugeBar(nullptr)
    , m_pCollectingGauge(nullptr)
    , m_pCurrentCollectingTile(nullptr)
    , m_pHandItem(nullptr)
    , m_pCurrentHandItemIcon(nullptr)
    , m_pBlueprint(nullptr)
    , m_pBPL(nullptr)
    , m_pCurrentMouseItemSlot(nullptr)
    , m_vCreateItemQueue()
    , m_vCreatingSlotList({})
    , m_fCreateItemTime(0.f)
    , m_bCanCreateStructure(true)
    , m_bCreatedCStructure(false)
    , m_pSoundObj(nullptr)
    , m_pCollider(nullptr)
    , m_eCrtStructureDir(CStructure::RIGHT)
    , m_iEightDirection(0)
    , m_fLastHitTime(0)
    , m_bIsShooting(false)
    , m_pShootTarget(nullptr)
    , m_pGunMuzzleEffect(nullptr)
    , m_fShootTime(0.f)
{
}

CPlayer::~CPlayer()
{
    OnDestroy();
}

void CPlayer::Awake()
{
	Awake_();

    m_sStat.maxHp = 100.f;
    m_sStat.crtHp = m_sStat.maxHp;
    m_sStat.moveSpeed = 6.f;

    CGameObject* colliderObj = m_pGameObject->getScene()->AddObject(L"Player Collider", Layer::Player);
    m_pCollider = dynamic_cast<CCollider*>(colliderObj->AddComponent(new CCollider_Rect(vector2::one(), vector2::down() * 0.25f)));
    m_pCollider->SetBody(m_pGameObject);
    m_pCollider->SetStatic(false);
    CPhysics::GetInstance().AddCollider(m_pCollider, Layer::Player);
    colliderObj->SetParent(m_pGameObject);

    CGameObject* hpBarObj = new CGameObject();
    hpBarObj->SetLayer(Layer::UI);
    hpBarObj->SetName(L"Player Hp Bar");
    m_pHpBar = dynamic_cast<CImage*>(hpBarObj->AddComponent(new CImage(L"")));
    m_pHpBar->getRect().SetAnchorPreset(vector2(0.f, 1.f));
    m_pHpBar->getRect().SetAnchoredPos(vector2::up() * 130.f);
    m_pHpBar->getRect().SetSize(vector2(200.f, 20.f));
    m_pHpBar->SetColor(ColorValue::Black());
    CSceneManager::GetInstance().getCurrentscene()->FindObjectOfType<CCanvas>(Layer::UI)->InsertUI(m_pHpBar);
    
    CGameObject* hpGaugeObj = new CGameObject();
    hpGaugeObj->SetLayer(Layer::UI);
    hpGaugeObj->SetName(L"Player Hp Gauge");
    m_pHpGauge = dynamic_cast<CImage*>(hpGaugeObj->AddComponent(new CImage(L"UI_GaugeBar_Green")));
    m_pHpGauge->SetSpriteArea(vector2Int::zero(), vector2Int::one() * 16);
    m_pHpGauge->getRect().SetSize(vector2(200.f, 20.f));
    m_pHpGauge->SetParent(m_pHpBar);
    CSceneManager::GetInstance().getCurrentscene()->FindObjectOfType<CCanvas>(Layer::UI)->InsertUI(m_pHpGauge);
    m_pHpGauge->SetFillAmountX(0.f);

    hpBarObj->SetActive(false);

    for (int i = 0; i < 4; ++i)
    {
        CGameObject* creatingUIObj = new CGameObject();
        creatingUIObj->SetLayer(Layer::UI);
        creatingUIObj->SetName(L"CreatingUIObj" + to_wstring(i));
        m_vCreatingSlotList.push_back(dynamic_cast<CCreatingSlot*>(creatingUIObj->AddComponent(new CCreatingSlot(i))));
        CSceneManager::GetInstance().getCurrentscene()->FindObjectOfType<CCanvas>(Layer::UI)->InsertUI(m_vCreatingSlotList.back());
        m_vCreatingSlotList.back()->getRect().SetAnchorPreset(vector2(-1.f, 1.f));
        m_vCreatingSlotList.back()->getRect().SetAnchoredPos(vector2(100 + 45 * i, -60));
        m_vCreatingSlotList.back()->Start();
        m_vCreatingSlotList.back()->getObj()->SetActive(false);
    }

    CGameObject* soundObj = m_pGameObject->getScene()->AddObject(L"Player Sound", Layer::Default);
    m_pSoundObj = dynamic_cast<CAudioSource*>(soundObj->AddComponent(new CAudioSource()));
    //m_pSoundObj->SetClip(L"BGM_Wind");

    CGameObject* gunMuzzleEffectObj = m_pGameObject->getScene()->AddObject(L"GunMuzzle Effect", Layer::Player);
    m_pGunMuzzleEffect = dynamic_cast<CAnimator*>(gunMuzzleEffectObj->AddComponent(new CAnimator()));
    gunMuzzleEffectObj->getTransform().SetScale(vector2::one() * 0.5f);
    gunMuzzleEffectObj->SetParent(m_pGameObject);
    gunMuzzleEffectObj->AddComponent(new CSpriteRenderer(m_pGunMuzzleEffect));
    m_pGunMuzzleEffect->CreateAnimation(L"Shoot", L"Ptc_Gun_Muzzle", 13, vector2::zero(), vector2Int(34, 32), 0, 0.05f);
    m_pGunMuzzleEffect->SetAnimation(L"Shoot");
    gunMuzzleEffectObj->SetActive(false);
}

void CPlayer::Start()
{
	Start_();

	m_pPlayerAnim = getObj()->GetComponent<CAnimator>();

    CGameObject* handItemIcon = new CGameObject();
    handItemIcon->SetLayer(Layer::UI);
    handItemIcon->SetName(L"Hand Item Icon");
    m_pCurrentHandItemIcon = dynamic_cast<CImage*>(handItemIcon->AddComponent(new CImage(L"")));
    CSceneManager::GetInstance().getCurrentscene()->FindObjectOfType<CCanvas>(Layer::UI)->InsertUI(m_pCurrentHandItemIcon);
    m_pCurrentHandItemIcon->getRect().SetSize(vector2::one() * 40.f);
    m_pCurrentHandItemIcon->SetSpriteArea(vector2Int::zero(), vector2Int(32, 32));
    m_pCurrentHandItemIcon->getObj()->SetActive(false);

    CGameObject& blueprint = *m_pGameObject->getScene()->AddObject(L"Blueprint", Layer::Blueprint);
    m_pBlueprint = dynamic_cast<CSpriteRenderer*>(blueprint.AddComponent(new CSpriteRenderer(L"", vector2Int::one() * 32, true)));
    m_pBPL = dynamic_cast<CSpriteRender_Level*>(blueprint.AddComponent(new CSpriteRender_Level(m_pBlueprint)));
    vector2Int start[3];
    start[0] = vector2Int(0, 0);
    start[1] = vector2Int(0, 128);
    start[2] = vector2Int(0, 192);
    vector2Int end[3];
    end[0] = vector2Int::one() * 128;
    end[1] = vector2Int::one() * 64;
    end[2] = vector2Int::one() * 32;
    m_pBPL->SetRenderPos(start, end);
    blueprint.getTransform().SetPosition(m_pGameObject->getTransform().getPosition());
    blueprint.getTransform().SetScale(vector2::one());
    m_pBlueprint->SetColorTint(ColorValue::Green());

    m_pInventory->AddItem(1, 10);
    m_pInventory->AddItem(4, 5);
    m_pInventory->AddItem(0, 20);
    m_pInventory->AddItem(2, 5);
    m_pInventory->AddItem(3, 5);
    m_pInventory->AddItem(15, 100);
    m_pInventory->AddItem(5, 1);
    m_pInventory->AddItem(7, 5);
}

void CPlayer::Reset()
{
	Reset_();
}

void CPlayer::Update()
{
    Update_();

    if (m_fLastHitTime > 0.f)
    {
        m_fLastHitTime -= DELTA_TIME;
        m_pHpBar->getObj()->SetActive(true);
    }
    else
    {
        if (m_sStat.crtHp >= m_sStat.maxHp)
            m_pHpBar->getObj()->SetActive(false);
        m_fLastHitTime = 0.f;
    }

    if (m_fLastHitTime <= 0.f)
        AddHp(m_sStat.recoverHpVlauue * DELTA_TIME);

    if (m_fShootTime < m_sStat.shootInterval)
        m_fShootTime += DELTA_TIME;
    else
    {
        if (CInput::GetInstance().GetKey(SPACE))
        {
            list<CMonster*>& monsterList = CMonsterManager::GetInstance().getMonsterList();
            CMonster* nearMonster = nullptr;
            float nearDist = FLT_MAX;

            for (TRAVERSAL_ITER(monsterList, it))
            {
                float tempDist = vector2::Distance(getTransform().getPosition(), (*it)->getTransform().getPosition());

                if ((*it)->getAlibe() && tempDist <= m_sStat.shootDistance)
                {
                    if (tempDist < nearDist)
                    {
                        nearMonster = (*it);
                        nearDist = tempDist;
                    }
                }
            }

            if (nearMonster)
                m_pShootTarget = nearMonster->getObj();
            else
                m_pShootTarget = nullptr;
        }

        m_fShootTime = 0.f;

        if (m_pShootTarget && m_bIsShooting)
            GunShoot();
    }

    CTileMap& map = *CMapManager::GetInstance().getMainTile();

    vector2 mousePos = CSceneManager::GetInstance().getCurrentscene()->getMainCamera()->GetMousePosToWorld();
    vector2 camPos = CSceneManager::GetInstance().getCurrentscene()->getMainCamera()->getObj()->getTransform().getPosition();

    if ((int)round(getTransform().getPosition().x) < map.getRectSize().x && (int)round(getTransform().getPosition().y) < map.getRectSize().y)
        m_pStayTile = map((int)round(getTransform().getPosition().x), (int)round(getTransform().getPosition().y + 0.75f));
    m_pMouseOverTile = nullptr;

    if (mousePos.x >= 0 && mousePos.x <= map.getRectSize().x - 1 &&
        mousePos.y >= 0 && mousePos.y <= map.getRectSize().y - 1)
        m_pMouseOverTile = map((int)round(mousePos.x), (int)round(mousePos.y));

    if (m_pMouseOverTile)
        m_pOverStructure = m_pMouseOverTile->getHaveStructure();

    if (m_pStayTile)
    {
        // 벨트 타기

        if (m_pStayTile->getHaveStructure() && m_pStayTile->getHaveStructure()->getObj()->GetComponent<CBelt>())
        {
            CBelt* belt = m_pStayTile->getHaveStructure()->getObj()->GetComponent<CBelt>();

            float beltSpd = CBeltManager::GetInstance().getBeltSpeed();

            if (belt->getDirection() == CStructure::RIGHT)
                getTransform().AddPosition(vector2::right() * beltSpd * DELTA_TIME);
            else if (belt->getDirection() == CStructure::DOWN)
                getTransform().AddPosition(vector2::down() * beltSpd * DELTA_TIME);
            else if (belt->getDirection() == CStructure::LEFT)
                getTransform().AddPosition(vector2::left() * beltSpd * DELTA_TIME);
            else if (belt->getDirection() == CStructure::UP)
                getTransform().AddPosition(vector2::up() * beltSpd * DELTA_TIME);
        }
    }

    if (CInput::GetInstance().GetMouseButton(1))
    {
        m_pCurrentCollectingTile = m_pMouseOverTile;
    }
    else
    {
        m_pCurrentCollectingTile = nullptr;
    }

    m_v2Direction = vector2::zero();

    if (CInput::GetInstance().GetKeyDown(Q))
    {
        CloseHand();
        m_pBlueprint->getObj()->SetActive(false);
        m_pInventory->getInvenUI()->AllOffSlot();
        m_pHandItem = nullptr;
        m_pInventory->getInvenUI()->UpdateInventory(m_pInventory->getItemList());
    }

    if (m_pCurrentHandItemIcon->getObj()->isActive())
        m_pCurrentHandItemIcon->getRect().SetPosition(CInput::GetInstance().GetMousePos().toVector2() + vector2::one() * 20.f);

    if (CInput::GetInstance().GetKeyDown(Z))
    {
        if (m_pMouseOverTile && m_pHandItem)
        {
            if (!m_pMouseOverTile->getHaveStructure())
            {
                // 땅에 아이템 놓기
                DropFieldItem(m_pHandItem->getInfo().id, m_pMouseOverTile);
                GetItemHandler();
            }
            else
            {
                if (m_pMouseOverTile->getHaveStructure()->getObj()->GetComponent<CBelt>())
                {
                    // 벨트에 아이템 놓기
                    DropBeltItem(m_pHandItem->getInfo().id, m_pMouseOverTile->getHaveStructure()->getObj()->GetComponent<CBelt>());
                    GetItemHandler();
                }
            }
        }
    }

    if (CInput::GetInstance().GetKeyDown(F))
    {
        if (m_pMouseOverTile)
        {
            if (!m_pMouseOverTile->getHaveStructure())
            {
                // 땅에 아이템 줍기
                for (int i = 0; i < 4; ++i)
                {
                    if (m_pMouseOverTile->getDropItems(i))
                    {
                        m_pInventory->AddItem(m_pMouseOverTile->getDropItems(i)->getItem()->getInfo().id, 1);
                        m_pMouseOverTile->getDropItems(i)->getObj()->Destroy();
                        m_pMouseOverTile->DeleteDropItem(i);
                        GetItemHandler();
                    }
                } 
            }
            else
            {
                if (m_pMouseOverTile->getHaveStructure()->getObj()->GetComponent<CBelt>())
                {
                    // 벨트에 아이템 줍기
                    for (TRAVERSAL_ITER(CBeltManager::GetInstance().getItemList(), it))
                    {
                        if ((*it)->getCrtBelt() && ((*it)->getCrtBelt() == m_pMouseOverTile->getHaveStructure()->getObj()->GetComponent<CBelt>()))
                        {
                            m_pInventory->AddItem((*it)->getItem()->getInfo().id, 1);
                            (*it)->getObj()->Destroy();
                            GetItemHandler();
                        }                           
                    }
                }
            }
        }
    }

    if (CInput::GetInstance().GetKey(W))
        m_v2Direction += vector2::up();
    if (CInput::GetInstance().GetKey(S))
        m_v2Direction += vector2::down();
    if (CInput::GetInstance().GetKey(A))
        m_v2Direction += vector2::left();
    if (CInput::GetInstance().GetKey(D))
        m_v2Direction += vector2::right();

    m_bIsWalk = (m_v2Direction != vector2::zero());

    if (m_bIsWalk && !m_bIsShooting)
    {
        if (m_v2Direction == vector2::up())
        {
            m_pPlayerAnim->SetAnimation(L"Player_Running_Up");
            m_pShadowAnim->SetAnimation(L"Shadow_Run_Up");
        }
        else if (m_v2Direction == vector2(1, -1))
        {
            m_pPlayerAnim->SetAnimation(L"Player_Running_Up_Right");
            m_pShadowAnim->SetAnimation(L"Shadow_Run_Up_Right");
        }
        else if (m_v2Direction == vector2::right())
        {
            m_pPlayerAnim->SetAnimation(L"Player_Running_Right");
            m_pShadowAnim->SetAnimation(L"Shadow_Run_Right");

        }
        else if (m_v2Direction == vector2(1, 1))
        {
            m_pPlayerAnim->SetAnimation(L"Player_Running_Right_Down");
            m_pShadowAnim->SetAnimation(L"Shadow_Run_Right_Down");

        }
        else if (m_v2Direction == vector2::down())
        {
            m_pPlayerAnim->SetAnimation(L"Player_Running_Down");
            m_pShadowAnim->SetAnimation(L"Shadow_Run_Down");
        }
        else if (m_v2Direction == vector2(-1, 1))
        {
            m_pPlayerAnim->SetAnimation(L"Player_Running_Left_Down");
            m_pShadowAnim->SetAnimation(L"Shadow_Run_Left_Down");
        }
        else if (m_v2Direction == vector2::left())
        {
            m_pPlayerAnim->SetAnimation(L"Player_Running_Left");
            m_pShadowAnim->SetAnimation(L"Shadow_Run_Left");
        }
        else if (m_v2Direction == vector2(-1, -1))
        {
            m_pPlayerAnim->SetAnimation(L"Player_Running_Left_Up");
            m_pShadowAnim->SetAnimation(L"Shadow_Run_Left_Up");
        }

        m_v2PrevDirection = m_v2Direction;
    }

    if (m_bIsMining)
    {
        if (CInput::GetInstance().GetMouseButtonUp(1) || m_bIsWalk)
            CancelMining();

        if (m_pPlayerAnim->getCurrentAnimationFrame() == 20 || m_pPlayerAnim->getCurrentAnimationFrame() == 21)
        {
            m_pSoundObj->SetClip(L"SFX_PlayerMining1");
            m_pSoundObj->Play();
        }
    }

    //if (m_v2Direction != vector2::zero())
    //{
    //    getTransform().Translate(m_v2Direction.normalize() * m_fMoveSpeed * DELTA_TIME);
    //    //getTransform().getPosition() += m_v2Direction.normalize() * m_fMoveSpeed * DELTA_TIME;
    //}
    //else
    //    getTransform().Reset();

    //if (m_pShootTarget)
        //m_pShootTarget->getTransform().SetPosition(m_pGameObject->getScene()->getMainCamera()->GetMousePosToWorld());

    m_pGunMuzzleEffect->getObj()->SetActive(m_bIsShooting);

    if (CInput::GetInstance().GetKey(SPACE) && !m_bIsMining)
    {
        if (m_pShootTarget)
        {
            getTransform().SetScale(vector2(1.75f, 2.2f));
            m_sStat.moveSpeed = 4.5f;
            m_bIsShooting = true;

            vector2 myPos = getTransform().getPosition();
            vector2 targetPos = m_pShootTarget->getTransform().getPosition();

            if (fabs(myPos.x - targetPos.x) < 1.f)
            {
                m_v2ShootDirection.x = 0;
            }
            else if (myPos.x < targetPos.x)
            {
                m_v2ShootDirection.x = 1;
            }
            else
            {
                m_v2ShootDirection.x = -1;
            }

            if (fabs(myPos.y - targetPos.y) < 1.f)
            {
                m_v2ShootDirection.y = 0;
            }
            else if (myPos.y < targetPos.y)
            {
                m_v2ShootDirection.y = 1;
            }
            else
            {
                m_v2ShootDirection.y = -1;
            }

            if (m_v2ShootDirection == vector2Int::up())
            {
                m_pGunMuzzleEffect->getTransform().SetChildPosition(m_v2ShootDirection.toVector2() * 1.3f);
                // 완
                if (!m_bIsWalk)
                {
                    m_pPlayerAnim->SetAnimation(L"Gun_Idle_0");
                }
                else
                {
                    if (m_v2Direction == vector2::up())
                    {
                        m_pPlayerAnim->SetAnimation(L"Gun_Run_0");
                    }
                    else if (m_v2Direction == vector2::down())
                    {
                        m_pPlayerAnim->SetAnimation(L"Gun_Run_Flip_0");
                    }
                    else if (m_v2Direction == vector2(1, -1))
                    {
                        m_pPlayerAnim->SetAnimation(L"Gun_Run_1");
                    }
                    else if (m_v2Direction == vector2(-1, -1))
                    {
                        m_pPlayerAnim->SetAnimation(L"Gun_Run_19");
                    }
                    else if (m_v2Direction == vector2(1, 1))
                    {
                        m_pPlayerAnim->SetAnimation(L"Gun_Run_Flip_19");
                    }
                    else if (m_v2Direction == vector2(-1, 1))
                    {
                        m_pPlayerAnim->SetAnimation(L"Gun_Run_Flip_1");
                    }
                    else if (m_v2Direction == vector2::right())
                    {
                        m_pPlayerAnim->SetAnimation(L"Gun_Run_2");
                    }
                    else if (m_v2Direction == vector2::left())
                    {
                        m_pPlayerAnim->SetAnimation(L"Gun_Run_20");
                    }
                }
            }
            else if (m_v2ShootDirection == vector2Int(1, -1))
            {
                // 완
                if (!m_bIsWalk)
                {
                    m_pGunMuzzleEffect->getTransform().SetChildPosition((m_v2ShootDirection.toVector2().normalize() + vector2(-0.1f, -0.1f)) * 1.3f);
                    m_pPlayerAnim->SetAnimation(L"Gun_Idle_1");
                }
                else
                {
                    m_pGunMuzzleEffect->getTransform().SetChildPosition((m_v2ShootDirection.toVector2().normalize() + vector2(-0.3f, -0.1f)) * 1.3f);

                    if (m_v2Direction == vector2::right())
                    {
                        m_pPlayerAnim->SetAnimation(L"Gun_Run_3");
                    }
                    else if (m_v2Direction == vector2::left())
                    {
                        m_pPlayerAnim->SetAnimation(L"Gun_Run_Flip_3");
                    }
                    else if (m_v2Direction == vector2::up())
                    {
                        m_pPlayerAnim->SetAnimation(L"Gun_Run_4");
                    }
                    else if (m_v2Direction == vector2(1, -1))
                    {
                        m_pPlayerAnim->SetAnimation(L"Gun_Run_5");
                    }
                    else if (m_v2Direction == vector2(-1, 1))
                    {
                        m_pPlayerAnim->SetAnimation(L"Gun_Run_Flip_5");
                    }
                    else if (m_v2Direction == vector2(1, -1))
                    {
                        m_pPlayerAnim->SetAnimation(L"Gun_Run_Flip_5");
                    }
                    else if (m_v2Direction == vector2(-1, -1))
                    {
                        m_pPlayerAnim->SetAnimation(L"Gun_Run_Flip_3");
                    }
                    else if (m_v2Direction == vector2(1, 1))
                    {
                        m_pPlayerAnim->SetAnimation(L"Gun_Run_3");
                    }
                    else if (m_v2Direction == vector2::down())
                    {
                        m_pPlayerAnim->SetAnimation(L"Gun_Run_Flip_4");
                    }
                }
            }
            else if (m_v2ShootDirection == vector2Int::right())
            {
                m_pGunMuzzleEffect->getTransform().SetChildPosition((m_v2ShootDirection.toVector2() * 0.8f + vector2::up() * 0.5f));

                // 완
                if (!m_bIsWalk)
                {
                    m_pPlayerAnim->SetAnimation(L"Gun_Idle_2");
                }
                else
                {
                    if (m_v2Direction == vector2::up())
                    {
                        m_pPlayerAnim->SetAnimation(L"Gun_Run_Flip_7");
                    }
                    else if (m_v2Direction == vector2::down())
                    {
                        m_pPlayerAnim->SetAnimation(L"Gun_Run_7");
                    }
                    else if (m_v2Direction == vector2(1, -1))
                    {
                        m_pPlayerAnim->SetAnimation(L"Gun_Run_8");
                    }
                    else if (m_v2Direction == vector2(-1, -1))
                    {
                        m_pPlayerAnim->SetAnimation(L"Gun_Run_Flip_10");
                    }
                    else if (m_v2Direction == vector2(1, 1))
                    {
                        m_pPlayerAnim->SetAnimation(L"Gun_Run_10");
                    }
                    else if (m_v2Direction == vector2(-1, 1))
                    {
                        m_pPlayerAnim->SetAnimation(L"Gun_Run_Flip_8");
                    }
                    else if (m_v2Direction == vector2::right())
                    {
                        m_pPlayerAnim->SetAnimation(L"Gun_Run_9");
                    }
                    else if (m_v2Direction == vector2::left())
                    {
                        m_pPlayerAnim->SetAnimation(L"Gun_Run_Flip_9");
                    }
                }
            }
            else if (m_v2ShootDirection == vector2Int(1, 1))
            {
                // 완
                if (!m_bIsWalk)
                {
                    m_pGunMuzzleEffect->getTransform().SetChildPosition((m_v2ShootDirection.toVector2().normalize() + vector2(-0.25f, -0.75f)) * 1.3f);
                    m_pPlayerAnim->SetAnimation(L"Gun_Idle_3");
                }
                else
                {
                    m_pGunMuzzleEffect->getTransform().SetChildPosition((m_v2ShootDirection.toVector2().normalize() + vector2(-0.3f, -0.75f)) * 1.3f);

                    if (m_v2Direction == vector2(1, -1))
                    {
                        m_pPlayerAnim->SetAnimation(L"Gun_Run_Flip_11");
                    }
                    else if (m_v2Direction == vector2(-1, 1))
                    {
                        m_pPlayerAnim->SetAnimation(L"Gun_Run_11");
                    }
                    else if (m_v2Direction == vector2::right())
                    {
                        m_pPlayerAnim->SetAnimation(L"Gun_Run_12");
                    }
                    else if (m_v2Direction == vector2::left())
                    {
                        m_pPlayerAnim->SetAnimation(L"Gun_Run_Flip_12");
                    }
                    else if (m_v2Direction == vector2(1, 1))
                    {
                        m_pPlayerAnim->SetAnimation(L"Gun_Run_13");
                    }
                    else if (m_v2Direction == vector2(-1, -1))
                    {
                        m_pPlayerAnim->SetAnimation(L"Gun_Run_Flip_13");
                    }
                    else if (m_v2Direction == vector2::up())
                    {
                        m_pPlayerAnim->SetAnimation(L"Gun_Run_14");
                    }
                    else if (m_v2Direction == vector2::down())
                    {
                        m_pPlayerAnim->SetAnimation(L"Gun_Run_Flip_14");
                    }
                }
            }
            else if (m_v2ShootDirection == vector2Int::down())
            {
                m_pGunMuzzleEffect->getTransform().SetChildPosition((m_v2ShootDirection.toVector2() * 0.8f + vector2(-0.1f , -0.75f)));

                // 완
                if (!m_bIsWalk)
                {
                    m_pPlayerAnim->SetAnimation(L"Gun_Idle_4");
                }
                else
                {
                    if (m_v2Direction == vector2::right())
                    {
                        m_pPlayerAnim->SetAnimation(L"Gun_Run_15");
                    }
                    else if (m_v2Direction == vector2::left())
                    {
                        m_pPlayerAnim->SetAnimation(L"Gun_Run_Flip_15");
                    }
                    else if (m_v2Direction == vector2(1, 1))
                    {
                        m_pPlayerAnim->SetAnimation(L"Gun_Run_16");
                    }
                    else if (m_v2Direction == vector2(-1, -1))
                    {
                        m_pPlayerAnim->SetAnimation(L"Gun_Run_Flip_16");
                    }
                    else if (m_v2Direction == vector2::down())
                    {
                        m_pPlayerAnim->SetAnimation(L"Gun_Run_17");
                    }
                    else if (m_v2Direction == vector2::up())
                    {
                        m_pPlayerAnim->SetAnimation(L"Gun_Run_Flip_17");
                    }
                    else if (m_v2Direction == vector2(-1, 1))
                    {                        
                        m_pPlayerAnim->SetAnimation(L"Gun_Run_34");
                    }
                    else if (m_v2Direction == vector2(-1, -1))
                    {
                        m_pPlayerAnim->SetAnimation(L"Gun_Run_34");
                    }
                    else if (m_v2Direction == vector2(1, -1))
                    {
                        m_pPlayerAnim->SetAnimation(L"Gun_Run_Flip_34");
                    }
                }
            }
            else if (m_v2ShootDirection == vector2Int(-1, 1))
            {
                // 완
                if (!m_bIsWalk)
                {
                    m_pGunMuzzleEffect->getTransform().SetChildPosition((m_v2ShootDirection.toVector2() * 0.8f + vector2(0.f, -0.85f)));
                    m_pPlayerAnim->SetAnimation(L"Gun_Idle_5");
                }
                else
                {
                    m_pGunMuzzleEffect->getTransform().SetChildPosition((m_v2ShootDirection.toVector2() * 0.8f + vector2(0.3f, -0.95f)));

                    if (m_v2Direction == vector2(1, 1))
                    {
                        m_pPlayerAnim->SetAnimation(L"Gun_Run_29");
                    }
                    else if (m_v2Direction == vector2::left())
                    {
                        m_pPlayerAnim->SetAnimation(L"Gun_Run_30");
                    }
                    else if (m_v2Direction == vector2::right())
                    {
                        m_pPlayerAnim->SetAnimation(L"Gun_Run_Flip_30");
                    }
                    else if (m_v2Direction == vector2(-1, 1))
                    {
                        m_pPlayerAnim->SetAnimation(L"Gun_Run_31");
                    }
                    else if (m_v2Direction == vector2(1, -1))
                    {
                        m_pPlayerAnim->SetAnimation(L"Gun_Run_Flip_31");
                    }
                    else if (m_v2Direction == vector2::up())
                    {
                        m_pPlayerAnim->SetAnimation(L"Gun_Run_32");
                    }
                    else if (m_v2Direction == vector2::down())
                    {
                        m_pPlayerAnim->SetAnimation(L"Gun_Run_Flip_32");
                    }
                }
            }
            else if (m_v2ShootDirection == vector2Int::left())
            {
                m_pGunMuzzleEffect->getTransform().SetChildPosition((m_v2ShootDirection.toVector2() * 0.8f + vector2::up() * 0.55f));

                // 완
                if (!m_bIsWalk)
                {
                    m_pPlayerAnim->SetAnimation(L"Gun_Idle_6");
                }
                else
                {
                    if (m_v2Direction == vector2::up())
                    {
                        m_pPlayerAnim->SetAnimation(L"Gun_Run_25");
                    }
                    else if (m_v2Direction == vector2::down())
                    {
                        m_pPlayerAnim->SetAnimation(L"Gun_Run_Flip_25");
                    }
                    else if (m_v2Direction == vector2(-1, -1))
                    {
                        m_pPlayerAnim->SetAnimation(L"Gun_Run_26");
                    }
                    else if (m_v2Direction == vector2(1, 1))
                    {
                        m_pPlayerAnim->SetAnimation(L"Gun_Run_Flip_26");
                    }
                    else if (m_v2Direction == vector2::left())
                    {
                        m_pPlayerAnim->SetAnimation(L"Gun_Run_27");
                    }
                    else if (m_v2Direction == vector2::right())
                    {
                        m_pPlayerAnim->SetAnimation(L"Gun_Run_Flip_27");
                    }
                    else if (m_v2Direction == vector2(-1, 1))
                    {
                        m_pPlayerAnim->SetAnimation(L"Gun_Run_28");
                    }
                    else if (m_v2Direction == vector2(1, -1))
                    {
                        m_pPlayerAnim->SetAnimation(L"Gun_Run_Flip_28");
                    }
                }
            }
            else if (m_v2ShootDirection == vector2Int(-1, -1))
            {
                m_pGunMuzzleEffect->getTransform().SetChildPosition((m_v2ShootDirection.toVector2() * 0.8f + vector2(0.15f, -0.3f)));
                // 완
                if (!m_bIsWalk)
                {
                    m_pPlayerAnim->SetAnimation(L"Gun_Idle_7");
                }
                else
                {
                    if (m_v2Direction == vector2(-1, 1))
                    {
                        m_pPlayerAnim->SetAnimation(L"Gun_Run_21");
                    }
                    else if (m_v2Direction == vector2(1, -1))
                    {
                        m_pPlayerAnim->SetAnimation(L"Gun_Run_Flip_21");
                    }
                    else if (m_v2Direction == vector2::up())
                    {
                        m_pPlayerAnim->SetAnimation(L"Gun_Run_22");
                    }
                    else if (m_v2Direction == vector2::down())
                    {
                        m_pPlayerAnim->SetAnimation(L"Gun_Run_Flip_22");
                    }
                    else if (m_v2Direction == vector2(-1, -1))
                    {
                        m_pPlayerAnim->SetAnimation(L"Gun_Run_23");
                    }
                    else if (m_v2Direction == vector2(1, 1))
                    {
                        m_pPlayerAnim->SetAnimation(L"Gun_Run_Flip_23");
                    }
                    else if (m_v2Direction == vector2::left())
                    {
                        m_pPlayerAnim->SetAnimation(L"Gun_Run_24");
                    }
                    else if (m_v2Direction == vector2::right())
                    {
                        m_pPlayerAnim->SetAnimation(L"Gun_Run_Flip_24");
                    }
                }
            }
        }
    }
    else if (!m_bIsMining)
    {
        getTransform().SetScale(vector2(1.5f, 2.f));
        m_sStat.moveSpeed = 6.f;
    }

    if (m_bIsShooting && CInput::GetInstance().GetKeyUp(SPACE))
    {
        m_v2Direction = m_v2PrevDirection = m_v2ShootDirection.toVector2();
        m_bIsShooting = false; 
    }

    if (!m_bIsWalk && !m_bIsMining && !m_bIsShooting)
    {
        if (m_v2PrevDirection == vector2::up())
        {
            m_pPlayerAnim->SetAnimation(L"Player_Idle_Up");
            //m_pPlayerMaskAnim->SetAnimation(L"Mask_Idle_Up");
            m_pShadowAnim->SetAnimation(L"Shadow_Idle_Up");
        }
        else if (m_v2PrevDirection == vector2(1, -1))
        {
            m_pPlayerAnim->SetAnimation(L"Player_Idle_Up_Right");
            //m_pPlayerMaskAnim->SetAnimation(L"Mask_Idle_Up_Right");
            m_pShadowAnim->SetAnimation(L"Shadow_Idle_Up_Right");
        }
        else if (m_v2PrevDirection == vector2::right())
        {
            m_pPlayerAnim->SetAnimation(L"Player_Idle_Right");
            //m_pPlayerMaskAnim->SetAnimation(L"Mask_Idle_Right");
            m_pShadowAnim->SetAnimation(L"Shadow_Idle_Right");
        }
        else if (m_v2PrevDirection == vector2(1, 1))
        {
            m_pPlayerAnim->SetAnimation(L"Player_Idle_Right_Down");
            //m_pPlayerMaskAnim->SetAnimation(L"Mask_Idle_Right_Down");
            m_pShadowAnim->SetAnimation(L"Shadow_Idle_Right_Down");
        }
        else if (m_v2PrevDirection == vector2::down())
        {
            m_pPlayerAnim->SetAnimation(L"Player_Idle_Down");
            //m_pPlayerMaskAnim->SetAnimation(L"Mask_Idle_Down");
            m_pShadowAnim->SetAnimation(L"Shadow_Idle_Down");
        }
        else if (m_v2PrevDirection == vector2(-1, 1))
        {
            if (m_bIsShooting)
            {
                m_pPlayerAnim->SetAnimation(L"Gun_Idle_Left_Down");
            }
            else
            {
                m_pPlayerAnim->SetAnimation(L"Player_Idle_Left_Down");
                //m_pPlayerMaskAnim->SetAnimation(L"Mask_Idle_Left_Down");
                m_pShadowAnim->SetAnimation(L"Shadow_Idle_Left_Down");
            }
        }
        else if (m_v2PrevDirection == vector2::left())
        {
            if (m_bIsShooting)
            {
                m_pPlayerAnim->SetAnimation(L"Gun_Idle_Left");
            }
            else
            {
                m_pPlayerAnim->SetAnimation(L"Player_Idle_Left");
                //m_pPlayerMaskAnim->SetAnimation(L"Mask_Idle_Left");
                m_pShadowAnim->SetAnimation(L"Shadow_Idle_Left");
            }
        }
        else if (m_v2PrevDirection == vector2(-1, -1))
        {
            if (m_bIsShooting)
            {
                m_pPlayerAnim->SetAnimation(L"Gun_Idle_Left_Up");
            }
            else
            {
                m_pPlayerAnim->SetAnimation(L"Player_Idle_Left_Up");
                //m_pPlayerMaskAnim->SetAnimation(L"Mask_Idle_Left_Up");
                m_pShadowAnim->SetAnimation(L"Shadow_Idle_Left_Up");
            }
        }
    }

    if (m_bIsMining && !m_bIsShooting)
    {
        if (m_v2MiningDirection == vector2::up())
        {
            m_pPlayerAnim->SetAnimation(L"Player_Mining_Up");
            m_pShadowAnim->SetAnimation(L"Shadow_Mining_Up");
        }
        else if (m_v2MiningDirection == vector2(1, -1))
        {
            m_pPlayerAnim->SetAnimation(L"Player_Mining_Up_Right");
            m_pShadowAnim->SetAnimation(L"Shadow_Mining_Up_Right");
        }
        else if (m_v2MiningDirection == vector2::right())
        {
            m_pPlayerAnim->SetAnimation(L"Player_Mining_Right");
            m_pShadowAnim->SetAnimation(L"Shadow_Mining_Right");
        }
        else if (m_v2MiningDirection == vector2(1, 1))
        {
            m_pPlayerAnim->SetAnimation(L"Player_Mining_Right_Down");
            m_pShadowAnim->SetAnimation(L"Shadow_Mining_Right_Down");
        }
        else if (m_v2MiningDirection == vector2::down())
        {
            m_pPlayerAnim->SetAnimation(L"Player_Mining_Down");
            m_pShadowAnim->SetAnimation(L"Shadow_Mining_Down");
        }
        else if (m_v2MiningDirection == vector2(-1, 1))
        {
            m_pPlayerAnim->SetAnimation(L"Player_Mining_Left_Down");
            m_pShadowAnim->SetAnimation(L"Shadow_Mining_Left_Down");
        }
        else if (m_v2MiningDirection == vector2::left())
        {
            m_pPlayerAnim->SetAnimation(L"Player_Mining_Left");
            m_pShadowAnim->SetAnimation(L"Shadow_Mining_Left");
        }
        else if (m_v2MiningDirection == vector2(-1, -1))
        {
            m_pPlayerAnim->SetAnimation(L"Player_Mining_Left_Up");
            m_pShadowAnim->SetAnimation(L"Shadow_Mining_Left_Up");
        }

        if (m_pCurrentCollectingTile && m_pCurrentCollectingTile->getObj()->GetComponent<CResourceTile>())
        {
            m_pCurrentCollectingTile->getObj()->GetComponent<CResourceTile>()->AddHp(-50.f * DELTA_TIME);
            float hp = (float)m_pCurrentCollectingTile->getObj()->GetComponent<CResourceTile>()->getHp();
            m_pCollectingGauge->SetFillAmount(vector2((100.f - hp) / 100.f, 1.f));
        }
    }

    if (m_pCurrentCollectingTile)
    {
        if (m_pOverStructure)
        {
            m_pOverStructure->AddCollectingHp(-400.f * DELTA_TIME);
            float hp = m_pOverStructure->getCollectingHp();
            m_pCollectingGauge->SetFillAmount(vector2((100.f - hp) / 100.f, 1.f));
        }
    }

    m_pCollectingGaugeBar->SetActive(m_pCurrentCollectingTile && (m_pOverStructure || m_bIsMining));

    for (int i = 0; i < m_vCreatingSlotList.size(); ++i)
    {
        m_vCreatingSlotList[i]->getObj()->SetActive(false);
    }

    if (m_vCreateItemQueue.size() > 0)
    {
        for (int i = 0; i < m_vCreateItemQueue.size(); ++i)
        {
            if (i < 4 && !m_vCreatingSlotList[i]->getObj()->isActive())
            {
                m_vCreatingSlotList[i]->getObj()->SetActive(true);
                m_vCreatingSlotList[i]->SetItem(*m_vCreateItemQueue.front());
                m_vCreatingSlotList.front()->SetStart(true);
            }
        }
    }

    if (m_pHandItem)
    {
        vector2 mousePos = m_pGameObject->getScene()->getMainCamera()->GetMousePosToWorld();
        vector2 bpPos = vector2(round(mousePos.x), round(mousePos.y)) - vector2(m_pHandItem->getInfo().fieldSize.x / 2, m_pHandItem->getInfo().fieldSize.y / 2) + vector2(m_pHandItem->getInfo().fieldSize.x, m_pHandItem->getInfo().fieldSize.y);
        bpPos -= vector2::one() * 0.5f;
        m_pBlueprint->getTransform().SetPosition(bpPos);

        if (m_pHandItem->getInfo().isStructure)
        {
            if (m_pBlueprint->getObj()->isActive() && CInput::GetInstance().GetKeyDown(R))
            {
                if (m_eCrtStructureDir == CStructure::RIGHT)
                    m_eCrtStructureDir = CStructure::DOWN;
                else if (m_eCrtStructureDir == CStructure::DOWN)
                    m_eCrtStructureDir = CStructure::LEFT;
                else if (m_eCrtStructureDir == CStructure::LEFT)
                    m_eCrtStructureDir = CStructure::UP;
                else if (m_eCrtStructureDir == CStructure::UP)
                    m_eCrtStructureDir = CStructure::RIGHT;

                if (m_iEightDirection == 0)
                    m_iEightDirection = 3;
                else if (m_iEightDirection == 3)
                    m_iEightDirection = 1;
                else if (m_iEightDirection == 1)
                    m_iEightDirection = 2;
                else if (m_iEightDirection == 2)
                    m_iEightDirection = 4;
                else if (m_iEightDirection > 3)
                    ++m_iEightDirection;

                if (m_iEightDirection > 11)
                    m_iEightDirection = 0;

                OpenBlueprint();
            }

            AbleHideHandle(false);

            m_pBlueprint->getObj()->SetActive(true);

            wstring itemName = L"";

            float dist = vector2Int::Distance(m_pStayTile->getTransform().getPosition().toVector2Int(), mousePos.toVector2Int());

            bool result = true;

            if (!m_pInventory->getInvenUI()->getUI()->getObj()->isActive() || !m_pInventory->getInvenUI()->getUI()->getOnMouse())
            {
                if (dist > 8.f)
                {
                    m_pBlueprint->SetColorTint(ColorValue::Red());

#ifdef _DEBUG

                    if (CInput::GetInstance().GetMouseButtonDown(0))
                        CConsole::Print("설치 실패: 도달할 수  없습니다.");
#endif // _DEBUG
                    result = false;
                }
                else
                {
                    if (m_pMouseOverTile)
                    {
                        if (m_pMouseOverTile->getHaveStructure())
                        {
                            itemName = m_pMouseOverTile->getHaveStructure()->getItem()->getInfo().itemName;

                            m_pBlueprint->SetColorTint(ColorValue::Red());
                            result = false;
                        }
                        else
                        {
                            CTileMap& map = *CMapManager::GetInstance().getMainTile();

                            vector2Int startPos = m_pMouseOverTile->getIndex();

                            for (int x = startPos.x; x < startPos.x + (int)m_pHandItem->getInfo().fieldSize.x; ++x)
                            {
                                for (int y = startPos.y; y < startPos.y + (int)m_pHandItem->getInfo().fieldSize.y; ++y)
                                {
                                    if (x < map.getRectSize().x && y < map.getRectSize().y)
                                    {
                                        if (map(x, y)->getHaveStructure())
                                        {
                                            itemName = map(x, y)->getHaveStructure()->getItem()->getInfo().itemName;
                                            result = false;
                                        }
                                    }
                                    else
                                        result = false;
                                }
                            }
                        }

                        if (result)
                        {
                            m_pBlueprint->SetColorTint(ColorValue::Green());

                            if (CInput::GetInstance().GetMouseButton(0))
                            {
                                CreateStructure(m_pHandItem, m_pMouseOverTile, m_eCrtStructureDir);
                                m_bCreatedCStructure = true;
                            }
                        }
                        else
                        {
#ifdef _DEBUG
                            if (CInput::GetInstance().GetMouseButtonDown(0))
                                CConsole::Print("설치 실패: " + WStringToString(itemName) + "가 가로막고 있어 설치할 수 없습니다.");

#endif // _DEBUG
                            m_pBlueprint->SetColorTint(ColorValue::Red());
                        }
                    }
                }
            }
        }
        else
        {
            AbleHideHandle(true);
        }
    }
    else
        m_pBlueprint->getObj()->SetActive(false);

    // IO 인벤토리 열기
    if (!m_bCreatedCStructure && m_pOverStructure && m_pOverStructure->getObj()->GetComponent<CInOutStructure>())
    {
        if (!m_pInventory->getInvenUI()->getObj()->isActive())
        {
            if (CInput::GetInstance().GetMouseButton(0))
            {
                m_pInventory->getInvenUI()->SetSideType(CInventoryUI::IOStructureMenu);
                m_pInventory->getInvenUI()->SetIO(m_pOverStructure->getObj()->GetComponent<CInOutStructure>());
                m_pInventory->getInvenUI()->getObj()->SetActive(true);

#ifdef _DEBUG
                CConsole::Print(L"Open IOUI: " + m_pOverStructure->getObj()->GetComponent<CInOutStructure>()->getObj()->getName());
#endif
            }
        }
    }
}

void CPlayer::FixedUpdate()
{
	FixedUpdate_();

    // 이동
    if (m_v2Direction != vector2::zero())
    {
        getTransform().AddPosition(m_v2Direction.normalize() * m_sStat.moveSpeed * DELTA_TIME);
    }
    else
        getTransform().Reset();
}

void CPlayer::LateUpdate()
{
    LateUpdate_();

    if (!CInput::GetInstance().GetMouseButton(0))
        m_bCreatedCStructure = false;
}

void CPlayer::OnEnable()
{
	OnEnable_();

    m_bIsWalk = false;
    CancelMining();
}

void CPlayer::OnDisable()
{
	OnDisable_();

    m_bIsWalk = false;
    CancelMining();
}

void CPlayer::Render(HDC _hDC)
{
}

void CPlayer::OnCollisionEnter(CCollider* _other)
{
    OnCollisionEnter_(_other);
}

void CPlayer::OnCollisionStay(CCollider* _other)
{
    OnCollisionStay_(_other);
}

void CPlayer::OnCollisionExit(CCollider* _other)
{
    OnCollisionExit_(_other);
}

void CPlayer::OnDestroy()
{
	OnDestroy_();
}

void CPlayer::OnMouseEnter()
{
}

void CPlayer::OnMouseOver()
{
}

void CPlayer::OnMouseExit()
{
}

void CPlayer::Mining(CTile* _tile)
{
    if (m_bIsMining || m_pOverStructure)
        return;

    m_bIsMining = true;

    vector2 direction = (getTransform().getPosition() - _tile->getTransform().getPosition()).normalize();
    m_v2MiningDirection = vector2(-round(direction.x), -round(direction.y));
    m_v2PrevDirection = m_v2MiningDirection;
    m_pGameObject->getTransform().SetScale(vector2(3.5f, 3.f));
    if (m_pShadowAnim)
        m_pShadowAnim->getTransform().SetScale(vector2(3.5f, 1.5f));

    m_pCurrentCollectingTile = _tile;

    m_pCollectingGaugeBar->SetActive(true);
}

void CPlayer::CancelMining()
{
    if (!m_bIsMining)
        return;

    m_bIsMining = false;
    m_pGameObject->getTransform().SetScale(vector2(1.5f, 2.f));

    if (m_pShadowAnim)
        m_pShadowAnim->getTransform().SetScale(vector2(2.f, 1.f));
}

void CPlayer::CpatureStructure(CTile* _tile)
{
    if (m_bCaptuering)
        return;

    m_bCaptuering = true;

    m_pCurrentCollectingTile = _tile;

    m_pCollectingGaugeBar->SetActive(true);
}

void CPlayer::CancelCapture()
{
}

void CPlayer::AddHp(float _dmg)
{
    m_sStat.crtHp += _dmg;

    if (m_sStat.crtHp < 0)
        m_sStat.crtHp = 0;
    else if (m_sStat.crtHp > m_sStat.maxHp)
        m_sStat.crtHp = m_sStat.maxHp;

    m_pHpGauge->SetFillAmountX(m_sStat.crtHp / m_sStat.maxHp);

    if (_dmg < 0.f)
        m_fLastHitTime = 5.f;
}

void CPlayer::OpenHandle(CItem* _item)
{
    if (!_item)
        return;

    m_pHandItem = _item;
    m_pCurrentHandItemIcon->SetSprite(m_pHandItem->getInfo().icon_Slot);
    m_pCurrentHandItemIcon->getRect().SetPosition(CInput::GetInstance().GetMousePos().toVector2());
    m_pCurrentHandItemIcon->getObj()->SetActive(m_pInventory->getInvenUI()->getObj()->isActive());
}

void CPlayer::AbleHideHandle(const bool _value)
{
    m_pCurrentHandItemIcon->getObj()->SetActive(_value);
}

void CPlayer::OpenBlueprint()
{
    vector2Int start[3];
    vector2Int end[3];

    if (m_pHandItem->getInfo().isStructure)
    {
        m_pBPL->SetEnable(true);

        start[0] = vector2Int(0, 0);
        start[1] = vector2Int(0, 64 * (int)m_pHandItem->getInfo().fieldSize.x);
        start[2] = vector2Int(0, 96 * (int)m_pHandItem->getInfo().fieldSize.x);
        end[2] = vector2Int::one() * 16 * (int)m_pHandItem->getInfo().fieldSize.y;
        end[0] = vector2Int::one() * 64 * (int)m_pHandItem->getInfo().fieldSize.y;
        end[1] = vector2Int::one() * 32 * (int)m_pHandItem->getInfo().fieldSize.y;
        m_pBPL->SetRenderPos(start, end);
    }
    else
    {
        m_pBlueprint->SetSpriteArea(vector2Int::zero(), vector2Int::one() * 32);
        m_pBPL->SetEnable(false);
    }

    if (m_pHandItem == nullptr)
        return;

    CItem& item = *CItemManager::GetInstance().GetItem(m_pHandItem->getInfo().id);

    if (&item)
    {
        m_pBlueprint->getTransform().SetPivot(vector2::zero());
        m_pBlueprint->SetSprite(item.getInfo().icon_Field);
        m_pBlueprint->getTransform().SetScale(item.getInfo().fieldSize);
        m_pBlueprint->SetSpriteArea(vector2Int(0, 0), item.getInfo().blueprintSpriteArea);
        m_pBlueprint->getObj()->SetActive(true);

        if (m_pHandItem->getInfo().id == 15)
        {
            m_pBPL->SetEnable(false);
            m_pBlueprint->SetSprite(L"Belt_Move");
            m_pBlueprint->SetSpriteArea(vector2Int(0, 64 * m_iEightDirection), vector2Int::one() * 64);

            if (m_eCrtStructureDir == CStructure::RIGHT || m_eCrtStructureDir == CStructure::LEFT)
                m_pBlueprint->getTransform().SetScale(vector2(2.f, 1.8f));
            else
                m_pBlueprint->getTransform().SetScale(vector2(1.8f, 2.f));
        }
    }
}

void CPlayer::HideBlueprint()
{
    m_pBlueprint->getObj()->SetActive(false);
}

void CPlayer::CloseHand()
{
    m_pHandItem = nullptr;
    m_pCurrentHandItemIcon->getObj()->SetActive(false);
    m_pCurrentHandItemIcon->SetSprite(L"");

    m_pBlueprint->getObj()->SetActive(false);
    m_pBlueprint->SetSprite(L"");
}

void CPlayer::ChangeHandleSprite(wstring _sprite, vector2Int _start, vector2Int _end)
{
    m_pCurrentHandItemIcon->SetSprite(_sprite);
    m_pCurrentHandItemIcon->SetSpriteArea(_start, _end);
}

void CPlayer::AddCreateItemQueue(CItem* _item)
{
    vector<IMaterial> need = _item->getMaterialsAll();

    for (size_t i = 0; i < need.size(); ++i)
    {
        m_pInventory->SubItem(need[i].item, need[i].count);
    }

    m_vCreateItemQueue.push(_item);
}

void CPlayer::CreateStructure(CItem* _item, CTile* _tile, CStructure::StructureDir _dir)
{
    if (!m_bCanCreateStructure)
        return;

    CGameObject* newStructure = m_pGameObject->getScene()->AddObject(L"Struct_" + _item->getInfo().itemName + L"(" + to_wstring(_tile->getIndex().x) +  L"," + to_wstring(_tile->getIndex().y) + L")", Layer::Structure);
    CStructure* structure = nullptr;
    
    if (!_item)
        return;

    if (!&_item->getInfo())
        return;

    switch (_item->getInfo().id)
    {
    case 4:
        structure = dynamic_cast<CStructure*>(newStructure->AddComponent(new CStoneFurnace()));
        break;
    case 5:
        structure = dynamic_cast<CStructure*>(newStructure->AddComponent(new CBurnerDrill()));
        break;
    case 15:
        structure = dynamic_cast<CStructure*>(newStructure->AddComponent(new CBelt()));
        break;
    default:
        break;
    }

    if (!structure)
        return;

    const int id = CItemManager::GetInstance().GetItem(_item->getInfo().id)->getInfo().id;

    structure->SetItem(CItemManager::GetInstance().GetItem(_item->getInfo().id));

    if (structure->getDirType() == CStructure::StructureDirType::RLUD)
        structure->SetDirection(m_eCrtStructureDir);
    if (structure->getDirType() == CStructure::StructureDirType::EIGHT)
    {
        structure->SetDirection(m_eCrtStructureDir);
        structure->SetEDirection(m_iEightDirection);
    }

    CTileMap& map = *CMapManager::GetInstance().getMainTile();

    for (int x = _tile->getIndex().x; x < _tile->getIndex().x + (int)_item->getInfo().fieldSize.x; ++x)
    {
        for (int y = _tile->getIndex().y; y < _tile->getIndex().y + (int)_item->getInfo().fieldSize.y; ++y)
        {
            if (x < map.getRectSize().x && y < map.getRectSize().y)
            {
                structure->SetTile(map(x, y));
            }
        }
    }

    m_pInventory->SubItem(_item->getInfo().id, 1);

    if (!m_pInventory->HaveItem(id, 1))
    {
        m_pInventory->getInvenUI()->Reset();
        m_pHandItem = nullptr;
    }

    CBeltManager::GetInstance().AllResetBelt();

    structure->Start();
}

void CPlayer::DropFieldItem(int _itemId, CTile* _tile)
{
    CGameObject* newObj = m_pGameObject->getScene()->AddObject(CItemManager::GetInstance().GetItem(m_pHandItem->getInfo().id)->getInfo().itemName + L"Drop Image", Layer::FieldItem);
    CDropItem* newDrop = dynamic_cast<CDropItem*>(newObj->AddComponent(new CDropItem(_itemId, _tile)));
}

void CPlayer::DropBeltItem(int _itemId, CBelt* _belt)
{
    CGameObject* newObj = m_pGameObject->getScene()->AddObject(CItemManager::GetInstance().GetItem(m_pHandItem->getInfo().id)->getInfo().itemName + L"Drop Image", Layer::FieldItem);
    CDropItem* newDrop = dynamic_cast<CDropItem*>(newObj->AddComponent(new CDropItem(_itemId, _belt)));

    vector2 mousePos = m_pGameObject->getScene()->getMainCamera()->GetMousePosToWorld();
    vector2 tilePos = _belt->getFirstTile()->getTransform().getPosition();

    float pivot = 0.25f;

    float offsetX = (mousePos.x > tilePos.x) ? pivot : -pivot;
    float offsetY = (mousePos.y > tilePos.y) ? pivot : -pivot;

    newDrop->getTransform().SetPosition(tilePos + vector2(offsetX, offsetY));
}

void CPlayer::GunShoot()
{
    m_pShootTarget->GetComponent<CMonster>()->AddHp(m_sStat.shootDmg);
    m_pSoundObj->SetClip(L"SFX_PlayerShoot");
    m_pSoundObj->Play();
}

void CPlayer::GetItemHandler()
{
    m_pSoundObj->SetClip(L"SFX_Item");
    m_pSoundObj->Play();
}
