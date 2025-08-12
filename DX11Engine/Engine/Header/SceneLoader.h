#pragma once

#include "epch.h"
#include "MeshBuffer.h"
#include "SkyBox.h"

NS_BEGIN(Engine)

class ENGINE_DLL CSceneLoader final
{
	SINGLETONCLASS(CSceneLoader);

private:
	static unsigned __stdcall ThreadMain(void* pParam);

public:
	static HRESULT Initialize();

public:
	static const _bool Is_Loading();
	static const _float Get_LoadingProgress();

public:
	static void StartLoading(vector<string>& _nameList, vector<string>& _fileList, vector<string>& _formatList);

private:
	static void ThreadLoadingLoop();
	static void Shutdown();

private:
	static CSkyBox::SKYBOXBUFFERDESC FormatToSkyBoxDesc(wstring _name, wstring _format);
	static CMeshBuffer::TERRAINBUFFERDESC FormatToTerrainDesc(wstring _name, wstring _format);

private:
	HANDLE m_hThread;
	CRITICAL_SECTION m_pCriticalSection;

	vector<string> m_mReadyFiles_Name, m_mReadyFiles_Path, m_mReadyFiles_Format;

	_bool m_bRunning;
	_bool m_bLoading;

	_uint m_iTootalFile;
	_uint m_iLoadedFile;
};

NS_END
