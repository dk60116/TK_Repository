#include "cpch.h"
#include "DungeonObject.h"
#include "DungeonChapter.h"

CDungeonObject::CDungeonObject()
	: m_pChapter(nullptr)
	, m_strObjName(L"")
	, m_sDescription({})
	, m_bHasAnimation(nullptr)
	, m_vRenderer({})
	, m_vSkinnedRenderer({})
	, m_pAnimator(nullptr)
	, m_pCollider(nullptr)
{
}

CDungeonObject::~CDungeonObject()
{
}

HRESULT CDungeonObject::Initialize()
{
	if (FAILED(__super::Initialize()))
		return E_FAIL;

	m_bOnlyCloneComponent = true;

	return S_OK;
}

void CDungeonObject::Awake()
{
	if (!m_bHasAnimation)
	{
		m_vRenderer = m_pGameObject->CreateMeshHierachy(CResources::LoadMeshBuffersOnScene(m_strObjName + L"_Model (MeshBuffer)"), m_sDescription.scaleFactor);

		for (TRAVERSAL_ITER(m_vRenderer, it))
			(*it)->Get_Material()->Set_Texture(CResources::LoadOnScene<CTexture>(m_strObjName + L"_BaseMap (Texture)"));
	}
	else
	{
		m_vSkinnedRenderer = m_pGameObject->CreateSkinnedMeshHierachy(CResources::LoadSkinnedMeshBuffersOnScene(m_strObjName + L"_Model (SkinnedBuffer)"), CResources::LoadSkinnedBonesOnScene(m_strObjName + L"_Model (SkinnedBuffer)"), m_sDescription.scaleFactor);

		for (TRAVERSAL_ITER(m_vSkinnedRenderer, it))
			(*it)->Get_Material()->Set_Texture(CResources::LoadOnScene<CTexture>(m_strObjName + L"_BaseMap (Texture)"));
	}
}

void CDungeonObject::Update()
{
}

void CDungeonObject::OnDestroy()
{
	Safe_Release(m_pChapter);
}

void CDungeonObject::Set_Chapter(CDungeonChapter* _chapter)
{
	m_pChapter = _chapter;
	
	if (m_pChapter)
		m_pChapter->AddRef();
}

const wstring& CDungeonObject::Get_ObjName()
{
	return m_strObjName;
}
