#include "CGameObject.h"
#include "CSceneManager.h"
#include "CCamera.h"
#include "CScene.h"

CGameObject::CGameObject()
	: m_eLayer(Default)
	, m_pMyScene(nullptr)
	, m_strName(L"")
	, m_bActive(true)
	, m_bPrevActive(true)
	, m_bEnable(true)
	, m_sTransform({})
	, m_pParent(nullptr)
	, m_vComponentList({})
	, m_bKill(false)
{
}

CGameObject::CGameObject(CGameObject& _rhs)
	: m_eLayer(_rhs.m_eLayer)
	, m_pMyScene(nullptr)
	, m_strName(_rhs.m_strName)
	, m_bActive(true)
	, m_bPrevActive(true)
	, m_bEnable(true)
	, m_sTransform(_rhs.m_sTransform)
	, m_pParent(nullptr)
	, m_vComponentList(_rhs.m_vComponentList)
	, m_bKill(false)
{
}

CGameObject::~CGameObject()
{
	OnDestroy();
}

void CGameObject::Awake()
{
	for (TRAVERSAL_ITER(m_vComponentList, it))
		(*it)->Awake();

	Start();
}

void CGameObject::Start()
{
	for (TRAVERSAL_ITER(m_vComponentList, it))
		(*it)->Start();
}

void CGameObject::Update()
{
	m_sTransform.SetPosition(m_sTransform.getPosition() + m_sTransform.getTransateDir());

	for (TRAVERSAL_ITER(m_vComponentList, it))
	{
		if (m_bActive && m_bEnable)
		{
			if ((*it)->getEnable())
				(*it)->Update();
		}
	}
}

void CGameObject::FixedUpdate()
{
	for (TRAVERSAL_ITER(m_vComponentList, it))
	{
		if (m_bEnable)
		{
			if ((*it)->getEnable())
				(*it)->FixedUpdate();
		}
	}
}

void CGameObject::LateUpdate()
{
	if (!m_pParent)
		m_bEnable = m_bActive;
	else
	{
		m_bEnable = m_pParent->m_bEnable;
		m_sTransform.SetPosition(m_pParent->m_sTransform.getPosition() + m_sTransform.getChildPosition());
	}

	if (!m_bPrevActive && m_bActive)
		OnEnable();
	else if (m_bPrevActive && !m_bActive)
		OnDisable();

	m_bPrevActive = m_bActive;

	for (TRAVERSAL_ITER(m_vComponentList, it))
		(*it)->LateUpdate();
}

void CGameObject::Render(HDC _hDC)
{
	for (TRAVERSAL_ITER(m_vComponentList, it))
	{
		if (m_bActive && m_bEnable)
		{
			if ((*it)->getEnable())
				(*it)->Render(_hDC);
		}
	}
}

void CGameObject::OnCollisionEnter(CCollider* _other)
{
	for (TRAVERSAL_ITER(m_vComponentList, it))
	{
		if (m_bActive && m_bEnable)
		{
			if ((*it)->getEnable())
				(*it)->OnCollisionEnter(_other);
		}
	}
}

void CGameObject::OnCollisionStay(CCollider* _other)
{
	for (TRAVERSAL_ITER(m_vComponentList, it))
	{
		if (m_bActive && m_bEnable)
		{
			if ((*it)->getEnable())
				(*it)->OnCollisionStay(_other);
		}
	}
}

void CGameObject::OnCollisionExit(CCollider* _other)
{
	for (TRAVERSAL_ITER(m_vComponentList, it))
	{
		if (m_bActive && m_bEnable)
		{
			if ((*it)->getEnable())
				(*it)->OnCollisionExit(_other);
		}
	}
}

void CGameObject::OnEnable()
{
	for (TRAVERSAL_ITER(m_lChildList, it))
	{
		(*it)->OnEnable();

		for (TRAVERSAL_ITER((*it)->m_lChildList, it2))
		{
			(*it2)->OnEnable();

			for (TRAVERSAL_ITER((*it2)->m_lChildList, it3))
			{
				(*it3)->OnEnable();

				for (TRAVERSAL_ITER((*it3)->m_lChildList, it4))
					(*it4)->OnEnable();
			}
		}
	}

	for (TRAVERSAL_ITER(m_vComponentList, it))
	{
		(*it)->OnEnable();
	}
}

void CGameObject::OnDisable()
{
	for (TRAVERSAL_ITER(m_lChildList, it))
	{
		(*it)->OnDisable();

		for (TRAVERSAL_ITER((*it)->m_lChildList, it2))
		{
			(*it2)->OnDisable();

			for (TRAVERSAL_ITER((*it2)->m_lChildList, it3))
			{
				(*it3)->OnDisable();

				for (TRAVERSAL_ITER((*it3)->m_lChildList, it4))
					(*it4)->OnDisable();
			}
		}
	}

	for (TRAVERSAL_ITER(m_vComponentList, it))
	{
		(*it)->OnDisable();
	}
}

void CGameObject::OnDestroy()
{
	for (TRAVERSAL_ITER(m_vComponentList, it))
		(*it)->OnDestroy();

	for (TRAVERSAL_ITER(m_vComponentList, it))
		Safe_Delete(*it);

	m_vComponentList.clear();
}

void CGameObject::MessageUpdate()
{
}

CMonoBehaviour* CGameObject::AddComponent(CMonoBehaviour* _component)
{
	m_vComponentList.push_back(_component);

	if (dynamic_cast<CCamera*>(_component))
		CSceneManager::GetInstance().getCurrentscene()->AddCamera(dynamic_cast<CCamera*>(_component));

	m_vComponentList.back()->SetObject(this);
	m_vComponentList.back()->Awake();

	return m_vComponentList.back();
}

void CGameObject::Destroy()
{
	m_bKill = true;

	for (TRAVERSAL_ITER(m_lChildList, it))
	{
		(*it)->Destroy();
	}
}

void CGameObject::SetActive(const bool _enable)
{
	if (!m_pParent)
		m_bEnable = _enable;
	
	m_bActive = _enable;
}

void CGameObject::SetParent(CGameObject* _parent)
{
	m_pParent = _parent;

	if (find(_parent->m_lChildList.begin(), _parent->m_lChildList.end(), this) == _parent->m_lChildList.end())
		_parent->m_lChildList.push_back(this);
}
