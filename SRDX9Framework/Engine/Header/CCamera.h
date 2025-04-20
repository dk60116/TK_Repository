#pragma once

#include "CComponent.h"
#include "CScreen.h"

BEGIN(Engine)

struct CameraViewParameters
{
	float fov = D3DXToRadian(60.f);
	float size = 1.f;
	float ascpect = (float)CScreen::GetInstance().getGameResolution().x / (float)CScreen::GetInstance().getGameResolution().y;
	float nearZ = 0.1f;
	float farZ = 600.f;
};

class ENGINE_DLL CCamera 
	: public CComponent
{
private:
	enum CameraViewMode { PERSPECTIVE, ORTHOGRAPHIC };

public:
    explicit CCamera();
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

public:
	const CameraViewMode getMode() { return m_eCamViewMode; }
	void SetMode(const CameraViewMode _mode) { m_eCamViewMode = _mode; }
	_matrix& getProjMatrix() { return m_matProjMatrix; }
	_matrix& getViewMatrix() { return m_matViewMatrix; }
	CameraViewParameters& getParameters() { return m_sParameters; };
	void ResetAspectFromResolution(const vector2Int& _resolution);

private:
	CameraViewMode m_eCamViewMode;
	CameraViewParameters m_sParameters;
	_matrix m_matProjMatrix, m_matViewMatrix;
};

END

