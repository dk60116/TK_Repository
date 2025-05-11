#include "CMapManager.h"
#include "CRandom.h"
#include "CTestScene.h"
#include "CResourceTile.h"

CMapManager::CMapManager()
	: m_pMainTile(nullptr)
	, m_qMessage()
{
}

CMapManager::~CMapManager()
{
	Release();
}

void CMapManager::Init()
{
	{
		CTileMap* defaultMap = CreateTileMap(L"DefaultMap", TILE_TYPE::TILE_DEFAULT, vector2Int(76, 76), L"Terrain_Dirt_2");
	}
	{
		vector2Int randomSize(CRandom::GetInstance().Range(3, 8), CRandom::GetInstance().Range(3, 8));
		CTileMap* lake = CreateTileMap(L"Lake", TILE_TYPE::TILE_WATER, randomSize, L"Terrain_Water");
	}
	{
		vector2Int randomSize(CRandom::GetInstance().Range(4, 7), CRandom::GetInstance().Range(4, 7));
		CTileMap* coal = CreateTileMap(L"CoalGround", TILE_TYPE::TIEL_RESOURCE, randomSize, L"Terrain_Dirt_2", L"Coal");
	}
	{
		vector2Int randomSize(CRandom::GetInstance().Range(4, 7), CRandom::GetInstance().Range(4, 7));
		CTileMap* stone = CreateTileMap(L"StoneGround", TILE_TYPE::TIEL_RESOURCE, randomSize, L"Terrain_Dirt_2", L"Stone_Ground");
	}
	{
		vector2Int randomSize(CRandom::GetInstance().Range(4, 7), CRandom::GetInstance().Range(4, 7));
		CTileMap* iron = CreateTileMap(L"IronGround", TILE_TYPE::TIEL_RESOURCE, randomSize, L"Terrain_Dirt_2", L"Iron");
	}
	{
		vector2Int randomSize(CRandom::GetInstance().Range(4, 7), CRandom::GetInstance().Range(4, 7));
		CTileMap* copper = CreateTileMap(L"CopperGround", TILE_TYPE::TIEL_RESOURCE, randomSize, L"Terrain_Dirt_2", L"Copper");
	}
}

void CMapManager::Release()
{
	for (TRAVERSAL_ITER(m_mTileMapList, it))
	{
		(*it).second->Release();
		Safe_Delete(it->second);
	}

	m_mTileMapList.clear();
}

CTileMap* CMapManager::CreateTileMap(wstring _name, TILE_TYPE _type, vector2Int _tileSize, wstring _spriteName, wstring _propertie)
{
	CTileMap* tileMap = new CTileMap();

	tileMap->m_eTileType = _type;
	tileMap->m_v2MapSize = _tileSize;
	tileMap->m_strSpriteName = _spriteName;

	for (int i = 0; i < tileMap->getSize(); ++i)
	{
		CTile* tile = new CTile();

		if (_propertie.length() > 0)
			tileMap->m_strPropertie = _propertie;

		tileMap->m_vTileMap.push_back(tile);
	}

	m_mTileMapList.insert({ _name, tileMap });

	return  tileMap;
}

CTileMap* CMapManager::AttachTileMap(CScene* _scene, CTileMap* _targetMain, wstring _attachTile, vector2Int _startPos)
{
	CTileMap& map = *m_mTileMapList[_attachTile];

	CTileMap* result = new CTileMap();
	result->m_v2MapSize = map.m_v2MapSize;
	result->m_eTileType = map.m_eTileType;

	if (_targetMain == nullptr)
	{
		for (int x = 0; x < map.m_v2MapSize.x; ++x)
		{
			for (int y = 0; y < map.m_v2MapSize.y; ++y)
			{
				CGameObject* newObj = new CGameObject();
				CTile* newTile = new CTile(*map(x, y));
				newTile->SetType(map.m_eTileType);
				newTile->setIndex(vector2Int(x, y));
				newTile->SetTexture(map.m_strSpriteName);
				newObj->AddComponent(newTile);
				result->m_vTileMap.push_back(newTile);
				_scene->Instantiate(newObj, L"Tile", (_startPos + vector2Int(x, y)).toVector2());
			}
		}
	}
	else
	{
		for (int x = _startPos.x; x < _startPos.x + map.m_v2MapSize.x; ++x)
		{
			for (int y = _startPos.y; y < _startPos.y + map.m_v2MapSize.y; ++y)
			{
				CTile* srcTile = (*_targetMain)(x, y);

				srcTile->SetTexture(map.m_strSpriteName);
				srcTile->SetType(map.m_eTileType);
				srcTile->getObj()->GetComponent<CSpriteRenderer>()->SetSprite(map.m_strSpriteName);

				wstring prop = map.m_strPropertie;

				if (prop.length() > 0)
				{
					if (prop.find(L"Coal") != wstring::npos)
						srcTile->getObj()->AddComponent(new CResourceTile(CResourceTile::COAL, 1000));
					else if (prop.find(L"Iron") != wstring::npos)
						srcTile->getObj()->AddComponent(new CResourceTile(CResourceTile::IRON_ORE, 1000));
					else if (prop.find(L"Stone_Ground") != wstring::npos)
						srcTile->getObj()->AddComponent(new CResourceTile(CResourceTile::STONE, 1000));
					else if (prop.find(L"Copper") != wstring::npos)
						srcTile->getObj()->AddComponent(new CResourceTile(CResourceTile::COPPER_ORE, 1000));
				}
			}
		}
	}

	return result;
}
