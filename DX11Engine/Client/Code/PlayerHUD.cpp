#include "cpch.h"
#include "PlayerHUD.h"

CPlayerHUD::CPlayerHUD()
	: m_pPlayer(nullptr)
	, m_pCanvas(nullptr)
	, m_sOptions({})
	, m_pHeartContainer(nullptr)
	, m_vHeartBowlList({})
	, m_vHeartImageList({})
	, m_pArrowCrossHair(nullptr)
	, m_pRupeeIcon(nullptr)
	, m_pRupeeText(nullptr)
	, m_fACHAlpha(1.f)
	, m_vEquipSlotImage({})
	, m_vEquipIconImage()
{
}

CPlayerHUD::~CPlayerHUD()
{
}

CPlayerHUD* CPlayerHUD::Create()
{
	return new CPlayerHUD();
}

CComponent* CPlayerHUD::Clone() const
{
	return new CPlayerHUD();
}

HRESULT CPlayerHUD::Initialize()
{
	m_pCanvas = m_pGameObject->AddComponent<CCanvas>();

	SpawnHeartBowl();
	SpawnEquipSlot();
	SpawnRupeeUI();
	SpawnBowCrossHair();

	CGameManager::GetInstance().Set_PlayerHUD(this);

	return S_OK;
}

void CPlayerHUD::Awake()
{
	m_pPlayer = CGameManager::GetInstance().Get_Player();

	m_pHeartContainer->Set_Pivot(0.f, 1.f);
	m_pHeartContainer->Set_AnchorsMin(0.f, 1.f);
	m_pHeartContainer->Set_AnchoredPosition(20.f, -20.f);
	m_pHeartContainer->Set_WidthHeight(m_sOptions.heartSize * CPlayer::StaticPlayerStatus::HPMAX + m_sOptions.heartSpacing * CPlayer::StaticPlayerStatus::HPMAX, m_sOptions.heartSize);

	for (size_t i = 0; i < m_vHeartBowlList.size(); ++i)
	{
		CRectTransform* bowlRT = m_vHeartBowlList[i]->Get_RectTransform();
		bowlRT->Set_PivotX(0.f);
		bowlRT->Set_AnchorsMin(0.f, 0.5f);
		bowlRT->Set_WidthHeight(m_sOptions.heartSize, m_sOptions.heartSize);
		bowlRT->Set_AnchoredPositonX(m_sOptions.heartSize * i + m_sOptions.heartSpacing * i);

		CRectTransform* heartRT = m_vHeartImageList[i]->Get_RectTransform();
		bowlRT->Set_WidthHeight(m_sOptions.heartSize, m_sOptions.heartSize);
	}
}

void CPlayerHUD::Start()
{
	Update_Heart(3, 3);
}

void CPlayerHUD::Update()
{
	if (m_pArrowCrossHair->Get_GameObject()->ActiveSelf())
	{
		m_fACHAlpha = Lerp(m_fACHAlpha, 1.f, DELTA_TIME);
		m_pArrowCrossHair->SetAlpha(m_fACHAlpha);

		_float size = m_sOptions.bowHairSize_Max - m_fACHAlpha * m_sOptions.bowHairSize_Min;
		m_pArrowCrossHair->Get_RectTransform()->Set_WidthHeight(size, size);
	}
}

void CPlayerHUD::OnDestroy()
{
}

void CPlayerHUD::Update_Heart(const _int _current, const _int _max)
{
	if (_current < 0)
		return;

	_float targetHp = _current / 2.f;
	_int other = static_cast<_int>(_current) % 2;

	for (_int i = 0; i < static_cast<_int>(m_vHeartBowlList.size()); ++i)
	{
		m_vHeartBowlList[i]->Get_GameObject()->SetActive(i < static_cast<_int>(_max * 0.5f));
		m_vHeartImageList[i]->Get_GameObject()->SetActive(i < targetHp);
		m_vHeartImageList[i]->SetFillAmount(1.f);
	}

	m_vHeartImageList[static_cast<_int>(targetHp)]->SetFillAmount(other > 0.f ? 0.5f : 0.f);
}

void CPlayerHUD::OnOffBowCrossHair(const _bool _on)
{
	m_pArrowCrossHair->Get_GameObject()->SetActive(_on);

	m_fACHAlpha = 0.f;
}

void CPlayerHUD::SpawnHeartBowl()
{
	CScene* crtScene = m_pGameObject->Get_Scene();

	CGameObject* heartContainerObj = crtScene->Add_GameObject(L"Hearts");
	m_pHeartContainer = heartContainerObj->AddComponent<CUI>()->Get_RectTransform();
	m_pHeartContainer->SetParent(m_pCanvas->Get_RectTransform());

	CTexture* heartTex = CResources::LoadOnScene<CTexture>(L"HeartBar (Texture)");

	for (_uint i = 0; i < CPlayer::StaticPlayerStatus::HPMAX; ++i)
	{
		const wstring bowlName = wstring(L"Heart bowl ") + L"(" + to_wstring(i) + L")";
		CGameObject* heartBowlObj = crtScene->Add_GameObject(bowlName);
		m_vHeartBowlList.push_back(heartBowlObj->AddComponent<CImage>());
		m_vHeartBowlList.back()->Get_RectTransform()->SetParent(m_pHeartContainer);
		m_vHeartBowlList.back()->SetTexture(heartTex);
		m_vHeartBowlList.back()->SetColor(ColorValue::gray(0.5f));

		const wstring heartName = wstring(L"Heart") + L"(" + to_wstring(i) + L")";
		CGameObject* heartObj = crtScene->Add_GameObject(heartName);
		m_vHeartImageList.push_back(heartObj->AddComponent<CImage>());
		m_vHeartImageList.back()->Get_RectTransform()->SetParent(m_vHeartBowlList.back()->Get_RectTransform());
		m_vHeartImageList.back()->SetTexture(heartTex);
		m_vHeartImageList.back()->SetColor(ColorValue::red());
	}
}

