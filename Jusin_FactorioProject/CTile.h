#pragma once

#include "CMonoBehaviour.h"
#include "CSpriteRender_Level.h"
#include "CItemManager.h"

class CStructure;

enum TILE_TYPE { TILE_DEFAULT = 0, TILE_PRECIPIC, TILE_WATER, TIEL_RESOURCE };

class CTile :
    public  CMonoBehaviour
{
public:
    CTile();
	CTile(CTile& _rhs);
    ~CTile();

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
	void MessageUpdate() override;

public:
	const vector2Int& getIndex() { return m_v2Index; }
	void setIndex(const vector2Int _value) { m_v2Index = _value; }
	const wstring& getTextureName() { return m_strTexture; }
	void SetTexture(const wstring _name) { m_strTexture = _name; }
	const TILE_TYPE& getType() { return m_eType; }
	void SetType(const TILE_TYPE _type) { m_eType = _type; }
	CStructure* getHaveStructure() { return m_pHaveStructure; }
	void SetHaveStructure(CStructure* _value);
	int DropItem(CDropItem* _item);
	void DropItem(CItem* _item, int _index);
	CDropItem* getDropItems(int _index) { return m_pItemSlots[_index]; }
	void DeleteDropItem(int _index) { m_pItemSlots[_index] = nullptr; }
	void SetItem(int _index, CDropItem* _item);

private:
	TILE_TYPE m_eType;
	vector2Int m_v2Index;
	CSpriteRenderer* m_pRenderer;
	CSpriteRender_Level* m_pRenderLevel;
	wstring m_strTexture;
	int m_iTextureIndex;
	CStructure* m_pHaveStructure;
	CDropItem* m_pItemSlots[4];
	bool m_bIsItemMove;
};

