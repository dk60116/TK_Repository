#include "CResourceManager.h"

CResourceManager::CResourceManager()
{
}

CResourceManager::~CResourceManager()
{
	Release();
}

void CResourceManager::Init()
{
	Insert_Bmp(L"../Image/Player/Idle.bmp", L"Player_Idle");
	Insert_Bmp(L"../Image/Player/Running.bmp", L"Player_Running");
	Insert_Bmp(L"../Image/Player/Mining.bmp", L"Player_Mining");
	Insert_Bmp(L"../Image/Player/Gun_Idle.bmp", L"Player_Gun_Idle");
	Insert_Bmp(L"../Image/Player/Gun_Run.bmp", L"Player_Gun_Run");
	Insert_Bmp(L"../Image/Player/Gun_Run_Flip.bmp", L"Player_Gun_Run_Flip");

	Insert_Bmp(L"../Image/Player/Idle_Mask.bmp", L"PlayerMask_Idle");

	Insert_Bmp(L"../Image/Player/Shadow_Idle.bmp", L"Shadow_Idle");
	Insert_Bmp(L"../Image/Player/Shadow_Run.bmp", L"Shadow_Run");
	Insert_Bmp(L"../Image/Player/Shadow_Mining.bmp", L"Shadow_Mining");

	Insert_Bmp(L"../Image/Terrain/Dirt_2.bmp", L"Terrain_Dirt_2");
	Insert_Bmp(L"../Image/Terrain/Grass.bmp", L"Terrain_Grass");
	Insert_Bmp(L"../Image/Terrain/Water.bmp", L"Terrain_Water");

	Insert_Bmp(L"../Image/Resource/Coal.bmp", L"Resource_Coal");
	Insert_Bmp(L"../Image/Resource/Stone.bmp", L"Resource_Stone");
	Insert_Bmp(L"../Image/Resource/Iron.bmp", L"Resource_Iron");
	Insert_Bmp(L"../Image/Resource/Copper.bmp", L"Resource_Copper");

	Insert_Bmp(L"../Image/Item/Icon/Coal.bmp", L"Icon_Coal");
	Insert_Bmp(L"../Image/Item/Icon/Stone.bmp", L"Icon_Stone");
	Insert_Bmp(L"../Image/Item/Icon/Iron.bmp", L"Icon_Iron");
	Insert_Bmp(L"../Image/Item/Icon/Copper.bmp", L"Icon_Copper");
	Insert_Bmp(L"../Image/Item/Icon/StoneFurnace.bmp", L"Icon_StoneFurnace");
	Insert_Bmp(L"../Image/Item/Icon/StoneBrick.bmp", L"Icon_StonBrick");
	Insert_Bmp(L"../Image/Item/Icon/IronPlate.bmp", L"Icon_IronPlate");
	Insert_Bmp(L"../Image/Item/Icon/CopperPlate.bmp", L"Icon_CopperPlate");
	Insert_Bmp(L"../Image/Item/Icon/Gear.bmp", L"Icon_Gear");
	Insert_Bmp(L"../Image/Item/Icon/CopperWire.bmp", L"Icon_CopperWire");
	Insert_Bmp(L"../Image/Item/Icon/BurnerDrill.bmp", L"Icon_BurnerDrill");
	Insert_Bmp(L"../Image/Item/Icon/Belt.bmp", L"Icon_Belt");
	Insert_Bmp(L"../Image/Item/Icon/MachineGun.bmp", L"Icon_MachineGun");
	Insert_Bmp(L"../Image/Item/Icon/Circuit.bmp", L"Icon_Circuit");

	Insert_Bmp(L"../Image/Item/Field/Iron.bmp", L"Field_Iron");
	Insert_Bmp(L"../Image/Item/Field/StoneFurnace.bmp", L"Field_StoneFurnace");
	Insert_Bmp(L"../Image/Item/Field/BurnerDrill.bmp", L"Field_BurnerDrill");
	Insert_Bmp(L"../Image/Item/Field/Belt.bmp", L"Field_Belt");

	Insert_Bmp(L"../Image/Item/Field/StoneFurnace_Shadow.bmp", L"StoneFurnace_Shadow");

	Insert_Bmp(L"../Image/Item/Animation/StoneFurnace_Fire.bmp", L"StoneFurnace_Fire");
	Insert_Bmp(L"../Image/Item/Animation/StoneFurnace_Fire_Up.bmp", L"StoneFurnace_Fire_Up");
	Insert_Bmp(L"../Image/Item/Animation/BurnerDrill_Down.bmp", L"BurnerDrill_Down");
	Insert_Bmp(L"../Image/Item/Animation/Belt_Move.bmp", L"Belt_Move");

	Insert_Bmp(L"../Image/UI/MainMenu.bmp", L"UI_MainUI");
	Insert_Bmp(L"../Image/UI/MainBtn.bmp", L"UI_MainBtn");
	Insert_Bmp(L"../Image/UI/MainBtn_Red.bmp", L"UI_MainBtn_Red");
	Insert_Bmp(L"../Image/UI/XButton.bmp", L"UI_XButton");
	Insert_Bmp(L"../Image/UI/Inventory.bmp", L"UI_Inventory");
	Insert_Bmp(L"../Image/UI/GaugeBar_Yellow.bmp", L"UI_GaugeBar_Yellow");
	Insert_Bmp(L"../Image/UI/GaugeBar_Green.bmp", L"UI_GaugeBar_Green");
	Insert_Bmp(L"../Image/UI/GaugeBar_Red.bmp", L"UI_GaugeBar_Red");
	Insert_Bmp(L"../Image/UI/InvenSlot.bmp", L"UI_InvenSlot");
	Insert_Bmp(L"../Image/UI/CreateItemMenu.bmp", L"UI_CreateItemMenu");
	Insert_Bmp(L"../Image/UI/IOStructMenu.bmp", L"UI_IOStructMenu");
	Insert_Bmp(L"../Image/UI/CreateItemBtn.bmp", L"UI_CreateItemBtn");
	Insert_Bmp(L"../Image/UI/CreateItemWating.bmp", L"UI_CreateItemWait");
	Insert_Bmp(L"../Image/UI/CreateItemFill.bmp", L"UI_CreateItemFill");
	Insert_Bmp(L"../Image/UI/GaugeBar_Structure.bmp", L"UI_GaugeBar_Structure");
	Insert_Bmp(L"../Image/UI/GaugeBar_Fuel.bmp", L"UI_GaugeBar_Fuel");
	Insert_Bmp(L"../Image/UI/Ending.bmp", L"UI_Ending");

	Insert_Bmp(L"../Image/Monster/Biter/Biter_Run.bmp", L"Monster_Biter_Run");
	Insert_Bmp(L"../Image/Monster/Biter/Biter_Attack.bmp", L"Monster_Biter_Attack");
	Insert_Bmp(L"../Image/Monster/Biter/Biter_Death.bmp", L"Monster_Biter_Death");

	Insert_Bmp(L"../Image/Particle/Iron_Ore.bmp", L"Ptc_Iron_Ore");
	Insert_Bmp(L"../Image/Particle/Gun_Muzzle.bmp", L"Ptc_Gun_Muzzle");

	/////////////////////////////////////////////////////////////////////////////////////////////////////

	Insert_AudioClip("../Sound/Bgm/beyond-factory-outskirts.ogg", L"BGM1");
	Insert_AudioClip("../Sound/Bgm/world_base_wind.ogg", L"BGM_Wind");

	Insert_AudioClip("../Sound/Sfx/axe-mining-ore-1.ogg", L"SFX_PlayerMining1");

	Insert_AudioClip("../Sound/Sfx/Item.ogg", L"SFX_Item");
	Insert_AudioClip("../Sound/Sfx/Shoot.ogg", L"SFX_PlayerShoot");
}

