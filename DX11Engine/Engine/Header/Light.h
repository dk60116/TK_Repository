#pragma once

#include "Component.h"

NS_BEGIN(Engine)

class ENGINE_DLL CLight final : public CComponent
{
	friend class CGameObject;

public:
	enum class Type : _uint { Directional = 0, point = 1, spot = 2 };

protected:
	explicit CLight();
	~CLight();

private:
	static CLight* Create();
	CComponent* Clone() const override;

public:
	HRESULT Initialize() override;
	void Update() override;
	void Render_Editor() override;
	void Render() override;

	void OnDestroy() override;

public:
	const Type Get_Type() const;
	void Set_Type(const Type _type);
	const _float Get_Intensity() const;
	void Set_Intensity(const _float _value);
	void Set_Range(const _float _value);
	void Set_Color(const ColorValue _color);

	const _float4x4 To_LightInfo();

private:
	Type m_eType;

	_float m_fIntensity, m_fRange, m_fSpotAngle, m_fAttenuation;
	ColorValue m_vDiffuseColor, m_vSpecularColor;
};

NS_END

