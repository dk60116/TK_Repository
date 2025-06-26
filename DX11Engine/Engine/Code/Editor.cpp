#include "epch.h"
#include "Editor.h"

CEditor::CEditor()
	: m_hEditorWindow(nullptr)
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

	return S_OK;
}

void CEditor::Release()
{
#ifndef _DEBUG
	return;
#endif // DEBUG

	ImGui_ImplWin32_Shutdown();
	ImGui_ImplDX11_Shutdown();
	ImGui::DestroyContext();
	DestroyWindow(m_hEditorWindow);
}

HWND CEditor::Get_EditorWindow()
{
	return m_hEditorWindow;
}

HWND CEditor::CreateEditorWindow()
{
	WNDCLASS wc = {};
	wc.lpfnWndProc = DefWindowProc; // ¶Ç´Â ImGui¿ë WndProc
	wc.hInstance = CDisplay::GetInstance().Get_HInstance();
	wc.lpszClassName = "Editor";

	RegisterClass(&wc);

	HWND hwnd = CreateWindowEx
	(
		0,
		wc.lpszClassName,
		"Editor",
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT,
		CDisplay::GetInstance().Get_ScreenResolution().x, CDisplay::GetInstance().Get_ScreenResolution().y,
		nullptr,
		nullptr,
		CDisplay::GetInstance().Get_HInstance(),
		nullptr
	);

	ShowWindow(hwnd, SW_SHOW);
	UpdateWindow(hwnd);


	return hwnd;
}
