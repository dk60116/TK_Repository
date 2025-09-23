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
	virtual void OnUseEvent(const _uint _count);
	virtual void OnUnUseEvent(const _uint _count);

public:
	const wstring& Get_ItemName();
	const wstring& Get_ItemDescription();

protected:
	wstring m_strItemName, m_strItemDescription;
};

