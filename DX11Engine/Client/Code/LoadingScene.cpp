#include "cpch.h"
#include "LoadingScene.h"

LoadingScene::LoadingScene()
{
}

LoadingScene::~LoadingScene()
{
}

HRESULT LoadingScene::Initialize()
{
	if (FAILED(__super::Initialize()))
		return E_FAIL;

	return S_OK;
}

void LoadingScene::Update()
{
}
