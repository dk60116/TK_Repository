#include "epch.h"
#include "HierachyBox.h"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

CEditor::CEditor()
	: m_hEditorWindow(nullptr)
	, m_pHierachyBox(nullptr)
	, m_sOptions({})
{
}

CEditor::~CEditor()
{
	Release();
}

CEditor& CEditor::GetInstance()
{
	static CEditor inst;
	return inst;
}

HRESULT CEditor::Initialize()
{
#ifndef _DEBUG
	return S_OK;
#endif // DEBUG

	ImGuiContext* newCtx = ImGui::CreateContext();
	ImGui::SetCurrentContext(newCtx);

	m_hEditorWindow = CreateEditorWindow();

	ImGui_ImplWin32_Init(m_hEditorWindow);
	ImGui_ImplDX11_Init(CGraphicDevice::GetInstance().Get_Device(), CGraphicDevice::GetInstance().Get_Context());

	ImGuiIO& io = ImGui::GetIO();
	io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\malgun.ttf", 18.0f, NULL, io.Fonts->GetGlyphRangesKorean());

	CEditorBox::EDITORBOXDESC hierachyDesc = {};

	m_pHierachyBox = CHierachyBox::Create(hierachyDesc);
	m_pHierachyBox->AddRef();

	return S_OK;
}

void CEditor::Release()
{
#ifndef _DEBUG
	return;
#endif // DEBUG

	Safe_Release(m_pHierachyBox);

	ImGui_ImplWin32_Shutdown();
	ImGui_ImplDX11_Shutdown();
	ImGui::DestroyContext();
	DestroyWindow(m_hEditorWindow);
}

HWND CEditor::Get_EditorWindow()
{
	return m_hEditorWindow;
}

void CEditor::Editor_Update()
{
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	m_pHierachyBox->Render();

	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

HWND CEditor::CreateEditorWindow()
{
	WNDCLASS wc = {};
	wc.lpfnWndProc = EditorWndProc;
	wc.hInstance = CDisplay::GetInstance().Get_HInstance();
	wc.lpszClassName = "Editor";

	RegisterClass(&wc);

	RECT rc = { 0, 0, m_sOptions.windowWidth, m_sOptions.windowHeight };
	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);

	HWND hwnd = CreateWindowEx
	(
		0,
		wc.lpszClassName,
		"Editor",
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT,
		rc.right - rc.left, rc.bottom - rc.top,
		nullptr,
		nullptr,
		CDisplay::GetInstance().Get_HInstance(),
		nullptr
	);

	ShowWindow(hwnd, SW_SHOW);
	UpdateWindow(hwnd);

	return hwnd;
}

LRESULT CEditor::EditorWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (ImGui_ImplWin32_WndProcHandler(hwnd, msg, wParam, lParam))
		return true;

	switch (msg)
	{
	case WM_MOUSEWHEEL:
	{
#ifndef _DEBUG
#else
		short delta = GET_WHEEL_DELTA_WPARAM(wParam);
		_float normalized = static_cast<float>(delta) / WHEEL_DELTA;

		CInput::GetInstance().Get_WheelAxisRaw() += normalized;
#endif
	}
	return 0;
	}

	return DefWindowProc(hwnd, msg, wParam, lParam);
}

const CEditor::EDITORWINOPTION& CEditor::Get_Options()
{
	return m_sOptions;
}

const vector2Int CEditor::Get_ScreenResolution() const
{
	return vector2Int(m_sOptions.windowWidth, m_sOptions.windowHeight);
}