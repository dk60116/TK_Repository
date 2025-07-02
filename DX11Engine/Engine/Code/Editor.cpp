#include "epch.h"
#include "HierachyBox.h"
#include "InspectorBox.h"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

CEditor::CEditor()
	: m_hEditorWindow(nullptr)
	, m_pHierachyBox(nullptr)
	, m_pInspectorBox(nullptr)
	, m_mBoxList({})
	, m_sOptions({})
	, m_pSelectedGameObject(nullptr)
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

	m_pHierachyBox = CHierachyBox::Create();
	if (m_pHierachyBox)
	{
		m_pHierachyBox->AddRef();
		m_mBoxList.emplace(L"Hierachy", m_pHierachyBox);
	}

	m_pInspectorBox = CInspectorBox::Create();
	if (m_pInspectorBox)
	{
		m_pInspectorBox->AddRef();
		m_mBoxList.emplace(L"Inspector", m_pInspectorBox);
	}

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

	Safe_Release(m_pSelectedGameObject);

	for (TRAVERSAL_ITER(m_mBoxList, it))
		Safe_Release((*it).second);

	m_mBoxList.clear();
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

	for (TRAVERSAL_ITER(m_mBoxList, it))
		(*it).second->Render();

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

	RECT rc = { 0, 0, static_cast<LONG>(m_sOptions.windowWidth), static_cast<LONG>(m_sOptions.windowHeight) };
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

CEditor::EDITORWINOPTION CEditor::Get_Options() const
{
	return m_sOptions;
}

const vector2Int CEditor::Get_ScreenResolution() const
{
	return vector2Int(m_sOptions.windowWidth, m_sOptions.windowHeight);
}

void CEditor::Set_SelectedGameObject(CGameObject* _target)
{
	Safe_Release(m_pSelectedGameObject);

	m_pSelectedGameObject = _target;

	if (m_pSelectedGameObject)
	{
		m_pSelectedGameObject = _target;
		m_pSelectedGameObject->AddRef();
	}
}

CGameObject* CEditor::Get_SelectedGameObject() const
{
	return m_pSelectedGameObject;
}
