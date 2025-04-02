#include "CCanvas.h"
#include "CGameObject.h"

CCanvas::CCanvas()
	: m_lUiList({})
{
}

CCanvas::~CCanvas()
{
	OnDestroy();
}

void CCanvas::Awake()
{
	Awake_();
}

void CCanvas::Start()
{
	Start_();
}

void CCanvas::Reset()
{
	Reset_();
}

void CCanvas::Update()
{
	Update_();

	for (TRAVERSAL_ITER(m_lUiList, it))
	{
		if (!(*it)->getObj()->getParent())
		{
			if ((*it)->getObj()->isActive() && (*it)->getObj()->isEnable())
			{
				(*it)->getObj()->Update();
				(*it)->UI_Update();
			}
		}
		else
		{
			if ((*it)->getObj()->isActive() && (*it)->getObj()->isEnable())
			{
				(*it)->getObj()->Update();
				(*it)->UI_Update();
			}
		}
	}
}

void CCanvas::FixedUpdate()
{
	FixedUpdate_();
}

void CCanvas::LateUpdate()
{
	LateUpdate_();

	for (TRAVERSAL_ITER(m_lUiList, it))
	{
		(*it)->getObj()->LateUpdate();
		(*it)->UI_LateUpdate();
	}
}

void CCanvas::OnEnable()
{
	OnEnable_();
}

void CCanvas::OnDisable()
{
	OnDisable_();
}

void CCanvas::Render(HDC _hDC)
{
	for (TRAVERSAL_ITER(m_lUiList, it))
	{
		if ((*it)->getObj()->isActive() && (*it)->getObj()->isEnable())
			(*it)->Render(_hDC);
	}
}

void CCanvas::OnCollisionEnter(CCollider* _other)
{
}

void CCanvas::OnCollisionStay(CCollider* _other)
{
}

void CCanvas::OnCollisionExit(CCollider* _other)
{
}

void CCanvas::OnDestroy()
{
	OnDestroy_();

	for (TRAVERSAL_ITER(m_lUiList, it))
	{
		if ((*it)->getObj() != nullptr)
		{
			delete((*it)->getObj());
		}
	}

	m_lUiList.clear();
}

void CCanvas::OnMouseEnter()
{
}

void CCanvas::OnMouseOver()
{
}

void CCanvas::OnMouseExit()
{
}

void CCanvas::InsertUI(CUI* _ui)
{
	if (std::find(m_lUiList.begin(), m_lUiList.end(), _ui) == m_lUiList.end())
	{
		m_lUiList.push_back(_ui);
		_ui->SetCanvas(this);
		_ui->Awake();
	}
}
