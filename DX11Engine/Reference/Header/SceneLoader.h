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

private:
	void ThreadLoadingLoop();
	void Shutdown();

private:
	HANDLE m_hThread;
	CRITICAL_SECTION m_pCriticalSection;

	unordered_map<string, string> m_mReadyFiles;

	_bool m_bRunning;
	_bool m_bLoading;
};

NS_END
