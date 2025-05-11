#pragma once

#include "pch.h"
#include "CTileMap.h"
#include "CScene.h"

class CMapManager
{
	SingletonClass(CMapManager);

public:
	void Init();
	void Release();

public:
	queue<CTile*>& getMessageQueue() { return m_qMessage; }

public:
	CTileMap* CreateTileMap(wstring _name, TILE_TYPE _type, vector2Int _tileSize, wstring _spriteName, wstring _propertie = L"");
	CTileMap* AttachTileMap(CScene* _scene, CTileMap* _targetMain, wstring _attachTile, vector2Int _startPos);
	CTileMap* getMainTile() { return m_pMainTile; }
	void SetMainTile(CTileMap* _map) { m_pMainTile = _map; }

private:
	map<wstring, CTileMap*> m_mTileMapList;
	CTileMap* m_pMainTile;

	friend class CTile;

private:
	queue<CTile*> m_qMessage;
};

