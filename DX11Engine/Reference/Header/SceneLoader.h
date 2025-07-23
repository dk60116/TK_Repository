#pragma once

#include "epch.h"
#include "MeshBuffer.h"

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
	void StartLoading(vector<string>& _nameList, vector<string>& _fileList, vector<string>& _formatList);

private:
	void ThreadLoadingLoop();
	void Shutdown();

private:
	CMeshBuffer::TERRAINBUFFERDESC FormatToTerrainDesc(wstring _name, wstring _format) const;

private:
	HANDLE m_hThread;
	CRITICAL_SECTION m_pCriticalSection;

	vector<string> m_mReadyFiles_Name, m_mReadyFiles_Path, m_mReadyFiles_Format;

	_bool m_bRunning;
	_bool m_bLoading;
};

NS_END
