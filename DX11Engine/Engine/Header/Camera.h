#pragma once

#include "Component.h"

NS_BEGIN(Engine)

class ENGINE_DLL CCamera : public CComponent
{
	friend class CGameObject;

public:
	enum ViewMode { PERSPECTIVE, ORTHOGRAPHIC };

protected:
	explicit CCamera();
	~CCamera();

private:
	static CCamera* Create();

public:
	HRESULT Initialize() override;
	void Update() override;
	void Render() override;
	void OnDestroy() override;

public:
	_matrix Get_ViewMatrix() const;
	_matrix Get_ProjectionMatrix() const;
	const ViewMode Get_ViewMode() const;
	void Set_ViewMode(const ViewMode _mode);
	void SetNear(const _float _value);
	void SetFar(const _float _value);
	const ColorValue& Get_BackgroundColor() const;
	void Set_BackgroundColor(const ColorValue& _color);

protected:
	void Bind_ViewMatrix();
	void Bind_ProjectionMatrix();

private:
	void RenderMesh();
	void RenderUI();

protected:
	ViewMode m_eCamViewMode;
	_float4x4 m_vViewMatrix, m_vProjMatrix;

	_float m_fAspect;
	ColorValue m_vBackgroundColor;
	_float m_fNear, m_fFar;
	_float m_fFieldOfView;
	_float m_fSize;

	vector<class CUI*> m_vUIList;
};

NS_END

