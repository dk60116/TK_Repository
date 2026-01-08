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
	, m_pAudioSource(nullptr)
	, m_pCollider(nullptr)
	, m_pBodyCollider(nullptr)
	, m_bOperation(true)
	, m_iSiblingSwitchIndex(-1)
{
}

CDungeonObject::~CDungeonObject()
{
}

HRESULT CDungeonObject::Initialize(void* _desc)
{
	m_bOnlyCloneComponent = true;

	m_pGameObject->SetLayer(L"DungeonObject");

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
		m_vRenderer = m_pGameObject->CreateMeshHierachy(CResources::LoadMeshBuffersOnScene(m_strObjName + L"_Model (Mesh Buffer)"), m_sDescription.scaleFactor);

		for (TRAVERSAL_ITER(m_vRenderer, it))
			(*it)->Get_Material()->Set_Texture(CResources::LoadOnScene<CTexture>(m_strObjName + L"_BaseMap (Texture)"));
	}
	else
	{
		m_vSkinnedRenderer = m_pGameObject->CreateSkinnedMeshHierachy(CResources::LoadSkinnedMeshBuffersOnScene(m_strObjName + L"_Model (Mesh Buffer)"), CResources::LoadSkinnedBonesOnScene(m_strObjName + L"_Model (SkinnedBuffer)"), m_sDescription.scaleFactor);

		for (TRAVERSAL_ITER(m_vSkinnedRenderer, it))
			(*it)->Get_Material()->Set_Texture(CResources::LoadOnScene<CTexture>(m_strObjName + L"_BaseMap (Texture)"));
	}

	m_pAudioSource = m_pGameObject->AddComponent<CAudioSource>();
	m_pAudioSource->SetLoop(false);
}

void CDungeonObject::Start()
{
}

void CDungeonObject::Update()
{
}

void CDungeonObject::OnDisable()
{
	StopSound();
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

void CDungeonObject::Set_Operation(const _bool _on)
{
	m_bOperation = _on;
}

void CDungeonObject::Set_SibilingIndex(const _int _index)
{
	m_iSiblingSwitchIndex = _index;
}

const _int CDungeonObject::Get_Index() const
{
	return m_iSiblingSwitchIndex;
}

void CDungeonObject::PlaySoundEffect(const wstring _cilp)
{
	CAudioClip* clip = CResources::LoadOnScene<CAudioClip>(_cilp + L" (Audio)");
	m_pAudioSource->SetClip(clip);
	m_pAudioSource->Play();
}

void CDungeonObject::StopSound()
{
	m_pAudioSource->Stop();
}
