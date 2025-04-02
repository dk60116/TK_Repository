#include "CSpriteRenderer.h"
#include "CCamera.h"
#include "CScreen.h"
#include "CSceneManager.h"
#include "CMainProcess.h"
#include "CResourceManager.h"
#include "CAnimator.h"
#include "CTile.h"

#pragma comment(lib, "Msimg32.lib")

extern HWND g_hWnd;
extern bool g_rectRencder;

CSpriteRenderer::CSpriteRenderer()
	: m_eMode(BMP)
	, m_strResourceName(L"")
	, m_pAnimator(nullptr)
	, m_v2StartPoint(vector2Int::zero())
	, m_v2EndPoint(vector2Int::zero())
	, m_sColorTint(ColorValue::White())
	, m_sTPColor(ColorValue::Magenta())
	, m_fScale(1.f)
	, m_fAlpha(1.f)
{
}

CSpriteRenderer::CSpriteRenderer(wstring _resourceName, vector2Int _endPoint, bool _transparent)
	: m_eMode(_transparent ? TBMP : BMP)
	, m_strResourceName(_resourceName)
	, m_pAnimator(nullptr)
	, m_v2EndPoint(_endPoint)
	, m_sColorTint(ColorValue::White())
	, m_sTPColor(ColorValue::Magenta())
	, m_fScale(1.f)
	, m_fAlpha(1.f)
{
}

CSpriteRenderer::CSpriteRenderer(CAnimator* _animator)
	:m_eMode(BMPAnimation)
	, m_strResourceName(L"")
	, m_pAnimator(_animator)
	, m_sColorTint(ColorValue::White())
	, m_sTPColor(ColorValue::Magenta())
	, m_fScale(1.f)
	, m_fAlpha(1.f)
{
	_animator->SetRenderer(this);
}

CSpriteRenderer::~CSpriteRenderer()
{
	OnDestroy();
}

void CSpriteRenderer::Awake()
{
	Awake_();
}

void CSpriteRenderer::Start()
{
	Start_();
}

void CSpriteRenderer::Reset()
{
	Reset_();
}

void CSpriteRenderer::Update()
{
	Update_();
}

void CSpriteRenderer::FixedUpdate()
{
	FixedUpdate_();
}

void CSpriteRenderer::LateUpdate()
{
	LateUpdate_();
}

void CSpriteRenderer::OnEnable()
{
	OnEnable_();
}

void CSpriteRenderer::OnDisable()
{
	OnDisable_();
}

void CSpriteRenderer::Render(HDC _hDC)
{
	if (CSceneManager::GetInstance().getCurrentscene()->getMainCamera() == nullptr)
		return;

	CCamera* mainCam = CSceneManager::GetInstance().getCurrentscene()->getMainCamera();

	vector2 screen = vector2(CScreen::GetInstance().getOptions().width, CScreen::GetInstance().getOptions().height) / (mainCam->getSize());

	if (getTransform().getPosition().x < (int)mainCam->getTransform().getPosition().x - screen.x / UNIT)
		return;
	if (getTransform().getPosition().x > (int)mainCam->getTransform().getPosition().x + screen.x / UNIT)
		return;
	if (getTransform().getPosition().y < (int)mainCam->getTransform().getPosition().y - (screen.y / UNIT) - 1)
		return;
	if (getTransform().getPosition().y > (int)mainCam->getTransform().getPosition().y + (screen.y / UNIT) + 1)
		return;

	switch (m_eMode)
	{
	case RectDraw:
		RenderRect(_hDC);
		break;
	case CircleDraw:
		RenderCircle(_hDC);
		break;
	case BMP:
		RenderBMP(_hDC, m_v2StartPoint, m_v2EndPoint);
		break;
	case TBMP:
		RenderTBMP(_hDC, m_v2StartPoint, m_v2EndPoint);
		break;
	case BMPAnimation:
		RenderBMPAnimation(_hDC);
		break;
	default:
		break;
	}
}

void CSpriteRenderer::OnCollisionEnter(CCollider* _other)
{
}

