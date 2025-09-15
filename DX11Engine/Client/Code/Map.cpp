#include "cpch.h"
#include "Map.h"
#include "MapCollision.h"

CMap::CMap()
    : m_strMapName(L"")
    , m_sOptions({})
    , m_vRendererList({})
    , m_mTextureNameList({})
	, m_pMapCollision(nullptr)
{
    m_strName = L"Map";
}

CMap::~CMap()
{
}

HRESULT CMap::Initialize()
{
    m_vRendererList = m_pGameObject->CreateMeshHierachy(CResources::LoadMeshBuffersOnScene(m_strMapName + L"_Model (MeshBuffer)"), m_sOptions.scaleFactor, CGameObject::TransformStatic | CGameObject::NavigationStatic);
    AttachTextures();

	m_pMapCollision = m_pGameObject->AddComponent<CMapCollision>();
    m_pMapCollision->Set_Map(this);

    auto data = m_pMapCollision->ReadColliderInfo(m_strMapName);

    if (!data.empty())
        m_pMapCollision->LoadColliders(data);

    return S_OK;
}

void CMap::Awake()
{
}

void CMap::Update()
{
    if (CInput::GetKey_Editor(CONTROL))
    {
        if (CInput::GetKeyDown_Editor(K))
			m_pMapCollision->SaveColliders(L"BinaryAssets/SceneData/" + m_strMapName + L".mapcoldata");

        //if (CInput::GetKeyDown_Editor(L))
        //{
        //    auto d = m_pMapCollision->ReadColliderInfo(m_strMapName);
        //    
        //    if (!d.empty())
        //        m_pMapCollision->LoadColliders(d);
        //}
    }
}

void CMap::OnDestroy()
{
}

const wstring& CMap::Get_MapName() const
{
    return m_strMapName;
}

void CMap::AttachTextures()
{
    const size_t count = m_vRendererList.size();

    m_mTextureNameList.emplace(L"Base Map", vector<wstring>(count));
    m_mTextureNameList.emplace(L"Specular Map", vector<wstring>(count));
    m_mTextureNameList.emplace(L"Normal Map", vector<wstring>{count});
    m_mTextureNameList.emplace(L"Height Map", vector<wstring>{count});
    m_mTextureNameList.emplace(L"Ocullusion Map", vector<wstring>{count});

    {
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

    {
        for (size_t i = 0; i < count; i++)
        {
            wstring objName = m_vRendererList[i]->Get_GameObject()->Get_ObjectName();
            m_mTextureNameList[L"Normal Map"][i] = m_strMapName + L"_Pice-" + objName + L"-NormalMap";
        }

        vector<wstring>& normalMapTexList = m_mTextureNameList[L"Normal Map"];

        for (size_t i = 0; i < count; i++)
        {
            CTexture* tex = CResources::LoadOnScene<CTexture>(normalMapTexList[i] + L" (Texture)");

            if (tex)
                m_vRendererList[i]->Get_Material()->Set_Texture(tex, 1);
        }
    }
}
