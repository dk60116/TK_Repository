#pragma once

#include "UI.h"

NS_BEGIN(Engine)

class ENGINE_DLL CText final : public CUI
{
	friend class CGameObject;

private:
	CText();
	~CText();

private:
	static CText* Create();

public:
	HRESULT Initialize() override;
	void Render_Editor() override;
	void Render() override;
	void OnDestroy() override;

private:
	wstring m_strText;
};

NS_END

