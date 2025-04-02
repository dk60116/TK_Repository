#include "CRectTransform.h"
#include "CScreen.h"

CRectTransform::CRectTransform()
	: m_v2AnchoredPosition(vector2::zero())
	, m_v2ChildPosition(vector2::zero())
	, m_v2Size(vector2::one() * 50)
	, m_v2Pivot(vector2::zero())
	, m_v2AnchorPreset(vector2::zero() * 0.5f)
{
}

CRectTransform::~CRectTransform()
{
}

void CRectTransform::SetPosition(const vector2 _pos)
{
	vector2 pos = vector2(_pos.x - CScreen::GetInstance().getOptions().width * 0.5f, _pos.y - CScreen::GetInstance().getOptions().height * 0.5f);

	SetAnchoredPos(pos);
}

vector2& CRectTransform::getPosition()
{
	vector2 result = getAnchoredPos() + vector2(CScreen::GetInstance().getOptions().width * 0.5f, CScreen::GetInstance().getOptions().height * 0.5f);

	return result;
}

const TRect CRectTransform::getTRect()
{
	TRect rect;

	vector2 screen = vector2(CScreen::GetInstance().getOptions().width, CScreen::GetInstance().getOptions().height) * 0.5f;

	rect.left = (int)(m_v2AnchoredPosition.x - m_v2Size.x * 0.5f + screen.x);
	rect.top = (int)(m_v2AnchoredPosition.y - m_v2Size.y * 0.5f + screen.y);
	rect.right = (int)(m_v2AnchoredPosition.x + m_v2Size.x * 0.5f + screen.x);
	rect.bottom = (int)(m_v2AnchoredPosition.y + m_v2Size.y * 0.5f + screen.y);

	return rect;
}
