#pragma once

#include "Component.h"

NS_BEGIN(Engine)

class ENGINE_DLL CLight final : public CComponent
{
	friend class CGameObject;

public:
	enum class LightType : _uint { Directional = 0, point = 1, spot = 2 };

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
	const LightType Get_Type() const;
	void Set_Type(const LightType _type);
	const _float Get_Intensity() const;
	void Set_Intensity(const _float _value);
	void Set_Range(const _float _value);
	void Set_Color(const ColorValue _color);

	const _float4x4 To_LightInfo();

public:
	const _matrix& Get_ShadowView() const;
	const _matrix& Get_ShadowProj() const;
	const _matrix& Get_ShadowViewProj() const;

private:
	void UpdateShadowCameraMatrices();

public:
	static _vector SafeUpFromDir(_vector dir);

private:
	LightType m_eType;

	_float m_fIntensity, m_fRange, m_fSpotAngle, m_fAttenuation;
	ColorValue m_vDiffuseColor, m_vSpecularColor;

private:
	_bool m_bCastShadow;
	_matrix m_mShadowView;
	_matrix m_mShadowProj;
	_matrix m_mShadowViewProj;
};

NS_END

