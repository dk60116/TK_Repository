#pragma once

#include "Component.h"
#include "CScreen.h"

BEGIN(Engine)

struct CameraViewParameters
{
	_float size = 1.f;
	_float ascpect = (float)CScreen::GetInstance().getGameResolution().x / (float)CScreen::GetInstance().getGameResolution().y;
};

class ENGINE_DLL CCamera 
	: public CComponent
{
public:
	enum CameraViewMode { PERSPECTIVE, ORTHOGRAPHIC };

protected:
    explicit CCamera();
    ~CCamera();

public:
	void Awake() override;
	void Start() override;
	void UpdateEditor() override;
	void Update() override;
	void FixedUpdate() override;
	void LateUpdate() override;
	void Render() override;
	void OnEnable() override;
	void OnDisable() override;
	void OnDestroy() override;

public:
	static CCamera* Create();

private:
	void UpdateProjectionMatrix();
	void UpdateViewMatrix();

public:
	const CameraViewMode getMode() { return m_eCamViewMode; }
	void SetMode(const CameraViewMode _mode) { m_eCamViewMode = _mode; }
	_matrix& getProjMatrix() { return m_matProjMatrix; }
	_matrix& getViewMatrix() { return m_matViewMatrix; }
	void ViewProjextion();
	CameraViewParameters& getParameters() { return m_sParameters; };
	void ResetAspectFromResolution(const vector2Int _resolution);

private:
	CameraViewMode m_eCamViewMode;
	CameraViewParameters m_sParameters;
	_matrix m_matProjMatrix, m_matViewMatrix;

	_float m_fNear, m_fFar;
	_float m_fFieldOfView;

	BEGIN_SERIALIZEFIELD
	SERIALIZEFIELD(m_fNear)
	SERIALIZEFIELD(m_fFar)
	SERIALIZEFIELD(m_fFieldOfView)
	END_SERIALIZEFIELD
};

END

