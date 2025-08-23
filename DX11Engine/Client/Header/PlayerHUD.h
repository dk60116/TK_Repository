#pragma once

#include "Component.h"

class CPlayerHUD final : public CComponent
{
public:
	struct PlayerHUDOptions
	{
		_float heartSize = 30.f;
		_float heartSpacing = 5.f;
		_float bowHairSize_Max = 100.f;
		_float bowHairSize_Min = 50.f;
	};

private:
	explicit CPlayerHUD();
	~CPlayerHUD();

public:
	static CPlayerHUD* Create();
	CComponent* Clone() const override;

public:
	HRESULT Initialize() override;

	void Awake() override;
	void Start() override;
	void Update() override;
	void OnDestroy() override;

public:
	void Update_Heart(const _int _current, const _int _max);
	void OnOffBowCrossHair(const _bool _on);

private:
	CPlayer* m_pPlayer;
	CCanvas* m_pCanvas;
	PlayerHUDOptions m_sOptions;
	CRectTransform* m_pHeartContainer;
	vector<CImage*> m_vHeartBowlList, m_vHeartImageList;
	CImage* m_pArrowCrossHair;
	_float m_fACHAlpha;
};

