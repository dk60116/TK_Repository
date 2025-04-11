#pragma once

#include "pch.h"
#include "CBelt.h"
#include "CAnimator.h"

class CBeltManager
{
	SingletonClass(CBeltManager);

public:
	void Update();

public:
	void AddBelt(CBelt* _belt);
	void DeleteBelt(CBelt* _belt);
	void AddItem(CDropItem* _item);
	list<CDropItem*>& getItemList() { return m_lDropItemList; }
	void DeleteItem(CDropItem* _item);
	void AllResetBelt();
	CBelt* GetAdjacentBeltsAtTile(CTile* _tile);
	void TranspertItem(CTile* _tile);
	const float& getBeltSpeed() { return m_fBeltSpeed; }

public:
	int m_iCurrentFrame;
	list<CBelt*> m_lBeltList;
	list<CDropItem*> m_lDropItemList;
	float m_fCurrnetTime;
	float m_fBeltFrameLate;
	float m_fBeltSpeed;
};

