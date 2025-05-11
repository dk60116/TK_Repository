#include "CAnimation.h"
#include "CAnimator.h"
#include "CResourceManager.h"

#pragma comment(lib, "Msimg32.lib")

CAnimation::CAnimation()
    : m_hMemDC(NULL)
    , m_pAnimator(nullptr)
    , m_iFrameCount(0)
    , m_iCurrentFrame(0)
    , m_v2CurrentPos(vector2::zero())
    , m_v2Offset(vector2::zero())
    , m_v2xyLength(vector2Int::zero())
    , m_iYvalue(0)
    , m_bLoop(true)
    , m_fFrameInterval(0.1f)
    , m_fSpeed(1.0)
{
}

CAnimation::~CAnimation()
{
}

void CAnimation::Init(CAnimator* _animator, HDC _image, int _frameCount, vector2 _offset, vector2Int _xyLength, int _yValue, float _frameInterval, bool _loop)
{
    m_pAnimator = _animator;
    m_hMemDC = _image;
    m_iFrameCount = _frameCount;
    m_v2Offset = _offset;
    m_v2xyLength = _xyLength;
    m_iYvalue = _yValue;
    m_fFrameInterval = _frameInterval;
    m_bLoop = _loop;
}

void CAnimation::DisplayFrame(HDC& _hDC, CTransform& _tf, vector2 _position, float _size, const ColorValue _tpColor)
{
    if (m_pAnimator->m_pRenderer->getAlpha() >= 1.f)
    {
        GdiTransparentBlt
        (
            _hDC,
            int((_position.x - UNIT * 0.5f * _tf.getScale().x * _size) + m_v2Offset.x * UNIT * _size),
            int((_position.y - UNIT * 0.5f * _tf.getScale().y * _size) + m_v2Offset.y * UNIT * _size),
            (int)(UNIT * _tf.getScale().x * _size),
            (int)(UNIT * _tf.getScale().y * _size),
            m_hMemDC,
            (int)(m_iCurrentFrame * m_v2xyLength.x),
            (int)(m_iYvalue * m_v2xyLength.y),
            (int)(m_v2xyLength.x),
            (int)(m_v2xyLength.y),
            RGB(_tpColor.r, _tpColor.g, _tpColor.b)
        );

        return;
    }
    else
    {
        int destX = static_cast<int>(
            (_position.x - UNIT * 0.5f * _tf.getScale().x * _size)
            + (m_v2Offset.x * UNIT * _size)
            );
        int destY = static_cast<int>(
            (_position.y - UNIT * 0.5f * _tf.getScale().y * _size)
            + (m_v2Offset.y * UNIT * _size)
            );
        int destW = static_cast<int>(UNIT * _tf.getScale().x * _size);
        int destH = static_cast<int>(UNIT * _tf.getScale().y * _size);

        int srcX = static_cast<int>(m_iCurrentFrame * m_v2xyLength.x);
        int srcY = static_cast<int>(m_iYvalue * m_v2xyLength.y);
        int srcW = static_cast<int>(m_v2xyLength.x);
        int srcH = static_cast<int>(m_v2xyLength.y);

        BITMAPINFO bmInfo = {};
        bmInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
        bmInfo.bmiHeader.biWidth = srcW;
        bmInfo.bmiHeader.biHeight = -srcH;
        bmInfo.bmiHeader.biPlanes = 1;
        bmInfo.bmiHeader.biBitCount = 32;
        bmInfo.bmiHeader.biCompression = BI_RGB;

        void* pBits = nullptr;
        HDC hMemDC = CreateCompatibleDC(NULL);
        HBITMAP hDib = CreateDIBSection(hMemDC, &bmInfo, DIB_RGB_COLORS, &pBits, NULL, 0);

        HBITMAP hOldBmp = (HBITMAP)SelectObject(hMemDC, hDib);

        BitBlt(
            hMemDC,
            0, 0,
            srcW, srcH,
            m_hMemDC,
            srcX, srcY,
            SRCCOPY
        );

        DWORD* pixelData = static_cast<DWORD*>(pBits);
        int    pixelCount = srcW * srcH;
        BYTE   alphaValue = BYTE(m_pAnimator->m_pRenderer->getAlpha() * 255);

        for (int i = 0; i < pixelCount; ++i)
        {
            DWORD c = pixelData[i];

            BYTE b = (BYTE)((c >> 0) & 0xFF);
            BYTE g = (BYTE)((c >> 8) & 0xFF);
            BYTE r = (BYTE)((c >> 16) & 0xFF);

            if (r == _tpColor.r && g == _tpColor.g && b == _tpColor.b)
            {
                pixelData[i] = 0x00000000;
            }
            else
            {
                pixelData[i] = (alphaValue << 24) | (r << 16) | (g << 8) | (b);
            }
        }

        BLENDFUNCTION blendFunc = {};
        blendFunc.BlendOp = AC_SRC_OVER;
        blendFunc.BlendFlags = 0;
        blendFunc.SourceConstantAlpha = 255;
        blendFunc.AlphaFormat = AC_SRC_ALPHA;

        AlphaBlend
        (
            _hDC,
            destX, destY,
            destW, destH,
            hMemDC,
            0, 0,
            srcW, srcH,
            blendFunc
        );

        SelectObject(hMemDC, hOldBmp);
        DeleteObject(hDib);
        DeleteDC(hMemDC);
    }
}
