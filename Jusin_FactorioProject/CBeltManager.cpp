#include "CBeltManager.h"
#include "CGameObject.h"
#include "CMapManager.h"

CBeltManager::CBeltManager()
	: m_iCurrentFrame(0)
	, m_lBeltList({})
	, m_fCurrnetTime(0.f)
	, m_fBeltFrameLate(0.025f)
	, m_fBeltSpeed(1.2f)
{
}

CBeltManager::~CBeltManager()
{
	m_lBeltList.clear();
	m_lDropItemList.clear();
}

void CBeltManager::Update()
{
	m_fCurrnetTime += DELTA_TIME;

	while (m_fCurrnetTime >= m_fBeltFrameLate)
	{
		m_fCurrnetTime -= m_fBeltFrameLate;

		++m_iCurrentFrame;

		for (TRAVERSAL_ITER(m_lDropItemList, it))
		{
			//(*it)->Reset();
		}

		if (m_iCurrentFrame >= 16)
			m_iCurrentFrame = 0;
	}

	for (TRAVERSAL_ITER(m_lBeltList, it))
	{
		(*it)->m_pBodyAnimator->m_pCurrentAnimation->m_iCurrentFrame = m_iCurrentFrame;
		(*it)->m_pSideAnmA->m_pCurrentAnimation->m_iCurrentFrame = m_iCurrentFrame;
		(*it)->m_pSideAnmB->m_pCurrentAnimation->m_iCurrentFrame = m_iCurrentFrame;
	}
}

void CBeltManager::AddBelt(CBelt* _belt)
{
	m_lBeltList.push_back(_belt);
}

void CBeltManager::DeleteBelt(CBelt* _belt)
{
	if (!_belt)
		return;

	if (m_lBeltList.size() <= 0)
		return;

	auto& lBeltList = m_lBeltList;

	auto it = find(lBeltList.begin(), lBeltList.end(), _belt);

	if (it == lBeltList.end())
		return;

	lBeltList.remove(_belt);
}

void CBeltManager::AddItem(CDropItem* _item)
{
	m_lDropItemList.push_back(_item);
}

void CBeltManager::DeleteItem(CDropItem* _item)
{
	if (!_item)
		return;

	if (m_lDropItemList.size() <= 0)
		return;

	auto& lItemList = m_lDropItemList;

	auto it = find(lItemList.begin(), lItemList.end(), _item);

	if (it == lItemList.end())
		return;

	lItemList.remove(_item);
}

void CBeltManager::AllResetBelt()
{
	for (TRAVERSAL_ITER(m_lBeltList, it))
		(*it)->Reset();
}

CBelt* CBeltManager::GetAdjacentBeltsAtTile(CTile* _tile)
{
	CTileMap& map = *CMapManager::GetInstance().getMainTile();

	vector2Int myInex = _tile->getIndex();

	CTile tiles[CStructure::COUNT] = {};
	CBelt result[CStructure::COUNT] = {};

	tiles[CStructure::RIGHT] = *map(myInex.x + 1, myInex.y);
	tiles[CStructure::LEFT] = *map(myInex.x - 1, myInex.y);
	tiles[CStructure::UP] = *map(myInex.x, myInex.y - 1);
	tiles[CStructure::DOWN] = *map(myInex.x, myInex.y + 1);

	for (size_t i = 0; i < CStructure::COUNT; ++i)
		result[i] = *(dynamic_cast<CBelt*>(tiles[i].getHaveStructure()));

	return result;
}

void CBeltManager::TranspertItem(CTile* _tile)
{
	for (int i = 0; i < 4; ++i)
	{
		CDropItem* item = _tile->getDropItems(i);

		if (item)
			_tile->getDropItems(i)->getObj()->getTransform().AddPosition(vector2::right() * 0.06f * DELTA_TIME);
	}
}
