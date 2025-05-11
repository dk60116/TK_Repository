#include "CMonsterManager.h"

CMonsterManager::CMonsterManager()
{
}

CMonsterManager::~CMonsterManager()
{
	m_lMonsterList.clear();
}

void CMonsterManager::AddMonster(CMonster* _monster)
{
	m_lMonsterList.push_back(_monster);
}

void CMonsterManager::DeleteMonster(CMonster* _monster)
{
	if (!_monster)
		return;

	if (m_lMonsterList.size() <= 0)
		return;

	auto& lMonsterList = m_lMonsterList;

	auto it = find(lMonsterList.begin(), lMonsterList.end(), _monster);

	if (it == lMonsterList.end())
		return;

	lMonsterList.remove(_monster);
}
