#include "epch.h"
#include "UIManager.h"

CUIManager::CUIManager()
{
}

CUIManager::~CUIManager()
{
}

CUIManager& CUIManager::GetInstance()
{
	static CUIManager* instance;

	return *instance;
}
