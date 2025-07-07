#pragma once

#include "epch.h"

NS_BEGIN(Engine)

class ENGINE_DLL CSceneLoader final
{
	SINGLETONCLASS(CSceneLoader);

private:
	static unsigned __stdcall ThreadMain(void* pParam);

public:
	HRESULT Initialize();

public:
	const _bool Is_Loading() const;

public:
	void StartLoading(vector<string>& _nameList, vector<string>& _fileList);
	void EndLoading();

private:
	void ThreadLoadingLoop();
	void LoadComplete_Scene(class CEngineResource* _ptr, wstring _name);
	void Shutdown();

private:
	HANDLE m_hThread;
	CRITICAL_SECTION m_pCriticalSection;

	unordered_map<string, string> m_mReadyFiles;

	_bool m_bRunning;
	_bool m_bLoading;

	_bool m_bGameResourceCreated;
};

NS_END
