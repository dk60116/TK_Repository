#pragma once

#include "Transform.h"

NS_BEGIN(Engine)

class ENGINE_DLL CRectTransform final : public CTransform
{
	friend class CGameObject;

protected:
	CRectTransform();
	~CRectTransform();

private:
	static CRectTransform* Create();

private:
	_int m_iWidth, m_iHeight;
};

NS_END

