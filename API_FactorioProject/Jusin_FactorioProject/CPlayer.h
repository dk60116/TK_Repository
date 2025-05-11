#pragma once

#include "CMonoBehaviour.h"
#include "CAnimator.h"
#include "CTile.h"
#include "CInventory.h"
#include "CImage.h"
#include "CGameObject.h"
#include "CItemSlot.h"
#include "CCreatingSlot.h"
#include "CSpriteRender_Level.h"
#include "CStructure.h"
#include "CAudioSource.h"
#include "CCollider.h"
#include "CItemManager.h"

struct PlayerStatus
{
	float maxHp = 100.f;
	float crtHp = 0.f;
	float moveSpeed = 0.6f;
	float recoverHpVlauue = 5.f;
	float shootInterval = 0.05f;
	float shootDmg = 1.f;
	float shootDistance = 8.f;
};

class CPlayer :
    public CMonoBehaviour
{
public:
	CPlayer();
	~CPlayer();

public:
	void Awake() override;
	void Start() override;
	void Reset() override;
	void Update() override;
	void FixedUpdate() override;
	void LateUpdate() override;
	void OnEnable() override;
	void OnDisable() override;
	void Render(HDC _hDC) override;
	void OnCollisionEnter(CCollider* _other) override;
	void OnCollisionStay(CCollider* _other) override;
	void OnCollisionExit(CCollider* _other) override;
	void OnDestroy() override;
	void OnMouseEnter() override;
	void OnMouseOver() override;
	void OnMouseExit() override;

public:
	PlayerStatus& getStat() { return m_sStat; }

	void Mining(CTile* _tile);
	void CancelMining();
	void CpatureStructure(CTile* _tile);
	void CancelCapture();

	void AddHp(float _dmg);
	void SetMaskAnim(CAnimator* _anim) { m_pPlayerMaskAnim = _anim; }
	void SetShadowAnim(CAnimator* _anim) { m_pShadowAnim = _anim; }
	CInventory* getInventory() { return m_pInventory; }
	void SetInventory(CInventory* _inven) { m_pInventory = _inven; }

	void SetCollectingGBObj(CGameObject* _obj) { m_pCollectingGaugeBar = _obj; }
	void SetCollectingGauge(CImage* _img) { m_pCollectingGauge = _img; }

	bool isHandOpen() { return m_pCurrentHandItemIcon->getObj()->isActive(); }
	CItem* getHandItem() { return m_pHandItem; }
	void OpenHandle(CItem* _item);
	void AbleHideHandle(const bool _value);
	void OpenBlueprint();
	void HideBlueprint();
	void BlueprintUpdate() { m_pBlueprint->getObj()->Update(); }
	void CloseHand();
	void ChangeHandleSprite(wstring _sprite, vector2Int _start, vector2Int _end);
	CItemSlot* getCurrentMouseSlot() { return m_pCurrentMouseItemSlot; }
	void SetCrtSlot(CItemSlot* _slot) { m_pCurrentMouseItemSlot = _slot; }
	queue<CItem*>& getQueue() { return m_vCreateItemQueue; }
	void AddCreateItemQueue(CItem* _item);
	void CreateStructure(CItem* _item, CTile* _tile, CStructure::StructureDir _dir);
	void SetCanCreateStructure(const bool _value) { m_bCanCreateStructure = _value; }
	void DropFieldItem(int _itemId, CTile* _tile);
	void DropBeltItem(int _itemId, CBelt* _belt);
	void GunShoot();
	void GetItemHandler();

private:
	PlayerStatus m_sStat;
	CAnimator* m_pPlayerAnim;
	CAnimator* m_pPlayerMaskAnim;
	CAnimator* m_pShadowAnim;
	CTile* m_pStayTile;
	CInventory* m_pInventory;
	vector2 m_v2Direction, m_v2PrevDirection, m_v2MiningDirection;
	bool m_bIsWalk, m_bIsMining, m_bCaptuering;
	CStructure* m_pOverStructure;
	CGameObject* m_pCollectingGaugeBar;
	CImage* m_pCollectingGauge;
	CItem* m_pHandItem;


	CImage* m_pHpBar, * m_pHpGauge;
	CImage* m_pCurrentHandItemIcon;
	CSpriteRenderer* m_pBlueprint;
	CSpriteRender_Level* m_pBPL;

	CTile* m_pMouseOverTile;
	CTile* m_pCurrentCollectingTile;
	CItemSlot* m_pCurrentMouseItemSlot;

	queue<CItem*> m_vCreateItemQueue;
	vector<CCreatingSlot*> m_vCreatingSlotList;
	float m_fCreateItemTime;
	bool m_bCanCreateStructure;

	CAudioSource* m_pSoundObj;
	CCollider* m_pCollider;

	bool m_bCreatedCStructure;

	CStructure::StructureDir m_eCrtStructureDir;
	BYTE m_iEightDirection;

	float m_fLastHitTime;
	bool m_bIsShooting;
	CGameObject* m_pShootTarget;
	vector2Int m_v2ShootDirection;
	CAnimator* m_pGunMuzzleEffect;
	float m_fShootTime;
};

