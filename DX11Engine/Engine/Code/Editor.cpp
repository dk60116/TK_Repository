#include "epch.h"
#include "TopToolBar.h"
#include "ProjectBox.h"
#include "HierachyBox.h"
#include "InspectorBox.h"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

CEditor::CEditor()
	: m_hEditorWindow(nullptr)
	, m_bPlaying(false)
	, m_bPrevPlaying(false)
	, m_mBoxList({})
	, m_sOptions({})
	, m_eControleTool(TransformControleTool::MOVE)
	, m_pSelectedGameObject(nullptr)
	, m_pMoveTargetGameObject(nullptr)
	, m_vCameraPos({})
	, m_vCameraQuat({})
	, m_bDoubleClicked(false)
	, m_bIsMovingCamera(false)
	, m_vCameraMoveStartPos({})
	, m_vCameraMoveTargetPos({})
	, m_fCameraMoveDuration(0.3f)
	, m_fCameraMoveProgress(0.f)
	, m_hEditorWindowIcon_Default(nullptr)
	, m_hEditorWindoIcon_Small(nullptr)
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
#ifdef _CLIENT_BUILD
	return S_OK;
#endif

	ImGuiContext* newCtx = ImGui::CreateContext();
	ImGui::SetCurrentContext(newCtx);

	GetInstance().m_hEditorWindow = CreateEditorWindow();

	ImGui_ImplWin32_Init(GetInstance().m_hEditorWindow);
	ImGui_ImplDX11_Init(CGraphicDevice::GetInstance().Get_Device(), CGraphicDevice::GetInstance().Get_Context());

	ImGuiIO& io = ImGui::GetIO();
	io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\malgun.ttf", 18.0f, NULL, io.Fonts->GetGlyphRangesKorean());

	CTopToolBar* toolbar = CTopToolBar::Create();
	if (toolbar)
	{
		GetInstance().m_mBoxList.emplace(L"TopTool", toolbar);
		toolbar->AddRef();
	}

	CProjectBox* projectBox = CProjectBox::Create();
	if (projectBox)
	{
		GetInstance().m_mBoxList.emplace(L"Project", projectBox);
		projectBox->AddRef();
	}

	CHierarchyBox* hierachyBox = CHierarchyBox::Create();
	if (hierachyBox)
	{
		GetInstance().m_mBoxList.emplace(L"Hierarchy", hierachyBox);
		hierachyBox->AddRef();
	}

	CInspectorBox* inspectorBox = CInspectorBox::Create();
	if (inspectorBox)
	{
		GetInstance().m_mBoxList.emplace(L"Inspector", inspectorBox);
		inspectorBox->AddRef();
	}

	return S_OK;
}

void CEditor::Release()
{
#ifdef _CLIENT_BUILD
	return;
#endif

	ImGui_ImplWin32_Shutdown();
	ImGui_ImplDX11_Shutdown();
	ImGui::DestroyContext();

	for (TRAVERSAL_ITER(GetInstance().m_mBoxList, it))
		Safe_Release((*it).second);

	GetInstance().m_mBoxList.clear();
}

void CEditor::Play()
{
	GetInstance().m_bPlaying = true;
}

void CEditor::Stop()
{
	GetInstance().m_bPlaying = false;
}

void CEditor::Pause()
{
	GetInstance().m_bPlaying = false;
}

const _bool CEditor::IsPlaying()
{
	return GetInstance().m_bPlaying;
}

HWND CEditor::Get_EditorWindow()
{
	return GetInstance().m_hEditorWindow;
}

void CEditor::Editor_Update_Begin()
{
	// 1. 프레임 시작
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	// 2. Hierarchy/Inspector 박스 렌더
	for (TRAVERSAL_ITER(GetInstance().m_mBoxList, it))
		(*it).second->Render();
}

void CEditor::Editor_Update_During()
{
	ChangeControleTool();

	if (GetInstance().m_bIsMovingCamera)
	{
		const _float dt = DELTA_TIME;
		GetInstance().m_fCameraMoveProgress += dt / GetInstance().m_fCameraMoveDuration;

		if (GetInstance().m_fCameraMoveProgress >= 1.f)
		{
			GetInstance().m_fCameraMoveProgress = 1.f;
			GetInstance().m_bIsMovingCamera = false;
		}

		float t = GetInstance().m_fCameraMoveProgress;
		t = t * t * (3.f - 2.f * t);

		vector3 interpPos = vector3::Lerp(GetInstance().m_vCameraMoveStartPos, GetInstance().m_vCameraMoveTargetPos, t);

		CTransform* camTransform = CSceneManager::Get_EditorCamera()->Get_Transform();
		camTransform->Set_Position(interpPos);
	}

	if (GetInstance().m_pSelectedGameObject)
	{
		if (CInput::GetKeyDown_Editor(KEY_DELETE))
			CGameObject::Destroy(GetInstance().m_pSelectedGameObject);
	}
}

