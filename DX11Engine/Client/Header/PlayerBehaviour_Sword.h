#pragma once
#include "PlayerBehaviour.h"

class CPlayerBehaviour_Sword final : public CPlayerBehaviour
{
	friend class CPlayerController;

private:
	explicit CPlayerBehaviour_Sword();
	~CPlayerBehaviour_Sword();

public:
	HRESULT Initialize(class CPlayer* _player) override;

	void Enter(void* _desc = nullptr) override;
	void During() override;
	void Exit() override;

private:
	_ubyte m_iCurrentCombo, m_iPrevCombo;
	_bool m_bContinueCombo;
};

