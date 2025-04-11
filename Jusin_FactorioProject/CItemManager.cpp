#include "CItemManager.h"

CItemManager::CItemManager()
	: m_mItemList({})
{
}

CItemManager::~CItemManager()
{
	Release();
}

void CItemManager::Init()
{
	CItem& coal = *CreateItem(0, L"Coal", L"Icon_Coal", L"Icon_Coal");
	coal.getInfo().isStructure = false;

	CItem& stone = *CreateItem(1, L"Stone", L"Icon_Stone", L"Icon_Stone");
	stone.getInfo().isStructure = false;

	CItem& iron = *CreateItem(2, L"Iron", L"Icon_Iron", L"Icon_Iron");
	iron.getInfo().isStructure = false;

	CItem& copper = *CreateItem(3, L"Copper", L"Icon_Copper", L"Icon_Copper");
	copper.getInfo().isStructure = false;

	CItem& stoneFurnace = *CreateItem(4, L"StoneFurnace", L"Icon_StoneFurnace", L"Field_StoneFurnace");
	stoneFurnace.AddMaterial(&stone, 1);
	stoneFurnace.getInfo().fieldSize = vector2::one() * 2;
	stoneFurnace.getInfo().blueprintSpriteArea = vector2Int::one() * 128;
	stoneFurnace.getInfo().createDuring = 1.f;

	CItem& stoneBrick = *CreateItem(6, L"StoneBrick", L"Icon_StonBrick", L"Icon_StonBrick");
	stoneBrick.getInfo().isStructure = false;
	//stoneBrick.AddMaterial(&stone, 1);

	CItem& ironPlate = *CreateItem(7, L"IronPlate", L"Icon_IronPlate", L"Icon_IronPlate");
	ironPlate.getInfo().isStructure = false;
	//ironPlate.AddMaterial(&iron, 1);

	// 8. 구리판
	CItem& copperPlate = *CreateItem(8, L"CopperPlate", L"Icon_CopperPlate", L"Icon_CopperPlate");
	copperPlate.getInfo().isStructure = false;
	//copperPlate.AddMaterial(&copper, 1);

	// 9. 톱니바퀴
	CItem& gear = *CreateItem(9, L"Gear", L"Icon_Gear", L"Icon_Gear");
	gear.getInfo().isStructure = false;
	gear.AddMaterial(&ironPlate, 1);
	gear.getInfo().createDuring = 0.5f;

	// 5. 채굴기
	CItem& burnerDrill = *CreateItem(5, L"BurnerDrill", L"Icon_BurnerDrill", L"Field_BurnerDrill");
	burnerDrill.AddMaterial(&ironPlate, 1);
	burnerDrill.AddMaterial(&gear, 3);
	burnerDrill.AddMaterial(&stoneFurnace, 1);
	burnerDrill.getInfo().fieldSize = vector2::one() * 2;
	burnerDrill.getInfo().blueprintSpriteArea = vector2Int::one() * 128;
	burnerDrill.getInfo().createDuring = 1.f;

	// 10. 구리 선
	CItem& copperWire = *CreateItem(10, L"CopperWire", L"Icon_CopperWire", L"Icon_CopperWire");
	copperWire.getInfo().isStructure = false;
	copperWire.AddMaterial(&copperPlate, 1);
	copperWire.getInfo().createDuring = 0.5f;

	// 11. 전자회로

	// 12. 투입기

	// 13. 철 상자

	// 14. 나무 상자

	// 15. 강철 상자

	// 16. 벨트
	CItem& belt = *CreateItem(15, L"Belt", L"Icon_Belt", L"Field_Belt");
	belt.AddMaterial(&ironPlate, 1);
	belt.AddMaterial(&gear, 1);
	belt.getInfo().fieldSize = vector2::one();
	belt.getInfo().blueprintSpriteArea = vector2Int::one() * 64;
	belt.getInfo().createDuring = 1.f;

	// 18. 회로
	CItem& circuit = *CreateItem(18, L"Circuit", L"Icon_Circuit", L"Icon_Circuit");
	ironPlate.getInfo().isStructure = false;
	circuit.AddMaterial(&ironPlate, 1);
	circuit.AddMaterial(&copperWire, 3);
	circuit.getInfo().fieldSize = vector2::one();
	circuit.getInfo().blueprintSpriteArea = vector2Int::one() * 64;
	circuit.getInfo().createDuring = 5.f;

	// 17. 기관총
	CItem& machineGun = *CreateItem(17, L"MachineGun", L"Icon_MachineGun", L"Icon_MachineGun");
	ironPlate.getInfo().isStructure = false;
	machineGun.AddMaterial(&ironPlate, 1);
	machineGun.AddMaterial(&gear, 3);
	machineGun.AddMaterial(&circuit, 2);
	machineGun.AddMaterial(&coal, 3);
	machineGun.getInfo().fieldSize = vector2::one();
	machineGun.getInfo().blueprintSpriteArea = vector2Int::one() * 64;
	machineGun.getInfo().createDuring = 5.f;
}

void CItemManager::Release()
{
	for (TRAVERSAL_ITER(m_mItemList, it))
		Safe_Delete(it->second);

	m_mItemList.clear();
}

CItem* CItemManager::CreateItem(const int _id, const wstring _name, const wstring _iconA, const wstring _iconB)
{
	CItem& newItem = *(new CItem());

	newItem.getInfo().id = _id;
	newItem.getInfo().itemName = _name;
	newItem.getInfo().icon_Slot = _iconA;
	newItem.getInfo().icon_Field = _iconB;

	m_mItemList.insert({ _id, &newItem });

	return m_mItemList[_id];
}

CItem* CItemManager::GetItem(const int _itemId)
{
	return m_mItemList[_itemId];
}