void CSpriteRenderer::OnCollisionStay(CCollider* _other)
{
}

void CSpriteRenderer::OnCollisionExit(CCollider* _other)
{
}

void CSpriteRenderer::OnDestroy()
{
	OnDestroy_();
}

void CSpriteRenderer::OnMouseEnter()
{
}

void CSpriteRenderer::OnMouseOver()
{
}

void CSpriteRenderer::OnMouseExit()
{
}

void CSpriteRenderer::RenderRect(HDC _hDC)
{
	vector2 camPos = CSceneManager::GetInstance().getCurrentscene()->getMainCamera()->getMove();
	float size = CSceneManager::GetInstance().getCurrentscene()->getMainCamera()->getSize();

	vector2 myScale = m_pGameObject->getTransform().getScale();
	vector2 myPos = CSceneManager::GetInstance().getCurrentscene()->getMainCamera()->getTransform().getPosition() - getTransform().getPosition();
	myPos *= ((size) * UNIT);

	if (m_strResourceName == L"")
	{
		DrawRectangle(_hDC,
			vector2::zero() - (camPos + myPos) - vector2::one(),
			vector2::one() * myScale * UNIT * size + (vector2::one() * 2),
			&getTransform(), m_sColorTint, m_sColorTint);
	}
	else
	DrawTPRectangle(_hDC,
		vector2::zero() - (camPos + myPos) - vector2::one(),
		vector2::one() * myScale * UNIT * size + (vector2::one() * 2),
		&getTransform());
}

void CSpriteRenderer::RenderCircle(HDC _hDC)
{
}

