#include "Resources.h"
#include "CTexture.h"
#include "EDebug.h"

CResources::CResources()
	: m_strBasePath(L"Assets")
{
    m_strName = L"Resourecs";
}

CResources::~CResources()
{
	Release();
}

HRESULT CResources::LoadAllFiles(LPDIRECT3DDEVICE9 _device)
{
    // 현재 작업 디렉토리 출력
    wchar_t cwd[MAX_PATH];
    GetCurrentDirectory(MAX_PATH, cwd);
    CDebug::Log(L"[Working Directory] " + wstring(cwd));
    CDebug::Log(L"[Base Path] " + m_strBasePath);

    // 텍스처 폴더 경로
    wstring textureFolder = m_strBasePath + L"\\Textures";
    wstring textureSearch = textureFolder + L"\\*.*";

    // 존재 확인
    if (GetFileAttributes(textureFolder.c_str()) == INVALID_FILE_ATTRIBUTES)
    {
        CDebug::Log(L"[Error] Texture folder not found: " + textureFolder);
        return E_FAIL;
    }
    else
    {
        CDebug::Log(L"[Success] Texture folder found: " + textureFolder);
    }

    WIN32_FIND_DATA findData;
    HANDLE hFind = FindFirstFile(textureSearch.c_str(), &findData);
    if (hFind == INVALID_HANDLE_VALUE)
        return E_FAIL;

    do
    {
        if (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
            continue;

        wstring fileName = findData.cFileName;
        wstring ext = PathFindExtension(fileName.c_str());

        if (ext == L".png" || ext == L".jpg" || ext == L".bmp" || ext == L".dds")
        {
            wstring nameOnly = fileName.substr(0, fileName.find_last_of(L'.'));
            wstring relativePath = L"Textures\\" + fileName;

            CDebug::Log(L"Loading texture: " + relativePath);
            CreateResource<CTexture>(_device, nameOnly, relativePath);
        }

    } while (FindNextFile(hFind, &findData));

    FindClose(hFind);
    return S_OK;
}

void CResources::Release()
{
    for (auto& typePair : m_resourceMap)
    {
        auto& resourceContainer = typePair.second;

        for (auto& resPair : resourceContainer)
        {
            if (resPair.second)
                resPair.second->Release();
        }

        resourceContainer.clear();
    }

    m_resourceMap.clear();
}
