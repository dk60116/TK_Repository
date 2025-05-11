#include "CObjHpSlot.h"
#include "CGameObject.h"
#include "CSceneManager.h"

CObjHpSlot::CObjHpSlot(CGameObject* _obj)
	: m_pTargetObj(_obj)
	, m_pBox(nullptr)
	, m_vSlotList({})
	, m_fFill(0.f)
	, m_sOption({})
{
}

CObjHpSlot::~CObjHpSlot()
{
	OnDestroy();
}

void CObjHpSlot::Awake()
{
	Awake_();

	float slotSpacing = 0.25f;

	CGameObject* boxObj = m_pGameObject->getScene()->AddObject(m_pTargetObj->getName()+  L"HpSlot Box", Layer::UI);
	m_pBox = dynamic_cast<CSpriteRenderer*>(boxObj->AddComponent(new CSpriteRenderer(L"", vector2Int::one() * 32)));
	m_pBox->SetColorTint(ColorValue::Black());
	boxObj->getTransform().SetScale(vector2(slotSpacing * (float)m_sOption.maxSlotCount + slotSpacing * 0.25f, slotSpacing));
	boxObj->getTransform().SetChildPosition(vector2::down() * 1.f);
	boxObj->SetParent(m_pGameObject);

	for (int i = 0; i < m_sOption.maxSlotCount; ++i)
	{
		CGameObject* slotObj = m_pGameObject->getScene()->AddObject(m_pTargetObj->getName() + L"_HpSlot_" + to_wstring(i), Layer::UI);
		CSpriteRenderer* render = dynamic_cast<CSpriteRenderer*>(slotObj->AddComponent(new CSpriteRenderer(L"", vector2Int::one() * 32)));
		m_vSlotList.push_back(render);
		slotObj->SetParent(boxObj);
		float centerOffset = (m_sOption.maxSlotCount - 1) * 0.5f;
		float xPos = (i - centerOffset) * slotSpacing;
		slotObj->getTransform().SetChildPosition(vector2(xPos, 0.f));
		slotObj->getTransform().SetScale(vector2::one() * 0.15f);
		m_vSlotList.back()->SetColorTint(ColorValue::Green());
	}

	m_pBox->getObj()->SetActive(false);
}

void CObjHpSlot::Start()
{
	Start_();
}

void CObjHpSlot::Reset()
{
	Reset_();
}

void CObjHpSlot::Update()
{
	Update_();
}

void CObjHpSlot::FixedUpdate()
{
	FixedUpdate_();
}

void CObjHpSlot::LateUpdate()
{
	LateUpdate_();

	for (TRAVERSAL_ITER(m_vSlotList, it))
		(*it)->getObj()->SetActive(m_pBox->getObj()->isActive());
}

void CObjHpSlot::OnEnable()
{
	OnEnable_();
}

void CObjHpSlot::OnDisable()
{
	OnDisable_();
}

void CObjHpSlot::Render(HDC _hDC)
{
}

void CObjHpSlot::OnCollisionEnter(CCollider* _other)
{
}

void CObjHpSlot::OnCollisionStay(CCollider* _other)
{
}

void CObjHpSlot::OnCollisionExit(CCollider* _other)
{
}

void CObjHpSlot::OnDestroy()
{
}

void CObjHpSlot::OnMouseEnter()
{
}

void CObjHpSlot::OnMouseOver()
{
}

void CObjHpSlot::OnMouseExit()
{
}

void CObjHpSlot::UpdateFill(const float _value)
{
	m_fFill = _value;

	ColorValue _color = ColorValue::Green();

	if (m_fFill > 0.75f)
		_color = ColorValue::Green();
	else if (m_fFill > 0.25f)
		_color = ColorValue::Yellow();
	else
		_color = ColorValue::Red();

	int count = static_cast<int>(std::ceil(_value * (float)m_sOption.maxSlotCount));

	m_pBox->getObj()->SetActive(m_fFill > 0.f);

	for (int i = 0; i < m_vSlotList.size(); ++i)
	{
		if (i < count)
			m_vSlotList[i]->SetColorTint(_color);
		else
			m_vSlotList[i]->SetColorTint(ColorValue::Gray());
	}
}
