#include "Resources.h"
#include "Texture.h"
#include "AudioClip.h"
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

void CResources::SearchFiles(LPDIRECT3DDEVICE9 _device, const wstring _folder)
{
    wstring searchPath = _folder + L"\\*";
    WIN32_FIND_DATA findData;
    HANDLE hFind = FindFirstFile(searchPath.c_str(), &findData);

    if (hFind == INVALID_HANDLE_VALUE)
        return;

    do
    {
        if (wcscmp(findData.cFileName, L".") == 0 || wcscmp(findData.cFileName, L"..") == 0)
            continue;

        wstring fullPath = _folder + L"\\" + findData.cFileName;

        if (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
        {
            SearchFiles(_device, fullPath);
        }
        else
        {
            wstring ext = PathFindExtension(findData.cFileName);
            wstring nameOnly = findData.cFileName;
            nameOnly = nameOnly.substr(0, nameOnly.find_last_of(L'.'));

            wstring relativePath = fullPath.substr(m_strBasePath.length() + 1);

            if (ext == L".png" || ext == L".jpb" || ext == L".bmp" || ext == L".dds")
            {
                CDebug::Log(L"[Texture]" + relativePath);
                CreateResource<CTexture>(_device, nameOnly, relativePath);
            }
            else if (ext == L".mp3" || ext == L".wav" || ext == L".ogg")
            {
                CDebug::Log(L"[Audio]" + relativePath);
                CreateResource<CAudioClip>(_device, nameOnly, relativePath);
            }
        }

    } while (FindNextFile(hFind, &findData));

    FindClose(hFind);
}

HRESULT CResources::LoadAllFiles(LPDIRECT3DDEVICE9 _device)
{
    wchar_t cwd[MAX_PATH];
    GetCurrentDirectory(MAX_PATH, cwd);
    CDebug::Log(L"[Working Directory] " + wstring(cwd));
    CDebug::Log(L"[Base Path] " + m_strBasePath);

    SearchFiles(_device, m_strBasePath);

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
                resPair.second->Destroy();
        }

        resourceContainer.clear();
    }

    m_resourceMap.clear();
}
