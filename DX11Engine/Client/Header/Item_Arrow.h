#pragma once
#include "Item.h"

class CItem_Arrow final : public CItem
{
	friend class CInventory;

	explicit CItem_Arrow();
	~CItem_Arrow();

public:
	void OnGetEvent(const _uint _count) override;
	void OnDropEvent(const _uint _count) override;
	void OnUseEvent() override;
	void OnUnUseEvent() override;
};

