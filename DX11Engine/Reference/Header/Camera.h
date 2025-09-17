#pragma once

#include "Component.h"
#include "Physics.h"

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
	CComponent* Clone() const override;

public:
	HRESULT Initialize(void* _desc) override;
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

	void Add_RenderTarget_Mesh(class CRenderer* _mesh);
	void Add_RenderTarget_BlendMesh(class CRenderer* _mesh);
	void Add_RenderTarget_UI(class CUI* _ui);

protected:
	void Bind_ViewMatrix();
	void Bind_ProjectionMatrix();

	void Bind_RenderTarget();

public:
	void RenderMesh();
	void RenderUI();

public:
	CPhysics::Ray ScreenPointToRay(const vector2Int& _pixel, _float _maxDist = 999999.f);
	CPhysics::Ray ScreenPointToRay_Editor(const vector2Int& _pixel, _float _maxDist = 999999.f);

protected:
	ViewMode m_eCamViewMode;
	_float4x4 m_vViewMatrix, m_vProjMatrix;

	_float m_fAspect;
	ColorValue m_vBackgroundColor;
	_float m_fNear, m_fFar;
	_float m_fFieldOfView;
	_float m_fSize;

	vector<CRenderer*> m_vMeshList_Lit, m_vMeshList_Blend;
	vector<CUI*> m_vUIList;

private:
	static const ColorValue s_vDefaultCameraColor;
};

NS_END