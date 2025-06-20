#pragma once

#include "Object.h"
#include "Scene.h"

NS_BEGIN(Engine)

class ENGINE_DLL CSceneManager final : public UObject
{
	SINGLETONCLASS(CSceneManager);

public:
	void Destroy();

public:
	CScene* Get_CrtScene();

private:
	CScene* m_pCrtScene;
	list<CScene*> m_lSceneList;
};

NS_END

