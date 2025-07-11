#include "epch.h"
#include "GameObject.h"

CGameObject::CGameObject(const wstring _name, ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
	: m_iUniqueID(999999)
	, m_strGameObjectName(L"")
	, m_bActive(true)
	, m_lComponentList({})
	, m_pScene(nullptr)
	, m_pTransform(nullptr)
	, m_pDevice(_pDevice)
	, m_pContext(_pContext)
{
	m_strName = L"Game Object";
	m_pDevice->AddRef();
	m_pContext->AddRef();
}

CGameObject::CGameObject(const CGameObject& _rhs)
	: m_iUniqueID(999999)
	, m_strGameObjectName(_rhs.m_strGameObjectName)
	, m_bActive(_rhs.m_bActive)
	, m_lComponentList(_rhs.m_lComponentList)
	, m_pScene(_rhs.m_pScene)
	, m_pTransform(_rhs.m_pTransform)
	, m_pDevice(_rhs.m_pDevice)
	, m_pContext(_rhs.m_pContext)
{
	m_iUniqueID = CSceneManager::GetInstance().Get_CrtScene()->Get_UniqueObjectCount();
}

CGameObject::~CGameObject()
{
	OnDestroy();
}

HRESULT CGameObject::Initialize()
{
	if (!GetComponent<CTransform>())
		m_pTransform = AddComponent<CTransform>();

	return S_OK;
}

void CGameObject::Awake()
{
	for (TRAVERSAL_ITER(m_lComponentList, it))
	{
		if ((*it)->Get_Enable())
			(*it)->Awake();
	}
}

void CGameObject::Start()
{
	for (TRAVERSAL_ITER(m_lComponentList, it))
	{
		if ((*it)->Get_Enable())
			(*it)->Start();
	}
}

void CGameObject::Update_Editor()
{
	for (TRAVERSAL_ITER(m_lComponentList, it))
	{
		if ((*it)->Get_Enable())
			(*it)->Update_Editor();
	}
}

void CGameObject::Update()
{
	for (TRAVERSAL_ITER(m_lComponentList, it))
	{
		if ((*it)->Get_Enable())
			(*it)->Update();
	}

	if (GetComponent<CRectTransform>())
		int a = 0;
}

void CGameObject::FixedUpdate()
{
	for (TRAVERSAL_ITER(m_lComponentList, it))
	{
		if ((*it)->Get_Enable())
			(*it)->FixedUpdate();
	}
}

void CGameObject::LateUpdate()
{
	for (TRAVERSAL_ITER(m_lComponentList, it))
	{
		if ((*it)->Get_Enable())
			(*it)->LateUpdate();
	}
}

void CGameObject::LateUpdate_Editor()
{
	for (TRAVERSAL_ITER(m_lComponentList, it))
	{
		if ((*it)->Get_Enable())
			(*it)->LateUpdate_Editor();
	}
}

void CGameObject::OnMouseEnter()
{
	for (TRAVERSAL_ITER(m_lComponentList, it))
	{
		if ((*it)->Get_Enable())
			(*it)->OnMouseEnter();
	}
}

void CGameObject::OnMouseOver()
{
	for (TRAVERSAL_ITER(m_lComponentList, it))
	{
		if ((*it)->Get_Enable())
			(*it)->OnMouseOver();
	}
}

void CGameObject::OnMouseExit()
{
	for (TRAVERSAL_ITER(m_lComponentList, it))
	{
		if ((*it)->Get_Enable())
			(*it)->OnMouseExit();
	}
}

void CGameObject::OnMouseDown()
{
	for (TRAVERSAL_ITER(m_lComponentList, it))
	{
		if ((*it)->Get_Enable())
			(*it)->OnMouseDown();
	}
}

void CGameObject::OnMouseDrag()
{
	for (TRAVERSAL_ITER(m_lComponentList, it))
	{
		if ((*it)->Get_Enable())
			(*it)->OnMouseDrag();
	}
}

void CGameObject::OnMouseUp()
{
	for (TRAVERSAL_ITER(m_lComponentList, it))
	{
		if ((*it)->Get_Enable())
			(*it)->OnMouseUp();
	}
}

void CGameObject::OnPreCull_Editor()
{
	for (TRAVERSAL_ITER(m_lComponentList, it))
	{
		if ((*it)->Get_Enable())
			(*it)->OnPreCull_Editor();
	}
}

void CGameObject::OnPreRender_Editor()
{
	for (TRAVERSAL_ITER(m_lComponentList, it))
	{
		if ((*it)->Get_Enable())
			(*it)->OnPreRender_Editor();
	}
}

void CGameObject::Render_Editor()
{
	for (TRAVERSAL_ITER(m_lComponentList, it))
	{
		if ((*it)->Get_Enable())
			(*it)->Render_Editor();
	}
}

void CGameObject::OnPostRender_Editor()
{
	for (TRAVERSAL_ITER(m_lComponentList, it))
	{
		if ((*it)->Get_Enable())
			(*it)->OnPostRender_Editor();
	}
}

void CGameObject::OnPreCull()
{
	for (TRAVERSAL_ITER(m_lComponentList, it))
	{
		if ((*it)->Get_Enable())
			(*it)->OnPreCull();
	}
}

void CGameObject::OnPreRender()
{
	for (TRAVERSAL_ITER(m_lComponentList, it))
	{
		if ((*it)->Get_Enable())
			(*it)->OnPreRender();
	}
}

void CGameObject::Render()
{
	for (TRAVERSAL_ITER(m_lComponentList, it))
	{
		if ((*it)->Get_Enable())
			(*it)->Render();
	}
}

void CGameObject::OnPostRender()
{
	for (TRAVERSAL_ITER(m_lComponentList, it))
	{
		if ((*it)->Get_Enable())
			(*it)->OnPostRender();
	}
}

void CGameObject::Render_Gizmo()
{
	for (TRAVERSAL_ITER(m_lComponentList, it))
		(*it)->Render_Gizmo();
}

void CGameObject::OnEnable()
{
	for (TRAVERSAL_ITER(m_lComponentList, it))
	{
		if ((*it)->Get_Enable())
			(*it)->OnEnable();
	}
}

void CGameObject::OnDisable()
{
	for (TRAVERSAL_ITER(m_lComponentList, it))
	{
		if ((*it)->Get_Enable())
			(*it)->OnDisable();
	}
}

void CGameObject::OnDestroy()
{
	for (TRAVERSAL_ITER(m_lComponentList, it))
	{
		(*it)->OnDestroy();
		Safe_Release(*it);
	}

	m_lComponentList.clear();
}

void CGameObject::OnApplicationQuit()
{
	for (TRAVERSAL_ITER(m_lComponentList, it))
	{
		if ((*it)->Get_Enable())
			(*it)->OnApplicationQuit();
	}
}

const _bool CGameObject::IsActive() const
{
	return m_bActive;
}

void CGameObject::SetActive(const _bool _active)
{
	m_bActive = _active;
}

list<CComponent*>& CGameObject::Get_ComponentList()
{
	return m_lComponentList;
}

CTransform* CGameObject::Get_Transform() const
{
	return m_pTransform;
}

void CGameObject::Set_Transform(CTransform* _transform)
{
	m_lComponentList.remove(m_pTransform);
	Safe_Release(m_pTransform);
	m_pTransform = nullptr;

	m_pTransform = _transform;

	if (m_pTransform)
	{
		m_lComponentList.push_back(m_pTransform);
		m_pTransform->AddRef();
	}
}

const _uint CGameObject::Get_UniqueID() const
{
	return m_iUniqueID;
}

wstring CGameObject::Get_ObjectName() const
{
	return m_strGameObjectName;
}

wstring CGameObject::Get_ObjectNameID() const
{
	return m_strGameObjectName + L"[" + to_wstring(m_iUniqueID) + L"]";
}

void CGameObject::Set_ObjectName(wstring& _name)
{
	m_strGameObjectName = _name;
}

void CGameObject::Set_Scene(CScene* _scene)
{
	m_pScene = _scene;
}

CScene* CGameObject::Get_Scene()
{
	return m_pScene;
}

