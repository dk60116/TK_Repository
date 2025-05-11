#include "CTileMap.h"

CTileMap::CTileMap()
	: m_vTileMap({})
	, m_eTileType(TILE_TYPE::TILE_DEFAULT)
	, m_v2MapSize(vector2Int::zero())
	, m_strSpriteName(L"")
	, m_strPropertie(L"")
{
}

CTileMap::~CTileMap()
{
}

void CTileMap::Release()
{
	for (TRAVERSAL_ITER(m_vTileMap, it))
		Safe_Delete(*it);

	m_vTileMap.clear();
}
