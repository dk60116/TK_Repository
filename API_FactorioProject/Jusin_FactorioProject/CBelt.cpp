#include "CBelt.h"
#include "CGameObject.h"
#include "CSceneManager.h"
#include "CBeltManager.h"
#include "CInput.h"
#include "CMapManager.h"

CBelt::CBelt()
	: m_pBodyAnimator(nullptr)
	, m_pSideAnmA(nullptr)
	, m_pSideAnmB(nullptr)
	, m_pAdjacentTiles()
	, m_pAdjacentBelts()
{
}

CBelt::~CBelt()
{
	OnDestroy();
}

void CBelt::Awake()
{
	Awake_();
	m_eDirType = EIGHT;
}

void CBelt::Start()
{
	Start_();

	StructureStart();

	m_pCollider->getObj()->SetActive(false);

	m_pRenderer->SetEnable(false);

	CGameObject* animObj = m_pGameObject->getScene()->AddObject(L"Belt_MoveAnim", Layer::Structure);
	m_pBodyAnimator = dynamic_cast<CAnimator*>(animObj->AddComponent(new CAnimator()));
	animObj->AddComponent(new CSpriteRenderer(m_pBodyAnimator));
	animObj->SetParent(m_pGameObject);
	animObj->getTransform().SetScale(vector2(2.f, 1.8f));
	m_pBodyAnimator->SetEnable(false);

	int anmCount = 16;
	vector2Int anmSlice = vector2Int::one() * 64;

	m_pBodyAnimator->CreateAnimation(L"MoveRight", L"Belt_Move", anmCount, vector2::zero(), anmSlice, 0);
	m_pBodyAnimator->CreateAnimation(L"MoveLeft", L"Belt_Move", anmCount, vector2::zero(), anmSlice, 1);
	m_pBodyAnimator->CreateAnimation(L"MoveUp", L"Belt_Move", anmCount, vector2::zero(), anmSlice, 2);
	m_pBodyAnimator->CreateAnimation(L"MoveDown", L"Belt_Move", anmCount, vector2::zero(), anmSlice, 3);
	m_pBodyAnimator->CreateAnimation(L"Left_Up", L"Belt_Move", anmCount, vector2::zero(), anmSlice, 4);
	m_pBodyAnimator->CreateAnimation(L"Down_Right", L"Belt_Move", anmCount, vector2::zero(), anmSlice, 5);
	m_pBodyAnimator->CreateAnimation(L"Right_Up", L"Belt_Move", anmCount, vector2::zero(), anmSlice, 6);
	m_pBodyAnimator->CreateAnimation(L"Down_Left", L"Belt_Move", anmCount, vector2::zero(), anmSlice, 7);
	m_pBodyAnimator->CreateAnimation(L"Up_Right", L"Belt_Move", anmCount, vector2::zero(), anmSlice, 8);
	m_pBodyAnimator->CreateAnimation(L"Left_Down", L"Belt_Move", anmCount, vector2::zero(), anmSlice, 9);
	m_pBodyAnimator->CreateAnimation(L"Up_Left", L"Belt_Move", anmCount, vector2::zero(), anmSlice, 10);
	m_pBodyAnimator->CreateAnimation(L"Right_Down", L"Belt_Move", anmCount, vector2::zero(), anmSlice, 11);
	m_pBodyAnimator->SetAnimation(L"MoveRight");

	CGameObject* sideObjA = m_pGameObject->getScene()->AddObject(L"Belt_SideAnimA", Layer::Structure);
	m_pSideAnmA = dynamic_cast<CAnimator*>(sideObjA->AddComponent(new CAnimator));
	sideObjA->AddComponent(new CSpriteRenderer(m_pSideAnmA));
	sideObjA->SetParent(animObj);
	sideObjA->getTransform().SetChildPosition(vector2(-1.f, 0.f));
	sideObjA->getTransform().SetScale(animObj->getTransform().getScale());

	m_pSideAnmA->CreateAnimation(L"Up_DownEmpty", L"Belt_Move", anmCount, vector2::zero(), anmSlice, 12);
	m_pSideAnmA->CreateAnimation(L"Down_DownEmpty", L"Belt_Move", anmCount, vector2::zero(), anmSlice, 13);
	m_pSideAnmA->CreateAnimation(L"Right_LeftEmpty", L"Belt_Move", anmCount, vector2::zero(), anmSlice, 14);
	m_pSideAnmA->CreateAnimation(L"Left_LeftEmpty", L"Belt_Move", anmCount, vector2::zero(), anmSlice, 15);
	m_pSideAnmA->CreateAnimation(L"Down_UpEmpty", L"Belt_Move", anmCount, vector2::zero(), anmSlice, 16);
	m_pSideAnmA->CreateAnimation(L"Up_UpEmpty", L"Belt_Move", anmCount, vector2::zero(), anmSlice, 17);
	m_pSideAnmA->CreateAnimation(L"Left_RightEmpty", L"Belt_Move", anmCount, vector2::zero(), anmSlice, 18);
	m_pSideAnmA->CreateAnimation(L"Right_RightEmpty", L"Belt_Move", anmCount, vector2::zero(), anmSlice, 19);

	m_pSideAnmA->SetAnimation(L"Right_RightEmpty");
	m_pSideAnmA->SetEnable(false);

	sideObjA->SetActive(false);

	CGameObject* sideObjB = m_pGameObject->getScene()->AddObject(L"Belt_SideAnimB", Layer::Structure);
	m_pSideAnmB = dynamic_cast<CAnimator*>(sideObjB->AddComponent(new CAnimator));
	sideObjB->AddComponent(new CSpriteRenderer(m_pSideAnmB));
	sideObjB->SetParent(animObj);
	sideObjB->getTransform().SetChildPosition(vector2(1.f, 0.f));
	sideObjB->getTransform().SetScale(animObj->getTransform().getScale());

	m_pSideAnmB->CreateAnimation(L"Up_DownEmpty", L"Belt_Move", anmCount, vector2::zero(), anmSlice, 12);
	m_pSideAnmB->CreateAnimation(L"Down_DownEmpty", L"Belt_Move", anmCount, vector2::zero(), anmSlice, 13);
	m_pSideAnmB->CreateAnimation(L"Right_LeftEmpty", L"Belt_Move", anmCount, vector2::zero(), anmSlice, 14);
	m_pSideAnmB->CreateAnimation(L"Left_LeftEmpty", L"Belt_Move", anmCount, vector2::zero(), anmSlice, 15);
	m_pSideAnmB->CreateAnimation(L"Down_UpEmpty", L"Belt_Move", anmCount, vector2::zero(), anmSlice, 16);
	m_pSideAnmB->CreateAnimation(L"Up_UpEmpty", L"Belt_Move", anmCount, vector2::zero(), anmSlice, 17);
	m_pSideAnmB->CreateAnimation(L"Left_RightEmpty", L"Belt_Move", anmCount, vector2::zero(), anmSlice, 18);
	m_pSideAnmB->CreateAnimation(L"Right_RightEmpty", L"Belt_Move", anmCount, vector2::zero(), anmSlice, 19);

	m_pSideAnmB->SetAnimation(L"Right_LeftEmpty");
	m_pSideAnmB->SetEnable(false);

	sideObjB->SetActive(false);

	CBeltManager::GetInstance().AddBelt(this);

	Reset();
}

