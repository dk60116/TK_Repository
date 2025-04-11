#include "CControlManager.h"
#include "CScreen.h"
#include "CCamera.h"
#include "CSceneManager.h"
#include "CMapManager.h"

CControlManager::CControlManager()
	: m_pPlayer(nullptr)
{
}

CControlManager::~CControlManager()
{
	Release();
}

void CControlManager::Init()
{
}

void CControlManager::Update()
{
	if (!CMapManager::GetInstance().getMainTile())
		return;

	CTileMap& map = *CMapManager::GetInstance().getMainTile();

	vector2 mousePos = CSceneManager::GetInstance().getCurrentscene()->getMainCamera()->GetMousePosToWorld();
	vector2 camPos = CSceneManager::GetInstance().getCurrentscene()->getMainCamera()->getObj()->getTransform().getPosition();
	
	CTile* clickTile = nullptr;
	
	if (mousePos.x >= 0 && mousePos.x <= map.getRectSize().x - 1 &&
		mousePos.y >= 0 && mousePos.y <= map.getRectSize().y - 1)
		clickTile = map((int)round(mousePos.x), (int)round(mousePos.y));

	if (!clickTile)
		return;

	if (vector2::Distance(clickTile->getObj()->getTransform().getPosition(), camPos) < 2.4f)
	{
		if (clickTile->getType() == TILE_TYPE::TIEL_RESOURCE)
		{
			if (CInput::GetInstance().GetMouseButton(1))
			{
				m_pPlayer->Mining(clickTile);
			}
		}
		else
			m_pPlayer->CancelMining();
	}
	else
		m_pPlayer->CancelMining();
}

void CControlManager::LateUpdate()
{
}

void CControlManager::Release()
{
	m_pPlayer = nullptr;
}
