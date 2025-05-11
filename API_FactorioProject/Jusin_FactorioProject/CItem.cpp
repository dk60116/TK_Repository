#include "CItem.h"
#include "CItemManager.h"
#include "CItemSlot.h"

CItem::CItem()
	: m_sInfo({})
	, m_vMaterials({})
{
}

CItem::CItem(CItem& _rhs)
    : m_sInfo(_rhs.m_sInfo)
    , m_vMaterials(_rhs.m_vMaterials)
{
}

CItem::~CItem()
{
    m_vMaterials.clear();
}

void CItem::AddMaterial(CItem* _item, int _count)
{
	m_vMaterials.push_back(IMaterial({ _item->getInfo().id, _count}));
}

vector<IMaterial> CItem::getMaterialsAll()
{
	vector<IMaterial> resultList = {};

	for (int i = 0; i < m_vMaterials.size(); ++i)
	{
        resultList.push_back(m_vMaterials[i]);
	}

	return resultList;
}

NeedItemResult CItem::getMaterialsOfItemList(vector<CItem*> _items)
{
    vector<IMaterial> haveList;
    vector<IMaterial> needList;

    // 아무 아이템도 안 갖고 있으면, 모든 재료가 필요함
    if (_items.empty())
    {
        for (const auto& material : m_vMaterials)
        {
            needList.push_back({ material.item, material.count });
        }
        return { haveList, needList };
    }

    // 모든 재료에 대해 검사
    for (const auto& material : m_vMaterials)
    {
        CItem* requiredItem = CItemManager::GetInstance().GetItem(material.item);

        //// 복합 재료인 경우 (재귀처리 고려 가능)
        //if (!requiredItem || !requiredItem->m_vMaterials.empty())
        //{
        //    // TODO: 복합 재료 처리 로직
        //    continue;
        //}

        // 보유 아이템에서 해당 재료를 찾음
        bool found = false;
        for (const auto& ownedItem : _items)
        {
            if (!ownedItem) continue;

            if (ownedItem->getInfo().id == material.item)
            {
                found = true;
                int diff = material.count - ownedItem->getInfo().count;

                if (diff <= 0)
                {
                    haveList.push_back({ material.item, material.count });
                }
                else
                {
                    needList.push_back({ material.item, diff });
                }

                break; // 찾았으면 더 찾을 필요 없음
            }
        }

        // 아예 해당 아이템을 가지고 있지 않음
        if (!found)
        {
            needList.push_back({ material.item, material.count });
        }
    }

    return { haveList, needList };

    //vector<IMaterial> haveList;
    //vector<IMaterial> needList;

    //// 아무 아이템도 안 갖고 있으면, 모든 재료가 필요함
    //if (_items.empty())
    //{
    //    for (const auto& material : m_vMaterials)
    //    {
    //        needList.push_back({ material.item, material.count });
    //    }
    //    return { haveList, needList };
    //}

    //unordered_map<int, int> totalNeeded;  // itemID -> totalCount
    //unordered_map<int, int> ownedMap;     // itemID -> ownedCount

    //// 소유한 아이템 정리
    //for (auto* item : _items)
    //{
    //    if (!item) continue;
    //    int id = item->getInfo().id;
    //    int count = item->getInfo().count;
    //    ownedMap[id] += count;
    //}

    //// 재귀적으로 재료 누적
    //unordered_set<int> visited;
    //function<void(CItem*, int)> accumulateMaterials;
    //accumulateMaterials = [&](CItem* item, int multiplier)
    //    {
    //        if (!item || visited.count(item->getInfo().id)) return;
    //        visited.insert(item->getInfo().id);

    //        if (item->m_vMaterials.empty())
    //        {
    //            totalNeeded[item->getInfo().id] += multiplier;
    //            return;
    //        }

    //        for (const auto& material : item->m_vMaterials)
    //        {
    //            CItem* matItem = CItemManager::GetInstance().GetItem(material.item);
    //            accumulateMaterials(matItem, material.count * multiplier);
    //        }
    //    };

    //// 현재 아이템을 기준으로 재료 누적
    //accumulateMaterials(this, 1);

    //// 결과 리스트 정리
    //for (const auto& pair : totalNeeded)
    //{
    //    int itemID = pair.first;
    //    int requiredCount = pair.second;

    //    int ownedCount = ownedMap[itemID];
    //    if (ownedCount >= requiredCount)
    //    {
    //        haveList.push_back({ itemID, requiredCount });
    //    }
    //    else
    //    {
    //        if (ownedCount > 0)
    //            haveList.push_back({ itemID, ownedCount });

    //        needList.push_back({ itemID, requiredCount - ownedCount });
    //    }
    //}

    //return { haveList, needList };
}
