#pragma once

#include "pch.h"
#include "CMonster.h"

class CMonsterManager
{
	SingletonClass(CMonsterManager);

public:
	list<CMonster*>& getMonsterList() { return m_lMonsterList; }
	void AddMonster(CMonster* _monster);
	void DeleteMonster(CMonster* _monster);

private:
	list<CMonster*> m_lMonsterList;
};

