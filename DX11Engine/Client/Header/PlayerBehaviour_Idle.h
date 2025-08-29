#pragma once
#include "PlayerBehaviour.h"

class CPlayerBehaviour_Idle final : public CPlayerBehaviour
{
	friend class CPlayerController;

private:
	explicit CPlayerBehaviour_Idle();
	~CPlayerBehaviour_Idle();

public:
	HRESULT Initialize(class CPlayer* _player) override;

	void Enter(void* _desc = nullptr) override;
	void During() override;
	void Exit() override;

private:
	_bool m_bCombatMode;
};

