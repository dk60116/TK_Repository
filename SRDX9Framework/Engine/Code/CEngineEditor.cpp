#include "CEngineEditor.h"

CEngineEditor::CEngineEditor()
	: m_hInst(nullptr)
	, m_hMainWnd(nullptr)
{
}

CEngineEditor::~CEngineEditor()
{
}

void CEngineEditor::Init(HINSTANCE _hInst, HWND _mainWnd)
{
	m_hInst = _hInst;
	m_hMainWnd = _mainWnd;
}