void CBelt::Reset()
{
	Reset_();

	UpdateAdjacentTiles();
	UpdateSideAnimations();
}

void CBelt::Update()
{
	Update_();
}

void CBelt::FixedUpdate()
{
	FixedUpdate_();
}

void CBelt::LateUpdate()
{
	LateUpdate_();
}

void CBelt::OnEnable()
{
	OnEnable_();
}

void CBelt::OnDisable()
{
	OnDisable_();
}

void CBelt::Render(HDC _hDC)
{
	m_pBodyAnimator->getCurrentAnimationFrame();
}

void CBelt::OnCollisionEnter(CCollider* _other)
{
}

void CBelt::OnCollisionStay(CCollider* _other)
{
}

void CBelt::OnCollisionExit(CCollider* _other)
{
}

void CBelt::OnDestroy()
{
	OnDestroy_();

	CBeltManager::GetInstance().DeleteBelt(this);
}

void CBelt::OnMouseEnter()
{
}

void CBelt::OnMouseOver()
{
}

void CBelt::OnMouseExit()
{
}

void CBelt::UpdateAdjacentTiles()
{
	CTileMap& map = *CMapManager::GetInstance().getMainTile();

	vector2Int myInex = m_vTile[0]->getIndex();

	m_pAdjacentTiles[RIGHT] = map(myInex.x + 1, myInex.y);
	m_pAdjacentTiles[LEFT] = map(myInex.x - 1, myInex.y);
	m_pAdjacentTiles[UP] = map(myInex.x, myInex.y - 1);
	m_pAdjacentTiles[DOWN] = map(myInex.x, myInex.y + 1);

	for (size_t i = 0; i < COUNT; ++i)
		m_pAdjacentBelts[i] = dynamic_cast<CBelt*>(m_pAdjacentTiles[i]->getHaveStructure());
}

