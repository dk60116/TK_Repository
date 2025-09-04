#pragma once

#include "PlayerBehaviour.h"

class CPlayerBehaviour_Ladder : public CPlayerBehaviour
{
	friend class CPlayerController;

private:
	explicit CPlayerBehaviour_Ladder();
	~CPlayerBehaviour_Ladder();

public:
	HRESULT Initialize(class CPlayer* _player) override;

	void Enter(void* _desc = nullptr) override;
	void During() override;
	void Exit() override;

public:
	class CLadder* m_pLadder;

	_byte m_iDirection, m_iPrevDirection;
};

