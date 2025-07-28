#pragma once

#include "epch.h"

class CPlayerCamera final : public CComponent
{
public:
	struct PlayerCameraOptions
	{
		_float HeightOffset = 5.f;
		_float lookHeightOffset = 3.f;
		_float zoomMin = 2.f;
		_float zoomMax = 10.f;
	};

protected:
	CPlayerCamera();
	~CPlayerCamera();

public:
	static CPlayerCamera* Create();
	HRESULT Initialize() override;

	void Awake() override;
	void Start() override;
	void Update() override;
	void LateUpdate() override;
	void OnDestroy() override;

private:
	class CPlayer* m_pPlayer;
	PlayerCameraOptions m_sOptions;
	_float m_fBackOffset, m_fZoomSensor;
};