void CPlayerHUD::SpawnBowCrossHair()
{
	CGameObject* bcObj = m_pGameObject->Get_Scene()->Add_GameObject(L"Bow Cross Hair");
	m_pArrowCrossHair = bcObj->AddComponent<CImage>();
	bcObj->Get_Transform()->SetParent(m_pCanvas->Get_RectTransform());
	m_pArrowCrossHair->SetTexture(CResources::LoadOnScene<CTexture>(L"ArrowCrossHair (Texture)"));
	m_pArrowCrossHair->Get_RectTransform()->Set_WidthHeight(70, 70);
	bcObj->SetActive(false);
}

void CPlayerHUD::SpawnEquipSlot()
{
	CScene* crtScene = m_pGameObject->Get_Scene();

	CTexture* slotImage = CResources::LoadOnScene<CTexture>(L"EquipCountSlot (Texture)");

	CGameObject* equpSlotBox = crtScene->Add_GameObject(L"Equp Slots");
	CRectTransform* slotBoxRect = equpSlotBox->AddComponent<CUI>()->Get_RectTransform();
	slotBoxRect->SetParent(m_pCanvas->Get_RectTransform());

	slotBoxRect->Set_Pivot(1.f, 1.f);
	slotBoxRect->Set_AnchorsMin(1.f, 1.f);
	slotBoxRect->Set_AnchoredPosition(-50.f, -50.f);

	for (_uint i = 0; i < 4; ++i)
	{
		CGameObject* slotObj = crtScene->Add_GameObject(L"Eqip Slot (" + to_wstring(i) + L')');
		m_vEquipSlotImage.push_back(slotObj->AddComponent<CImage>());
		slotObj->Get_Transform()->SetParent(slotBoxRect);
		m_vEquipSlotImage.back()->SetTexture(slotImage);
		m_vEquipSlotImage.back()->Get_RectTransform()->Set_WidthHeight(50, 50);

		CGameObject* iconObj = crtScene->Add_GameObject(L"Eqip Icon");
		m_vEquipIconImage.push_back(iconObj->AddComponent<CImage>());
		iconObj->Get_Transform()->SetParent(slotObj->Get_Transform());
		m_vEquipIconImage.back()->Get_RectTransform()->Set_WidthHeight(m_vEquipSlotImage.back()->Get_RectTransform()->Get_WidthHeight());
		iconObj->SetActive(false);
	}

	const _float m_fSpacing = 40.f;

	m_vEquipSlotImage[0]->Get_RectTransform()->Set_AnchoredPosition(0.f, m_fSpacing);
	m_vEquipSlotImage[1]->Get_RectTransform()->Set_AnchoredPosition(-m_fSpacing, 0.f);
	m_vEquipSlotImage[2]->Get_RectTransform()->Set_AnchoredPosition(m_fSpacing, 0.f);
	m_vEquipSlotImage[3]->Get_RectTransform()->Set_AnchoredPosition(0.f, -m_fSpacing);

	m_vEquipIconImage[0]->SetTexture(CResources::LoadOnScene<CTexture>(L"Arrow_Icon (Texture)"));
	m_vEquipIconImage[0]->Get_GameObject()->SetActive(true);
}

void CPlayerHUD::SpawnRupeeUI()
{
	CScene* crtScene = m_pGameObject->Get_Scene();

	CGameObject* rupeeIconObj = crtScene->Add_GameObject(L"RupeeIcon");
	m_pRupeeIcon = rupeeIconObj->AddComponent<CImage>();
	rupeeIconObj->Get_Transform()->SetParent(Get_Transform());
	m_pRupeeIcon->SetTexture(CResources::LoadOnScene<CTexture>(L"RupeeIcon (Texture)"));
	m_pRupeeIcon->Get_RectTransform()->Set_Pivot(1.f, 0.f);
	m_pRupeeIcon->Get_RectTransform()->Set_AnchorsMin(1.f, 0.f);
	m_pRupeeIcon->Get_RectTransform()->Set_AnchoredPosition(-30.f, 30.f);
	m_pRupeeIcon->Get_RectTransform()->Set_WidthHeight(45, 45);

	CGameObject* rupeeTextObj = crtScene->Add_GameObject(L"Rupee Count Text");
	m_pRupeeText = rupeeTextObj->AddComponent<CText>();
	m_pRupeeText->Get_Transform()->SetParent(Get_Transform());
	m_pRupeeText->Set_FontSize(7);
	m_pRupeeText->Get_RectTransform()->Set_Pivot(1.f, 0.f);
	m_pRupeeText->Get_RectTransform()->Set_AnchorsMin(1.f, 0.f);
	m_pRupeeText->Get_RectTransform()->Set_AnchoredPosition(-80.f, 72.f);
	m_pRupeeText->SetColor(ColorValue::white());
	m_pRupeeText->Set_Text(L"000");
}
