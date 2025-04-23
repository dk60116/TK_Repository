#pragma once

#include "Engine_Define.h"

#include "CGraphicDev.h"

class CMainProcess
{
	SINGLETONCLASS(CMainProcess);

public:
	HRESULT Ready_MainApp();
	_int Update_MainApp();
	void Render_MainApp();
	void Release();

	void OnSceneScreenChange(const _uint _width, const _uint _height);
	void OnGameScreenChange(const _uint _width, const _uint _height);

private:
	CGraphicDev* m_pDevClass;
};

