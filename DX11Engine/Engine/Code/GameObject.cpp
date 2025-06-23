#include "epch.h"
#include "GameObject.h"

CGameObject::CGameObject(const wstring _name, ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
	: m_pDevice(_pDevice)
	, m_pContext(_pContext)
	, m_strName(L"")
	, m_bActive(true)
	, m_lComponentList({})
	, m_pScene(nullptr)
	, m_pTransform(nullptr)
{
	m_pDevice->AddRef();
	m_pContext->AddRef();
}

CGameObject::CGameObject(const CGameObject& _rhs)
	: m_pDevice(_rhs.m_pDevice)
	, m_pContext(_rhs.m_pContext)
	, m_strName(_rhs.m_strName)
	, m_bActive(_rhs.m_bActive)
	, m_lComponentList(_rhs.m_lComponentList)
	, m_pTransform(_rhs.m_pTransform)
{
}

CGameObject::~CGameObject()
{
	OnDestroy();
}

HRESULT CGameObject::Initialize()
{
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
		Safe_Release(*it);

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

list<CComponent*>& CGameObject::Get_ComponentList()
{
	return m_lComponentList;
}

CTransform* CGameObject::Get_Transfrom() const
{
	return m_pTransform;
}

