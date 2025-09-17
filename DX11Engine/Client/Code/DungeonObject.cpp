#include "cpch.h"
#include "DungeonObject.h"
#include "DungeonChapter.h"

CDungeonObject::CDungeonObject()
	: m_pChapter(nullptr)
	, m_strObjName(L"")
	, m_sDescription({})
	, m_vRenderer({})
	, m_vSkinnedRenderer({})
	, m_pAnimator(nullptr)
	, m_pCollider(nullptr)
{
}

CDungeonObject::~CDungeonObject()
{
}

HRESULT CDungeonObject::Initialize(void* _desc)
{
	m_bOnlyCloneComponent = true;

	if (FAILED(__super::Initialize(_desc)))
		return E_FAIL;

	return S_OK;
}

void CDungeonObject::Awake()
{
	if (m_sDescription.hasCollider)
	{
		m_pCollider = m_pGameObject->AddComponent<CBoxCollider>();
		m_pCollider->Set_Center(m_sDescription.colliderCenter);
		m_pCollider->Set_Size(m_sDescription.colliderSize);
		m_pCollider->SetTrigger(m_sDescription.isTrigger);
	}

	if (!m_sDescription.isSkinning)
	{
		m_vRenderer = m_pGameObject->CreateMeshHierachy(CResources::LoadMeshBuffersOnScene(m_strObjName + L"_Model (MeshBuffer)"), m_sDescription.scaleFactor);

		for (TRAVERSAL_ITER(m_vRenderer, it))
			(*it)->Get_Material()->Set_Texture(CResources::LoadOnScene<CTexture>(m_strObjName + L"_BaseMap (Texture)"));
	}
	else
	{
		m_vSkinnedRenderer = m_pGameObject->CreateSkinnedMeshHierachy(CResources::LoadSkinnedMeshBuffersOnScene(m_strObjName + L"_Model (MeshBuffer)"), CResources::LoadSkinnedBonesOnScene(m_strObjName + L"_Model (SkinnedBuffer)"), m_sDescription.scaleFactor);

		for (TRAVERSAL_ITER(m_vSkinnedRenderer, it))
			(*it)->Get_Material()->Set_Texture(CResources::LoadOnScene<CTexture>(m_strObjName + L"_BaseMap (Texture)"));
	}
}

void CDungeonObject::Start()
{
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
