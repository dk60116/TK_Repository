#pragma once

#include "Engine_Define.h"

#include "CGraphicDev.h"

#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZRHW | D3DFVF_DIFFUSE)

class CMainProcess
{
	SINGLETONCLASS(CMainProcess);

public:
	enum GameState { RUNNING, PAUSED, STOPPED };

public:
	HRESULT Ready_MainApp();
	_int Update_MainApp();
	void Render_MainApp();
	void Release();
	const GameState& getGameState() { return m_eGameState; };
	void SetGameState(const GameState _status) { m_eGameState = _status; };

private:
	LPDIRECT3DVERTEXBUFFER9 m_pVB;

private:
	GameState m_eGameState;
	CGraphicDev* m_pDevClass;
	LPDIRECT3DDEVICE9	m_pGraphicDev;
};

