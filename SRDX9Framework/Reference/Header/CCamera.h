#pragma once

#include "CComponent.h"
#include "CScreen.h"

BEGIN(Engine)

struct CameraViewParameters
{
	float m_fFovY = D3DXToRadian(60.f);
	float m_fAspect = (float)CScreen::GetInstance().getResolution().x / (float)CScreen::GetInstance().getResolution().y;
	float m_fNearZ = 0.1f;
	float m_fFarZ = 600.f;
};

class ENGINE_DLL CCamera :
    public CComponent
{
public:
	enum CameraViewMode { PERSPECTIVE, ORTHOGRAPHIC };

public:
    CCamera();
    ~CCamera();

public:
	virtual void Awake();
	virtual void Start();
	virtual void Update();
	virtual void FixedUpdate();
	virtual void LateUpdate();
	virtual void Render();
	virtual void OnEnable();
	virtual void OnDisable();
	virtual void OnDestroy();

private:
	void UpdateProjectionMatrix();
	void UpdateViewMatrix();
	void ResetAspectFromResolution();

public:
	const CameraViewMode getMode() { return m_eCamViewMode; }
	void SetMode(const CameraViewMode _mode) { m_eCamViewMode = _mode; }
	_matrix& getProjMatrix() { return m_matProjMatrix; }
	_matrix& getViewMatrix() { return m_matViewMatrix; }
	CameraViewParameters& getParameters() { return m_sParameters; };

private:
	CameraViewMode m_eCamViewMode;
	CameraViewParameters m_sParameters;
	_matrix m_matProjMatrix, m_matViewMatrix;
};

END

