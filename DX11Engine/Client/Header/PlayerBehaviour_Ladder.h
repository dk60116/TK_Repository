#pragma once

#include "PlayerBehaviour.h"

class PlayerBehaviour_Ladder : public CPlayerBehaviour
{
	friend class CPlayerController;

private:
	explicit PlayerBehaviour_Ladder();
	~PlayerBehaviour_Ladder();

public:
	HRESULT Initialize(class CPlayer* _player) override;

	void Enter(void* _desc = nullptr) override;
	void During() override;
	void Exit() override;
};

