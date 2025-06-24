#pragma once

#include "Component.h"

NS_BEGIN(Engine)

class ENGINE_DLL CCamera final : public CComponent
{
public:
	enum ViewMode { PERSPECTIVE, ORTHOGRAPHIC };

protected:
	explicit CCamera();
	~CCamera();

public:
	static CCamera* Create();

public:
	HRESULT Initialize() override;
	void Update() override;

public:
	_matrix Get_ViewMatrix() const;
	_matrix Get_ProjectionMatrix() const;
	const ViewMode Get_ViewMode() const;
	void Set_ViewMode(const ViewMode _mode);
	void SetNear(const _float _value);
	void SetFar(const _float _value);
	const ColorValue& Get_BackgroundColor() const;
	void Set_BackgroundColor(const ColorValue& _color);

private:
	void Bind_ViewMatrix();
	void Bind_ProjectionMatrix();

private:
	ViewMode m_eCamViewMode;
	_matrix m_vViewMatrix, m_vProjMatrix;

	ColorValue m_vBackgroundColor;
	_float m_fNear, m_fFar;
	_float m_fFieldOfView;
	_float m_fSize;
};

NS_END

