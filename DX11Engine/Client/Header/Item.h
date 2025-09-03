#pragma once
#include "Object.h"

class CItem abstract : public UObject
{
	friend class CInventory;

protected:
	explicit CItem();
	~CItem();

public:
	virtual void OnGetEvent(const _uint _count);
	virtual void OnDropEvent(const _uint _count);
	virtual void OnUseEvent();
	virtual void OnUnUseEvent();

public:
	const wstring& Get_ItemName();

protected:
	wstring m_strItemName;
};