void CBelt::UpdateSideAnimations()
{

	switch (m_eDir)
	{
	case RIGHT:
		m_pBodyAnimator->getTransform().SetScale(vector2(2.f, 1.8f));
	case LEFT:
		m_pBodyAnimator->getTransform().SetScale(vector2(2.f, 1.8f));
		break;
	case UP:
		m_pBodyAnimator->getTransform().SetScale(vector2(1.8f, 2.f));
	case DOWN:
		m_pBodyAnimator->getTransform().SetScale(vector2(1.8f, 2.f));
		break;
	case COUNT:
		break;
	default:
		break;
	}

	if (m_eDir == UP)
	{
		if (!m_pAdjacentBelts[DOWN] ||
			m_pAdjacentBelts[DOWN]->m_eDir == LEFT ||
			m_pAdjacentBelts[DOWN]->m_eDir == RIGHT)
		{
			m_pSideAnmA->getObj()->SetActive(true);
			m_pSideAnmA->SetAnimation(L"Up_DownEmpty");
			m_pSideAnmA->getTransform().SetScale(vector2(1.8f, 2.f));
			m_pSideAnmA->getTransform().SetChildPosition(vector2::down() * 0.97f);
		}
		else
		{
			m_pSideAnmA->getObj()->SetActive(false);
		}

		if (!m_pAdjacentBelts[UP] ||
			m_pAdjacentBelts[UP]->m_eDir == LEFT ||
			m_pAdjacentBelts[UP]->m_eDir == RIGHT)
		{
			m_pSideAnmB->getObj()->SetActive(true);
			m_pSideAnmB->SetAnimation(L"Up_UpEmpty");
			m_pSideAnmB->getTransform().SetScale(vector2(1.8f, 2.f));
			m_pSideAnmB->getTransform().SetChildPosition(vector2::up() * 0.97f);
		}
		else
		{
			m_pSideAnmB->getObj()->SetActive(false);
		}
	}
	else if (m_eDir == DOWN)
	{
		if (!m_pAdjacentBelts[UP] ||
			m_pAdjacentBelts[UP]->m_eDir == LEFT ||
			m_pAdjacentBelts[UP]->m_eDir == RIGHT)
		{
			m_pSideAnmA->getObj()->SetActive(true);
			m_pSideAnmA->SetAnimation(L"Down_UpEmpty");
			m_pSideAnmA->getTransform().SetScale(vector2(1.8f, 2.f));
			m_pSideAnmA->getTransform().SetChildPosition(vector2::up() * 0.97f);
		}
		else
		{
			m_pSideAnmA->getObj()->SetActive(false);
		}

		if (!m_pAdjacentBelts[DOWN] ||
			m_pAdjacentBelts[DOWN]->m_eDir == LEFT ||
			m_pAdjacentBelts[DOWN]->m_eDir == RIGHT)
		{
			m_pSideAnmB->getObj()->SetActive(true);
			m_pSideAnmB->SetAnimation(L"Down_DownEmpty");
			m_pSideAnmB->getTransform().SetScale(vector2(1.8f, 2.f));
			m_pSideAnmB->getTransform().SetChildPosition(vector2::down() * 0.97f);
		}
		else
		{
			m_pSideAnmB->getObj()->SetActive(false);
		}
	}
	else if (m_eDir == RIGHT)
	{
		if (!m_pAdjacentBelts[LEFT] ||
			m_pAdjacentBelts[LEFT]->m_eDir == UP ||
			m_pAdjacentBelts[LEFT]->m_eDir == DOWN)
		{
			m_pSideAnmA->getObj()->SetActive(true);
			m_pSideAnmA->SetAnimation(L"Right_LeftEmpty");
			m_pSideAnmA->getTransform().SetScale(vector2(2.f, 1.8f));
			m_pSideAnmA->getTransform().SetChildPosition(vector2::left() * 0.97f);
		}
		else
		{
			m_pSideAnmA->getObj()->SetActive(false);
		}

		if (!m_pAdjacentBelts[RIGHT] ||
			m_pAdjacentBelts[RIGHT]->m_eDir == UP ||
			m_pAdjacentBelts[RIGHT]->m_eDir == DOWN)
		{
			m_pSideAnmB->getObj()->SetActive(true);
			m_pSideAnmB->SetAnimation(L"Right_RightEmpty");
			m_pSideAnmB->getTransform().SetScale(vector2(2.f, 1.8f));
			m_pSideAnmB->getTransform().SetChildPosition(vector2::right() * 0.97f);
		}
		else
		{
			m_pSideAnmB->getObj()->SetActive(false);
		}
	}
	else if (m_eDir == LEFT)
	{
		if (!m_pAdjacentBelts[RIGHT] ||
			m_pAdjacentBelts[RIGHT]->m_eDir == UP ||
			m_pAdjacentBelts[RIGHT]->m_eDir == DOWN)
		{
			m_pSideAnmA->getObj()->SetActive(true);
			m_pSideAnmA->SetAnimation(L"Left_RightEmpty");
			m_pSideAnmA->getTransform().SetScale(vector2(2.f, 1.8f));
			m_pSideAnmA->getTransform().SetChildPosition(vector2::right() * 0.97f);
		}
		else
		{
			m_pSideAnmA->getObj()->SetActive(false);
		}

		if (!m_pAdjacentBelts[LEFT] ||
			m_pAdjacentBelts[LEFT]->m_eDir == UP ||
			m_pAdjacentBelts[LEFT]->m_eDir == DOWN)
		{
			m_pSideAnmB->getObj()->SetActive(true);
			m_pSideAnmB->SetAnimation(L"Left_LeftEmpty");
			m_pSideAnmB->getTransform().SetScale(vector2(2.f, 1.8f));
			m_pSideAnmB->getTransform().SetChildPosition(vector2::left() * 0.97f);
		}
		else
		{
			m_pSideAnmB->getObj()->SetActive(false);
		}
	}

	if (m_eDir == UP)
	{
		if (m_pAdjacentBelts[DOWN] && m_pAdjacentBelts[DOWN]->m_iDir == 4)
			m_pSideAnmA->getObj()->SetActive(false);

		if (m_pAdjacentBelts[UP] && m_pAdjacentBelts[UP]->m_iDir == 8)
		{
			m_pSideAnmB->getObj()->SetActive(false);
		}
	}

	if (m_iDir < 4)	
		m_pBodyAnimator->SetAnimation(m_eDir);
	else
	{
		m_pBodyAnimator->SetAnimation(m_iDir);
		m_pBodyAnimator->getTransform().SetScale(vector2::one() * 1.875f);
		m_pSideAnmA->getObj()->SetActive(false);
		m_pSideAnmB->getObj()->SetActive(false);
	}
}

void CBelt::OnOutputHandler()
{
}