void CEditor::Editor_Update_End()
{
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

HWND CEditor::CreateEditorWindow()
{
	WNDCLASS wc = {};
	wc.lpfnWndProc = EditorWndProc;
	wc.hInstance = CDisplay::Get_HInstance();
	wc.lpszClassName = "Editor";

	RegisterClass(&wc);

	RECT rc = { 0, 0, static_cast<LONG>(GetInstance().m_sOptions.windowWidth), static_cast<LONG>(GetInstance().m_sOptions.windowHeight) };
	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);

	GetInstance().m_hEditorWindowIcon_Default = (HICON)LoadImageW
	(
		NULL,
		L"../EngineResources/Icon/Engine_Icon.ico",
		IMAGE_ICON,
		32, 32,
		LR_LOADFROMFILE | LR_DEFAULTSIZE
	);

	GetInstance().m_hEditorWindoIcon_Small = (HICON)LoadImageW
	(
		NULL,
		L"../EngineResources/Icon/Engine_Icon.ico",
		IMAGE_ICON,
		16, 16,
		LR_LOADFROMFILE
	);

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
		CDisplay::Get_HInstance(),
		nullptr
	);

	SendMessage(hwnd, WM_SETICON, ICON_BIG, (LPARAM)GetInstance().m_hEditorWindoIcon_Small);

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

		CInput::Get_WheelAxisRaw() += normalized;
#endif
	}
	return 0;
	}

	return DefWindowProc(hwnd, msg, wParam, lParam);
}

void CEditor::ChangeControleTool()
{
	if (!CInput::GetMouseButton_Editor(1))
	{
		if (CInput::GetKeyDown_Editor(Q))
			Change_ControleTool(TransformControleTool::VIEW);
		if (CInput::GetKeyDown_Editor(W))
			Change_ControleTool(TransformControleTool::MOVE);
		if (CInput::GetKeyDown_Editor(E))
			Change_ControleTool(TransformControleTool::ROTATE);
		if (CInput::GetKeyDown_Editor(R))
			Change_ControleTool(TransformControleTool::SCALE);
		if (CInput::GetKeyDown_Editor(T))
			Change_ControleTool(TransformControleTool::RECT);
		if (CInput::GetKeyDown_Editor(Y))
			Change_ControleTool(TransformControleTool::TRANSFORM);
	}
}

CEditor::EDITORWINOPTION CEditor::Get_Options()
{
	return GetInstance().m_sOptions;
}

vector2Int CEditor::Get_WindowResolution()
{
	return vector2Int(GetInstance().m_sOptions.windowWidth, GetInstance().m_sOptions.windowHeight);
}

vector2Int CEditor::Get_ScreenResolution()
{
	_int width = _int(GetInstance().m_sOptions.windowWidth - (GetInstance().m_sOptions.projectWidth + GetInstance().m_sOptions.hierachyWidth + GetInstance().m_sOptions.inspectorWidth));
	_int height = _int(GetInstance().m_sOptions.windowHeight - (GetInstance().m_sOptions.topBarHeight));

	return vector2Int(width, height);
}

CEditor::TransformControleTool CEditor::Get_ControleTool()
{
	return GetInstance().m_eControleTool;
}

void CEditor::Change_ControleTool(const TransformControleTool _tool)
{
	GetInstance().m_eControleTool = _tool;
}

vector3 CEditor::Get_EditorCamPositon()
{
	return GetInstance().m_vCameraPos;
}

quaternion CEditor::Get_EditorCamQuaternion()
{
	return GetInstance().m_vCameraQuat;
}

void CEditor::Set_EditorCamTransform(CTransform* _transform)
{
	GetInstance().m_vCameraPos = _transform->Get_Position();
	GetInstance().m_vCameraQuat = _transform->Get_Quaternion();
}

void CEditor::Set_SelectedGameObject(CGameObject* _target)
{
	if (_target == GetInstance().m_pSelectedGameObject)
		return;

	GetInstance().m_pSelectedGameObject = _target;

	if (GetInstance().m_pSelectedGameObject)
	{
		GetInstance().m_pSelectedGameObject = _target;
	}
}

void CEditor::MoveTo_SelectedGameObject(CGameObject* _target)
{
	if (!_target)
	{
		GetInstance().m_pMoveTargetGameObject = nullptr;
		return;
	}

	if (_target == GetInstance().m_pMoveTargetGameObject)
		GetInstance().m_bDoubleClicked = !GetInstance().m_bDoubleClicked;
	else
		GetInstance().m_bDoubleClicked = false;

	_float distance = GetInstance().m_bDoubleClicked ? 6.f : 3.f;

	GetInstance().m_pMoveTargetGameObject = _target;

	_float3 targetPos = _target->Get_Transform()->Get_Position();
	CTransform& ect = *CSceneManager::Get_EditorCamera()->Get_Transform();

	GetInstance().m_vCameraMoveStartPos = ect.Get_Position();
	GetInstance().m_vCameraMoveTargetPos = targetPos + ect.Get_Directions().forward * -distance;

	GetInstance().m_fCameraMoveProgress = 0.f;
	GetInstance().m_bIsMovingCamera = true;
}

CGameObject* CEditor::Get_SelectedGameObject()
{
	return GetInstance().m_pSelectedGameObject;
}
