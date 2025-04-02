#include "CImage.h"
#include "CResourceManager.h"
#include "CGameObject.h"
#include "CScreen.h"

#pragma comment(lib, "Msimg32.lib")

CImage::CImage(wstring _spriteName)
	: m_strSpriteName(_spriteName)
	, m_sTPColor(ColorValue::Magenta())
	, m_v2SpriteStart(vector2Int::zero())
	, m_v2SpriteEnd(vector2Int::one() * -1.f)
	, m_fFillAmount(vector2::one())
	, m_sRectColor(ColorValue::White())
	, m_fAlpha(1.f)
{
}

CImage::~CImage()
{
	OnDestroy();
}

void CImage::Awake()
{
	Awake_();
}

void CImage::Start()
{
	Start_();
}

void CImage::Reset()
{
    Reset_();
}

void CImage::Update()
{
	Update_();
}

void CImage::FixedUpdate()
{
	FixedUpdate_();
}

void CImage::LateUpdate()
{
	LateUpdate_();
}

void CImage::OnEnable()
{
	OnEnable_();
}

void CImage::Render(HDC _hDC)
{
	if (!m_pCanvas)
		return;

	if (m_fAlpha >= 1.f)
	{
		vector2 pos = m_cRect.getAnchoredPos() + CScreen::GetInstance().getScreenVector().toVector2() * 0.5f;

		if (m_strSpriteName == L"")
		{
			CTransform emptyTP;
			DrawRectangle(_hDC, pos + m_cRect.getPivot(), m_cRect.getSize(), &emptyTP, m_sRectColor, m_sRectColor);

			return;
		}

		HDC	memDC = ::CResourceManager::GetInstance().Find_Sprite(m_strSpriteName);

		vector2Int end = m_v2SpriteEnd;

		if (m_v2SpriteEnd.x < 0)
			end = vector2Int((int)m_cRect.getSize().x, (int)m_cRect.getSize().y);

		GdiTransparentBlt(_hDC,
			int(pos.x - m_cRect.getSize().x * (0.5f * m_fFillAmount.x) + m_cRect.getPivot().x - (m_cRect.getSize().x * 0.5f * (1.f - m_fFillAmount.x))),
			int(pos.y - m_cRect.getSize().y * (0.5f * m_fFillAmount.y) + m_cRect.getPivot().y - (m_cRect.getSize().x * 0.5f * (1.f - m_fFillAmount.y))),
			int(m_cRect.getSize().x * m_pGameObject->getTransform().getScale().x * m_fFillAmount.x),
			int(m_cRect.getSize().y * m_pGameObject->getTransform().getScale().y * m_fFillAmount.y),
			memDC,
			(int)m_v2SpriteStart.x,
			(int)m_v2SpriteStart.y,
			end.x,
			end.y,
			RGB(m_sTPColor.r, m_sTPColor.g, m_sTPColor.b));
	}
	else
	{
        vector2 pos = m_cRect.getAnchoredPos()
            + CScreen::GetInstance().getScreenVector().toVector2() * 0.5f;

        HDC memDC = ::CResourceManager::GetInstance().Find_Sprite(m_strSpriteName);
        if (!memDC)
            return;

        vector2Int end = m_v2SpriteEnd;
        if (end.x < 0 || end.y < 0)
            end = vector2Int((int)m_cRect.getSize().x, (int)m_cRect.getSize().y);

        const int spriteWidth = end.x;
        const int spriteHeight = end.y;

        float fDrawWidth = m_cRect.getSize().x
            * m_pGameObject->getTransform().getScale().x
            * m_fFillAmount.x;
        float fDrawHeight = m_cRect.getSize().y
            * m_pGameObject->getTransform().getScale().y
            * m_fFillAmount.y;

        float fDrawX = pos.x
            - (m_cRect.getSize().x * 0.5f * m_fFillAmount.x)
            + m_cRect.getPivot().x
            - (m_cRect.getSize().x * 0.5f * (1.f - m_fFillAmount.x));
        float fDrawY = pos.y
            - (m_cRect.getSize().y * 0.5f * m_fFillAmount.y)
            + m_cRect.getPivot().y
            - (m_cRect.getSize().y * 0.5f * (1.f - m_fFillAmount.y));

        HDC hTempDC = CreateCompatibleDC(_hDC);

        BITMAPINFO bi{};
        bi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);

        bi.bmiHeader.biWidth = spriteWidth;

        bi.bmiHeader.biHeight = -spriteHeight;
        bi.bmiHeader.biPlanes = 1;
        bi.bmiHeader.biBitCount = 32;
        bi.bmiHeader.biCompression = BI_RGB;

        void* pBits = nullptr;
        HBITMAP hTempBmp = CreateDIBSection(
            hTempDC,
            &bi,
            DIB_RGB_COLORS,
            &pBits,
            nullptr,
            0
        );

        HBITMAP hOldTempBmp = (HBITMAP)SelectObject(hTempDC, hTempBmp);

        // 2) 원본 스프라이트를 임시 DC로 복사 (SRCCOPY)
        //    - 여기서는 GdiTransparentBlt 대신 BitBlt로 "원본 그대로"를 복사합니다.
        BitBlt(
            hTempDC,
            0, 0,
            spriteWidth, spriteHeight,
            memDC,
            m_v2SpriteStart.x,
            m_v2SpriteStart.y,
            SRCCOPY
        );

        // 3) 픽셀을 직접 순회하면서 크로마키 영역은 알파=0, 나머지는 알파=255로 설정
        //    - m_sTPColor와 동일한 색상(RGB)이라면 완전투명(0x00******)으로,
        //      아니면 불투명(0xFF******)으로 설정
        DWORD colorKey = RGB(m_sTPColor.r, m_sTPColor.g, m_sTPColor.b);
        DWORD* pixels = static_cast<DWORD*>(pBits);
        int totalPixelCount = spriteWidth * spriteHeight;

        for (int i = 0; i < totalPixelCount; ++i)
        {
            // 0xAARRGGBB 중 RR, GG, BB만 추출
            DWORD rgbOnly = pixels[i] & 0x00FFFFFF;
            if (rgbOnly == colorKey)
            {
                // 크로마키 영역 -> 완전 투명
                pixels[i] = 0x00000000;
            }
            else
            {
                // 불투명 영역 -> 알파 채널 0xFF
                pixels[i] |= 0xFF000000;
            }
        }

        // 4) 알파 블렌딩(투명도 m_fAlpha 적용)으로 최종 그리기
        //    - per-pixel alpha(AC_SRC_ALPHA) + SourceConstantAlpha(=m_fAlpha*255)
        BLENDFUNCTION bf{};
        bf.BlendOp = AC_SRC_OVER;
        bf.BlendFlags = 0;
        bf.SourceConstantAlpha = static_cast<BYTE>(m_fAlpha * 255.f);
        bf.AlphaFormat = AC_SRC_ALPHA; // 픽셀별 알파값 사용

        AlphaBlend
        (
            _hDC,
            (int)fDrawX,
            (int)fDrawY,
            (int)fDrawWidth,
            (int)fDrawHeight,
            hTempDC,
            0,
            0,
            spriteWidth,
            spriteHeight,
            bf
        );

        // 5) 사용이 끝난 리소스 정리
        SelectObject(hTempDC, hOldTempBmp);
        DeleteObject(hTempBmp);
        DeleteDC(hTempDC);
	}
}

void CImage::OnCollisionEnter(CCollider* _other)
{
}

void CImage::OnCollisionStay(CCollider* _other)
{
}

void CImage::OnCollisionExit(CCollider* _other)
{
}

void CImage::OnDestroy()
{
	OnDestroy_();
}

void CImage::OnMouseEnter()
{
}

void CImage::OnMouseOver()
{
}

void CImage::OnMouseExit()
{
}
