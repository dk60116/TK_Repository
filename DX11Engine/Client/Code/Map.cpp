#include "cpch.h"
#include "Map.h"

CMap::CMap()
    : m_strMapName(L"")
    , m_sOptions({})
    , m_vRendererList({})
    , m_mTextureNameList({})
{
    m_strName = L"Map";
}

CMap::~CMap()
{
}

HRESULT CMap::Initialize()
{
    m_vRendererList = m_pGameObject->CreateMeshHierachy(CResources::LoadMeshBuffersOnScene(m_strMapName + L"_Model (MeshBuffer)"), m_sOptions.scaleFactor, CGameObject::NavigationStatic);
    AttachTextures();

    return S_OK;
}

void CMap::Awake()
{
}

void CMap::Update()
{
}

void CMap::OnDestroy()
{
}

void CMap::AttachTextures()
{
    const size_t count = m_vRendererList.size();

    m_mTextureNameList.emplace(L"Base Map", vector<wstring>(count));
    m_mTextureNameList.emplace(L"Specular Map", vector<wstring>(count));
    m_mTextureNameList.emplace(L"Normal Map", vector<wstring>{count});
    m_mTextureNameList.emplace(L"Height Map", vector<wstring>{count});
    m_mTextureNameList.emplace(L"Ocullusion Map", vector<wstring>{count});

    for (size_t i = 0; i < count; i++)
    {
        wstring objName = m_vRendererList[i]->Get_GameObject()->Get_ObjectName();
        m_mTextureNameList[L"Base Map"][i] = m_strMapName + L"_Pice-" + objName + L"-BaseMap";
    }

    vector<wstring>& baseMapTexList = m_mTextureNameList[L"Base Map"];

    for (size_t i = 0; i < count; i++)
    {
        CTexture* tex = CResources::LoadOnScene<CTexture>(baseMapTexList[i] + L" (Texture)");
        
        if (tex)
            m_vRendererList[i]->Get_Material()->Set_Texture(tex, 0);
    }
}
