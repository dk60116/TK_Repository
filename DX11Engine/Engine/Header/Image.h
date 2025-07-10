#pragma once

#include "UI.h"

NS_BEGIN(Engine)

class ENGINE_DLL CImage : public CUI
{
	friend class CGameObject;

private:
	CImage();
	~CImage();

private:
	static CImage* Create();

public:
	HRESULT Initialize() override;
	void Render_Editor() override;
	void Render() override;
	void OnDestroy() override;

private:
	class CTextrue* m_pTexture;
};

NS_END

