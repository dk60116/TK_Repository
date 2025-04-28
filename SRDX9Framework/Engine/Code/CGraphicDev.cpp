#include "CGraphicDev.h"
#include "CEngineEditor.h"
#include "CManagement.h"

CGraphicDev::CGraphicDev() 
	: m_pSDK(NULL)
	, m_pGraphicDev(NULL)
{
	m_strName = L"GraphicDevice";
}

CGraphicDev::~CGraphicDev()
{
	Free();
}

LPDIRECT3DDEVICE9 CGraphicDev::Get_GraphicDev()
{
	return m_pGraphicDev;
}

HRESULT CGraphicDev::Ready_GraphicDev(HWND _hWnd, WINMODE _eMode,
	const _uint& _iSizeX, const _uint& iSizeY, CGraphicDev** _ppGraphicDev)
{
	// 장치를 조사할 객체를 생성
	m_pSDK = Direct3DCreate9(D3D_SDK_VERSION);
	
	if (nullptr == m_pSDK)
		return E_FAIL;

	// 하드웨어 장치(비디오 카드) 성능을 조사

	D3DCAPS9	DeviceCaps;
	ZeroMemory(&DeviceCaps, sizeof(D3DCAPS9));

	// D3DADAPTER_DEFAULT  : 현재 컴퓨터에 설치된 기본 그래픽 카드
	if (FAILED(m_pSDK->GetDeviceCaps(D3DADAPTER_DEFAULT,
									D3DDEVTYPE_HAL,
									&DeviceCaps)))
	{
		return E_FAIL;
	}

	// 그리기 객체를 생성

	_ulong	dwFlag(0);

	if (DeviceCaps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
		dwFlag |= D3DCREATE_HARDWARE_VERTEXPROCESSING;

	else
		dwFlag |= D3DCREATE_SOFTWARE_VERTEXPROCESSING;

	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp, sizeof(D3DPRESENT_PARAMETERS));

	d3dpp.BackBufferWidth  = _iSizeX;
	d3dpp.BackBufferHeight = iSizeY;
	d3dpp.BackBufferFormat = D3DFMT_A8R8G8B8;
	d3dpp.BackBufferCount  = 1;

	d3dpp.MultiSampleType = D3DMULTISAMPLE_NONE;
	d3dpp.MultiSampleQuality = 0;

	// D3DSWAPEFFECT_DISCARD = 스왑 체인 방식
	// D3DSWAPEFFECT_FLIP =  하나의 버퍼를 번갈아 사용
	// D3DSWAPEFFECT_COPY = 더블 버퍼링과 유사한 방식

	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;

	d3dpp.hDeviceWindow = _hWnd;

	d3dpp.Windowed = (_eMode == MODE_WIN);
	
	d3dpp.EnableAutoDepthStencil = TRUE;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;

	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

	if (FAILED(m_pSDK->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL,
		_hWnd, dwFlag, &d3dpp, &m_pGraphicDev)))
		return E_FAIL;

	*_ppGraphicDev = this;

	return S_OK;
}

// 후면 버퍼
void CGraphicDev::Render_Begin(D3DVIEWPORT9 _viewPort, D3DXCOLOR _color)
{
	m_pGraphicDev->SetViewport(&_viewPort);

	// 화면 Clear
	m_pGraphicDev->Clear(1, nullptr,
		D3DCLEAR_TARGET | D3DCLEAR_STENCIL | D3DCLEAR_ZBUFFER,
		_color, 1.f, 0);

	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	m_pGraphicDev->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);

	m_pGraphicDev->BeginScene();
}

void CGraphicDev::Render_End(HWND _window)
{
	m_pGraphicDev->EndScene();
	m_pGraphicDev->Present(NULL, NULL, _window, NULL);
}

HRESULT CGraphicDev::ReSize(_uint _newWidth, _uint _newHeight)
{
	if (!m_pGraphicDev)
		return E_FAIL;

	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp, sizeof(D3DPRESENT_PARAMETERS));

	d3dpp.BackBufferWidth = _newWidth;
	d3dpp.BackBufferHeight = _newHeight;
	d3dpp.BackBufferFormat = D3DFMT_A8R8G8B8;
	d3dpp.BackBufferCount = 1;
	d3dpp.MultiSampleType = D3DMULTISAMPLE_NONE;
	d3dpp.MultiSampleQuality = 0;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.hDeviceWindow = CEngineEditor::GetInstance().FindWindowHandle(L"Game");
	d3dpp.Windowed = TRUE;
	d3dpp.EnableAutoDepthStencil = TRUE;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

	// 디바이스 리셋
	if (FAILED(m_pGraphicDev->Reset(&d3dpp)))
	{
		MessageBox(NULL, L"Device Reset Failed", L"Error", MB_OK);
		return E_FAIL;
	}

	// 뷰포트 및 프로젝션 재설정
	D3DVIEWPORT9 vp = {};
	vp.X = 0;
	vp.Y = 0;
	vp.Width = _newWidth;
	vp.Height = _newHeight;
	vp.MinZ = 0.0f;
	vp.MaxZ = 1.0f;

	m_pGraphicDev->SetViewport(&vp);

	_matrix matProj;
	D3DXMatrixOrthoLH(&matProj, (float)_newWidth, (float)_newHeight, 0.0f, 1.0f);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &matProj);

	CManagement::GetInstance().getCrtScene()->UpdateAllLight();

	return S_OK;
}

void CGraphicDev::Free()
{
	Safe_Release(m_pSDK);
	Safe_Release(m_pGraphicDev);
}

void IDeviceResetListener::RegisterResetListener(IDeviceResetListener* pListener)
{
	m_vecResetListeners.push_back(pListener);
}
