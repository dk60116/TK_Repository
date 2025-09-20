#pragma once
#include "Item.h"

class CItem_Heart final : public CItem
{
	friend class CInventory;

	explicit CItem_Heart();
	~CItem_Heart();

public:
	void OnGetEvent(const _uint _count) override;
	void OnDropEvent(const _uint _count) override;
	void OnUseEvent() override;
	void OnUnUseEvent() override;
};

