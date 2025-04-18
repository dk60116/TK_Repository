#pragma once

#include "Engine_Define.h"

#include "CGraphicDev.h"

class CMainProcess
{
	SINGLETONCLASS(CMainProcess);

public :
	enum GameRunningState { RUNNING, PAUSED, STOPPED };

public:
	HRESULT Ready_MainApp();
	_int Update_MainApp();
	void Render_MainApp();
	void Release();

	const GameRunningState getGameState() { return m_eGameState; };
	void SetGameState(const GameRunningState _status) { m_eGameState = _status; };
	void OnScreenChange(const _uint& _width, const _uint& _height);

public:
	const _bool& getStepOne() { return m_bStepOneFrame; }
	void SetStepOne(_bool _value) { m_bStepOneFrame = _value; }

private:
	LPDIRECT3DVERTEXBUFFER9 m_pVB;

private:
	GameRunningState m_eGameState;
	CGraphicDev* m_pDevClass;
	_bool m_bStepOneFrame;
};

