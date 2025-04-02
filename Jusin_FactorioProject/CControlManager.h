#pragma once

#include "pch.h"
#include "CInput.h"
#include "CPlayer.h"

class CControlManager
{
	SingletonClass(CControlManager);

public:
	void Init();
	void Update();
	void LateUpdate();
	void Release();
	
public:
	void SetPlayer(CPlayer* pPlayer) { m_pPlayer = pPlayer; }

private:
	CPlayer* m_pPlayer;
};

