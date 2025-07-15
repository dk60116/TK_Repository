#pragma once

#include "Component.h"

NS_BEGIN(Engine)

class ENGINE_DLL CLight final : public CComponent
{
	friend class CGameObject;

public:
	enum class Type { Directional, point, spot };

protected:
	explicit CLight();
	~CLight();

private:
	static CLight* Create();

public:
	HRESULT Initialize() override;
	void Update() override;
	void Render_Editor() override;
	void Render() override;

	void OnDestroy() override;

public:
	const Type Get_Type() const;
	void Set_Type(const Type _type);
	const _float Get_Intencity() const;
	void Set_Intencity(const _float _value);
	void Set_Color(const ColorValue _color);

private:
	Type m_eType;

	_float m_fIntencity, m_fRange;
	ColorValue m_vDiffuseColor, m_vSpecularColor;
};

NS_END

