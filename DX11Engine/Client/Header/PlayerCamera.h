#pragma once

#include "epch.h"

class CPlayerCamera final : public CComponent
{
public:
	enum class PlayerCamMode { Default, BowAiming };

	struct PlayerCameraOptions
	{
		_float HeightOffset = 5.f;
		_float lookHeightOffset = 3.f;
		_float zoomMin = 2.f;
		_float zoomMax = 10.f;
		_float trackingSpeed = 2.f;
		_float firstZoomSensor = 4.f;
		_float firsBowY = 2.5f;
	};

protected:
	explicit CPlayerCamera();
	~CPlayerCamera();

public:
	static CPlayerCamera* Create();
	CComponent* Clone() const override;

public:
	HRESULT Initialize() override;

	void Awake() override;
	void Start() override;
	void Update() override;
	void LateUpdate() override;
	void OnDestroy() override;

public:
	const PlayerCamMode GetMode() const;
	void ChangeMode(const PlayerCamMode _mode);
	void ResetBowYValue();
	void AddBowYValue(const _float _value);
	const vector3& Get_FinalTargetPos();

private:
	void Look_Default();
	void Look_BowAiming();

private:
	class CPlayer* m_pPlayer;
	PlayerCameraOptions m_sOptions;
	_float m_fBackOffset, m_fZoomSensor, m_fBowYValue;
	PlayerCamMode m_eMode;
	vector3 m_vFinalTargetPos;
};

