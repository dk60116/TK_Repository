#pragma once

#include "epch.h"
#include "Scene.h"
#include "Shader.h"
#include "Texture.h"
#include "SkinnedMeshBuffer.h"
#include "AnimationClip.h"
#include "NaviMesh.h"

#include <filesystem>
namespace fs = filesystem;

NS_BEGIN(Engine)

class ENGINE_DLL CResources final
{
    SINGLETONCLASS(CResources);

public:
    HRESULT Initialize();

private:
    void Release();

public:
    static void LoadResourceComplete_Game(const class CEngineResource* _ptr);
    static void LoadResourceComplete_Scene(const class CEngineResource* _ptr);

public:
    HRESULT ConvertFBXToMeshBufferData(const wstring _filePath);
    HRESULT ConvertFBXToSkinnedBufferData(const wstring _filePath);
    HRESULT ConvertFBXToAnimationClipData(const wstring _filePath);

    HRESULT ConvertOTFTTFToSpriteFont(const wstring _filePath);

    HRESULT BakeNaviMesh(vector<CMeshBuffer*> _buffers);

public:
    HRESULT SaveSceneObjectTransformInfos(const wstring _filePath, vector<CScene::ObjectsTransformInfo> _infoList);
    vector<CScene::ObjectsTransformInfo> ReadSceneObjectTransformInfos(const wstring _binFileName);

    HRESULT SaveMeshBufferInfos(const wstring _filePath, vector<CMeshBuffer::MeshBufferInitiaizeInfo> _infoList);
    vector<CMeshBuffer::MeshBufferInitiaizeInfo> ReadMeshBufferInfos(const wstring _binFileName);
    HRESULT SaveSkinnedBufferInfos(const wstring _filePath, vector<CSkinnedMeshBuffer::SkinnedBufferInitiaizeInfo> _infoList, vector<CSkinnedMeshBuffer::SKINNEDSKELETAL> _skeletonInfo);
    CSkinnedMeshBuffer::SkinnedBuffer ReadSkinnedBufferInfos(const wstring _binFileName);
    HRESULT SaveNaviMeshBufferInfos(const wstring _filePath, EngineAI::CNaviMesh::NaviMeshBufferInitiaizeInfo _info);
    EngineAI::CNaviMesh::NaviMeshBufferInitiaizeInfo ReadNaviBufferInfos(const wstring _binFileName);
    HRESULT SaveAnimationClipBufferInfos(const wstring _filePath, vector<CAnimationClip::AnimationClipInitInfo> _infoList);
    vector<CAnimationClip::AnimationClipInitInfo> ReadAnimationClipBufferInfos(const wstring _binFileName);

public:
    template<typename T>
    T* CreateGameResource(const wstring& _name, const wstring& _path, void* _desc = nullptr);

    template<typename T>
    T* CreateSceneResource(const wstring& _name, const wstring& _path, void* _desc = nullptr, const _bool _tempScene = false);

    vector<MeshBundle> CreateSceneMeshBundle(const wstring& _name, vector<CMeshBuffer::MeshBufferInitiaizeInfo> _infoList, _int _filter, void* _desc = nullptr, const _bool _tempScene = false);
    vector<SkinnedMeshBundle> CreateSceneSkinnedBundle(const wstring& _name, vector<CSkinnedMeshBuffer::SkinnedBufferInitiaizeInfo> _infoList, vector<CSkinnedMeshBuffer::SKINNEDSKELETAL> _skelList, _int _filter, void* _desc = nullptr, const _bool _tempScene = false);

    template<typename T>
    T* LoadOnGame(const wstring& _name);

    template<typename T>
    T* CloneOnGame(const wstring& _name);

    template<typename T>
    T* LoadOnScene(const wstring& _name);

    vector<MeshBundle> LoadMeshBuffersOnScene(const wstring& _name);
    vector<SkinnedMeshBundle> LoadSkinnedMeshBuffersOnScene(const wstring& _name);
    vector<CSkinnedMeshBuffer::SKINNEDSKELETAL> LoadSkinnedBonesOnScene(const wstring& _name);

    static _bool FileExists(const wstring& _path);
    static _bool FileExists(const string& _path);

    unordered_map<wstring, CEngineResource*> m_mEditorResourceList;
    unordered_map<wstring, CEngineResource*> m_mGameResourceList;

private:
    void Ready_GameResources();
    void TraverseSkeleton(aiNode* _node, _int _parentId, vector<CSkinnedMeshBuffer::SKINNEDSKELETAL>& _outList);

private:
    wstring m_strDefaultAssetPath;
    wstring m_strEngineFilePath;
};

NS_END

template<typename T>
inline T* CResources::CreateGameResource(const wstring& _name, const wstring& _path, void* _desc)
{
    T* newResource = T::Create();

    if (!newResource)
        return nullptr;

    if (FAILED(newResource->Initialize(_name, m_strDefaultAssetPath + _path, _desc)))
    {
        delete newResource;
        return nullptr;
    }

    m_mGameResourceList.emplace(_name, newResource);
    newResource->AddRef();

    return newResource;
}

template<typename T>
inline T* CResources::CreateSceneResource(const wstring& _name, const wstring& _path, void* _desc, const _bool _tempScene)
{
    T* newResource = T::Create();

    if (FAILED(newResource->Initialize(_name, m_strDefaultAssetPath + _path, _desc)))
    {
        delete newResource;
        return nullptr;
    }

    CScene* targetScene = _tempScene ? CSceneManager::GetInstance().Get_TempScene() :
        CSceneManager::GetInstance().Get_CrtScene();

    if (!_tempScene)
        targetScene->Add_Resource(_name, newResource);
    else
        targetScene->Add_TempResource(_name, newResource);

    return newResource;
}

template<typename T>
inline T* CResources::LoadOnGame(const wstring& _name)
{
    auto iter = m_mGameResourceList.find(_name);

    if (iter == m_mGameResourceList.end())
        return nullptr;

    T* resultResource = dynamic_cast<T*>(iter->second);

    return resultResource;
}

template<typename T>
inline T* CResources::CloneOnGame(const wstring& _name)
{
    T* proto = LoadOnGame<T>(_name);

    T* clone = T::Clone(*proto);
    
    if (CSceneManager::GetInstance().Get_CrtScene())
        CSceneManager::GetInstance().Get_CrtScene()->Add_CloneResourece(clone);

    return clone;
}

template<typename T>
inline T* CResources::LoadOnScene(const wstring& _name)
{
    CEngineResource* r = nullptr;

    if (CSceneManager::GetInstance().Get_CrtScene())
        r = CSceneManager::GetInstance().Get_CrtScene()->Find_Resource(_name);

    T* resultResource = dynamic_cast<T*>(r);

    if (!r)
    {
        if (!CSceneManager::GetInstance().Get_TempScene())
            return nullptr;

        CEngineResource* r = CSceneManager::GetInstance().Get_TempScene()->Find_Resource(_name);

        resultResource = dynamic_cast<T*>(r);
    }

    return resultResource;
}