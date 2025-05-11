#pragma once

#include "pch.h"
#include "CTile.h"

class CTileManager;

class CTileMap
{
public:
	CTileMap();
	~CTileMap();

public:
	CTile* operator()(int _y, int _x)
	{
		return m_vTileMap[_y * m_v2MapSize.x + _x];
	}

	CTile* operator()(vector2Int _pos)
	{
		return m_vTileMap[_pos.y * m_v2MapSize.x + _pos.x];
	}

public:
	void Release();

public:
	const int getSize() { return m_v2MapSize.x* m_v2MapSize.y; }
	const vector2Int& getRectSize() { return m_v2MapSize; }
	const TILE_TYPE& getType() { return m_eTileType; }
	void SetType(const TILE_TYPE _type) { m_eTileType = _type; }
	const wstring& getPropertie() { return m_strPropertie; }
	void SetPropertie(const wstring _prop) { m_strPropertie = _prop; }

private:
	vector<CTile*> m_vTileMap;
	vector2Int m_v2MapSize;
	TILE_TYPE m_eTileType;
	wstring m_strSpriteName;
	wstring m_strPropertie;

	friend class CMapManager;
};