void CSpriteRenderer::RenderBMP(HDC _hDC, vector2Int _start, vector2Int _end)
{
	if (m_strResourceName == L"")
	{
		RenderRect(_hDC);

		return;
	}

	CCamera& cam = *CSceneManager::GetInstance().getCurrentscene()->getMainCamera();

	vector2 camPos = cam.getMove();
	float size = CSceneManager::GetInstance().getCurrentscene()->getMainCamera()->getSize();

	vector2 myScale = m_pGameObject->getTransform().getScale();
	vector2 myPos = cam.getTransform().getPosition() - getTransform().getPosition();
	myPos *= size * UNIT;

	HDC	memDC = ::CResourceManager::GetInstance().Find_Sprite(m_strResourceName);
	
	StretchBlt(_hDC,
		getTransform().getPosition().x - (int)(camPos.x + myPos.x) - (int)(getTransform().getScale().x * UNIT * 0.5f * size) - 1,
		getTransform().getPosition().y - (int)(camPos.y + myPos.y) - (int)(getTransform().getScale().y * UNIT * 0.5f * size) - 1,
		int((UNIT * myScale.x * size + 2) * m_fScale),
		int((UNIT * myScale.y * size + 2) * m_fScale),
		memDC,
		(int)_start.x,
		(int)_start.y,
		(int)_end.x,
		(int)_end.y,
		SRCCOPY);

	if (g_rectRencder)
		RenderRect(_hDC);

	return;

	// === 이미지 중앙에 정수 출력 추가 ===
	int num1 = m_pGameObject->GetComponent<CTile>()->getIndex().x;
	int num2 = m_pGameObject->GetComponent<CTile>()->getIndex().y;
	TCHAR buffer[50];

	wsprintf(buffer, TEXT("%d\ %d"), num1, num2);

	int imgX = (int)getTransform().getPosition().x - (int)(camPos.x + myPos.x) - (int)(getTransform().getScale().x * UNIT * 0.5f * size) + (UNIT * size);
	int imgY = (int)getTransform().getPosition().y - (int)(camPos.y + myPos.y) - (int)(getTransform().getScale().y * UNIT * 0.5f * size) + (UNIT * size);
	int imgWidth = UNIT * myScale.x * size;
	int imgHeight = UNIT * myScale.y * size;

	int centerX = imgX - (int)(imgWidth * 0.5f);
	int centerY = imgY - (int)(imgHeight * 0.5f);

	RECT textRect = { centerX - 50, centerY - 20, centerX + 50, centerY + 20 };

	SetBkMode(_hDC, TRANSPARENT);
	SetTextColor(_hDC, RGB(255, 255, 255));
	DrawText(_hDC, buffer, -1, &textRect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
}

void CSpriteRenderer::RenderTBMP(HDC _hDC, vector2Int _start, vector2Int _end)
{
	if (m_strResourceName == L"")
	{
		RenderRect(_hDC);

		return;
	}

	CCamera& cam = *CSceneManager::GetInstance().getCurrentscene()->getMainCamera();

	vector2 camPos = cam.getMove();
	float size = CSceneManager::GetInstance().getCurrentscene()->getMainCamera()->getSize();

	vector2 pivot = vector2::zero();

	vector2 myScale = m_pGameObject->getTransform().getScale();
	vector2 myPos = cam.getTransform().getPosition() - getTransform().getPosition();
	myPos *= size * UNIT;
	myPos += getTransform().getPivot() * UNIT;

	HDC	memDC = ::CResourceManager::GetInstance().Find_Sprite(m_strResourceName);

	if (m_fAlpha >= 1.f)
	{
		if (m_sColorTint == ColorValue::White())
		{
			GdiTransparentBlt(_hDC,
				getTransform().getPosition().x - (int)(camPos.x + myPos.x) - (int)(getTransform().getScale().x * int(UNIT * 0.5f * size)) - 2,
				getTransform().getPosition().y - (int)(camPos.y + myPos.y) - (int)(getTransform().getScale().y * int(UNIT * 0.5f * size)) - 2,
				int(UNIT * myScale.x * size + 2),
				int(UNIT * myScale.y * size + 2),
				memDC,
				(int)_start.x,
				(int)_start.y,
				(int)_end.x,
				(int)_end.y,
				RGB(m_sTPColor.r, m_sTPColor.g, m_sTPColor.b));
		}
		else
		{
			int posX = int(getTransform().getPosition().x - (camPos.x + myPos.x) - (getTransform().getScale().x * UNIT * size * 0.5f)) - 2;
			int posY = int(getTransform().getPosition().y - (int)(camPos.y + myPos.y) - (int)(getTransform().getScale().y * UNIT * size * 0.5f)) - 2;
			int width = int(UNIT * myScale.x * size + 2);
			int height = int(UNIT * myScale.y * size + 2);

			HDC tempDC = CreateCompatibleDC(_hDC);

			BITMAPINFO bmi = { 0 };
			bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
			bmi.bmiHeader.biWidth = width;
			bmi.bmiHeader.biHeight = -height;
			bmi.bmiHeader.biPlanes = 1;
			bmi.bmiHeader.biBitCount = 32;
			bmi.bmiHeader.biCompression = BI_RGB;

			void* bits = nullptr;
			HBITMAP bmp32 = CreateDIBSection(_hDC, &bmi, DIB_RGB_COLORS, &bits, NULL, 0);
			HBITMAP oldBmp = (HBITMAP)SelectObject(tempDC, bmp32);

			StretchBlt(tempDC, 0, 0, width, height, memDC, (int)_start.x, (int)_start.y, (int)_end.x, (int)_end.y, SRCCOPY);

			DWORD* pixelData = (DWORD*)bits;
			DWORD colKey = RGB(m_sTPColor.r, m_sTPColor.g, m_sTPColor.b);
			for (int i = 0; i < width * height; ++i)
			{
				if ((pixelData[i] & 0x00FFFFFF) == colKey)
				{
					pixelData[i] = 0x00000000;
				}
				else
				{
					DWORD src = pixelData[i];
					DWORD sr = (src >> 16) & 0xFF;
					DWORD sg = (src >> 8) & 0xFF;
					DWORD sb = src & 0xFF;
					sr = (sr * m_sColorTint.r) / 255;
					sg = (sg * m_sColorTint.g) / 255;
					sb = (sb * m_sColorTint.b) / 255;
					pixelData[i] = (0xFF << 24) | (sr << 16) | (sg << 8) | sb;
				}
			}

			BLENDFUNCTION bf;
			bf.BlendOp = AC_SRC_OVER;
			bf.BlendFlags = 0;
			bf.SourceConstantAlpha = 255;
			bf.AlphaFormat = AC_SRC_ALPHA;

			AlphaBlend(_hDC, posX, posY, width, height, tempDC, 0, 0, width, height, bf);

			SelectObject(tempDC, oldBmp);
			DeleteObject(bmp32);
			DeleteDC(tempDC);
		}
	}
	else
	{
		int posX = (int)getTransform().getPosition().x - (int)(camPos.x + myPos.x) - (int)(getTransform().getScale().x * UNIT * size * 0.5f) - 2;
		int posY = (int)getTransform().getPosition().y - (int)(camPos.y + myPos.y) - (int)(getTransform().getScale().y * UNIT * size * 0.5f) - 2;
		int width = (int)(UNIT * myScale.x * size + 2);
		int height = (int)(UNIT * myScale.y * size + 2);

		HDC tempDC = CreateCompatibleDC(_hDC);

		BITMAPINFO bmi = { 0 };
		bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
		bmi.bmiHeader.biWidth = width;
		bmi.bmiHeader.biHeight = -height;
		bmi.bmiHeader.biPlanes = 1;
		bmi.bmiHeader.biBitCount = 32;
		bmi.bmiHeader.biCompression = BI_RGB;

		void* bits = nullptr;
		HBITMAP bmp32 = CreateDIBSection(_hDC, &bmi, DIB_RGB_COLORS, &bits, NULL, 0);
		HBITMAP oldBmp = (HBITMAP)SelectObject(tempDC, bmp32);

		StretchBlt(tempDC, 0, 0, width, height, memDC, (int)_start.x, (int)_start.y, (int)_end.x, (int)_end.y, SRCCOPY);

		DWORD* pixelData = (DWORD*)bits;
		DWORD colKey = RGB(m_sTPColor.r, m_sTPColor.g, m_sTPColor.b);
		for (int i = 0; i < width * height; ++i)
		{
			if ((pixelData[i] & 0x00FFFFFF) == colKey)
				pixelData[i] = 0x00000000;
			else
				pixelData[i] |= 0xFF000000;
		}

		BLENDFUNCTION bf;
		bf.BlendOp = AC_SRC_OVER;
		bf.BlendFlags = 0;
		bf.SourceConstantAlpha = (BYTE)(255.f * m_fAlpha);
		bf.AlphaFormat = AC_SRC_ALPHA;

		AlphaBlend(_hDC, posX, posY, width, height, tempDC, 0, 0, width, height, bf);

		SelectObject(tempDC, oldBmp);
		DeleteObject(bmp32);
		DeleteDC(tempDC);
	}

	if (g_rectRencder)
		RenderRect(_hDC);
}

void CSpriteRenderer::RenderBMPAnimation(HDC _hDC)
{
	CCamera& cam = *CSceneManager::GetInstance().getCurrentscene()->getMainCamera();

	vector2 camPos = cam.getMove();
	float size = CSceneManager::GetInstance().getCurrentscene()->getMainCamera()->getSize();

	vector2 myScale = m_pGameObject->getTransform().getScale();
	vector2 myPos = cam.getTransform().getPosition() - getTransform().getPosition();
	myPos *= size * UNIT;

	vector2 lastPos = vector2(getTransform().getPosition().x - (camPos.x + myPos.x) - (getTransform().getScale().x),
		getTransform().getPosition().y - (camPos.y + myPos.y) - (getTransform().getScale().y));

	m_pAnimator->GetAnimationDisplay(_hDC, m_pGameObject->getTransform(), lastPos, size, m_sTPColor);

	if (g_rectRencder)
		RenderRect(_hDC);
}

void CSpriteRenderer::SetSpriteArea(const vector2Int _start, const vector2Int _end)
{
	m_v2StartPoint = _start;
	m_v2EndPoint = _end;
}