void CResourceManager::Release()
{
	for (TRAVERSAL_ITER(m_mBmpList, it))
		Safe_Delete((it)->second);

	for (TRAVERSAL_ITER(m_mAuidoList, it))
		Safe_Delete((it)->second);

	m_mBmpList.clear();
	m_mAuidoList.clear();
}

void CResourceManager::Insert_Bmp(const TCHAR* _filePath, const wstring _imgKey)
{
	CBMP* pMyBmp = new CBMP();

	pMyBmp->Load_Bmp(_filePath);

	m_mBmpList.insert({ _imgKey, pMyBmp });
}

void CResourceManager::Insert_AudioClip(const char* _filePath, const wstring _audioKey)
{
	CAudioClip* pMySource = new CAudioClip();

	pMySource->LoadSound(_filePath);

	m_mAuidoList.insert({ _audioKey, pMySource });
}

CBMP* CResourceManager::FindBMP(const wstring _name)
{
	CBMP* result = m_mBmpList[_name];
	return result;
}

HDC CResourceManager::Find_Sprite(const wstring _imgKey)
{
	HDC result = m_mBmpList[_imgKey]->Get_MemDC();
	return result;
}

CAudioClip* CResourceManager::Find_AudioClip(const wstring _audioKey)
{
	CAudioClip* result = m_mAuidoList[_audioKey];
	return result;
}
