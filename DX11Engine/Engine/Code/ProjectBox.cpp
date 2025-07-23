#include "epch.h"
#include "ProjectBox.h"

CProjectBox::CProjectBox()
	: m_strCurrentSelectedFilePath("")
{
}

CProjectBox::~CProjectBox()
{
	OnDestroy();
}

CProjectBox* CProjectBox::Create()
{
	CProjectBox* newBox = new CProjectBox();

	if (FAILED(newBox->Initialize()))
	{
		delete(newBox);
		newBox = nullptr;
		return nullptr;
	}

	newBox->m_strBoxName = L"Project";

	return newBox;
}

void CProjectBox::Render()
{
	CEditor& editor = CEditor::GetInstance();
	const CEditor::EDITORWINOPTION& editorOption = editor.Get_Options();
	CScene* currentScene = CSceneManager::GetInstance().Get_CrtScene();

	_float width = static_cast<_float>(editorOption.projectWidth);

	ImGuiViewport* viewport = ImGui::GetMainViewport();

	ImVec2 panelSize = ImVec2(width, viewport->Size.y - editorOption.topBarHeight);

	ImGui::SetNextWindowPos
	(
		ImVec2(viewport->Pos.x + viewport->Size.x - editorOption.hierachyWidth - editorOption.inspectorWidth, viewport->Pos.y + editorOption.topBarHeight),
		0,
		ImVec2(1.0f, 0.0f)
	);

	ImGui::SetNextWindowSize(panelSize);

	ImGui::Begin
	(
		CEngineString::WStringToString(m_strBoxName).c_str(),
		nullptr,
		ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoCollapse
	);

	ImGui::PushStyleVar(ImGuiStyleVar_IndentSpacing, 12.f);

	RenderAssetFoldersHierarchy();
	RenderBinaryFoldersHierarchy();

	ImGui::PopStyleVar();

	if (CInput::GetInstance().GetKeyDown_Editor(C))
	{
		CResources::GetInstance().ReadSkinnedBufferInfos(L"Player_Link.skinneddata");
	}

	ImGui::End();
}

void CProjectBox::OnDestroy()
{
}

void CProjectBox::RenderAssetFoldersHierarchy()
{
	static const fs::path rootPath = L"../Assets";

	RenderDirectoryRecursive(rootPath);
}

void CProjectBox::RenderBinaryFoldersHierarchy()
{
	static const fs::path rootPath = L"BinaryAssets";

	RenderDirectoryRecursive(rootPath);
}

void CProjectBox::RenderDirectoryRecursive(const fs::path& _dirPath)
{
	// 폴더 이름만 추출
	string folderName = _dirPath.filename().string();

	// 트리 노드 시작
	if (ImGui::TreeNode(folderName.c_str()))
	{
		for (const auto& entry : fs::directory_iterator(_dirPath))
		{
			if (entry.is_directory())
			{
				// 재귀적으로 하위 폴더 탐색
				RenderDirectoryRecursive(entry.path());
			}
			else if (entry.is_regular_file())
			{
				string filename = entry.path().filename().string();
				string buttonId = filename + "##" + entry.path().string();
				if (ImGui::Button(buttonId.c_str()))
				{
					m_strCurrentSelectedFilePath = entry.path();
				}

				if (ImGui::BeginPopupContextItem(buttonId.c_str())) // ID로 연결됨
				{
					auto splitName = CEngineString::Split(filename, ".");

					const string path = entry.path().string();
					const string fileName = splitName[0];
					const string extension = splitName[1];

					if (ImGui::Selectable("Log info"))
					{
						uintmax_t sizeInBytes = fs::file_size(entry.path());
						double sizeKB = sizeInBytes / 1024.0;

						CDebug::Log("Name: " + fileName);
						CDebug::Log("Type: " + extension);
						CDebug::Log("Path: " + path);
						CDebug::Log("Size: " + to_string(sizeKB) + "kb");
					}

					if (extension == "fbx")
					{
						if (ImGui::Selectable("Create Mesh Data"))
						{
							wstring path = CEngineString::Erase(entry.path().wstring(), L"../Assets\\");
							path = CEngineString::Replace(path, L"\\", L"/");
							CResources::GetInstance().ConvertFBXToMeshBufferData(path);
						}

						if (ImGui::Selectable("Create Skeleton Data"))
						{
							wstring path = CEngineString::Erase(entry.path().wstring(), L"../Assets\\");
							path = CEngineString::Replace(path, L"\\", L"/");
							CResources::GetInstance().ConvertFBXToSkinnedBufferData(path);
						}
					}

					if (ImGui::Selectable("Delete"))
					{
						filesystem::remove(path);
					}

					ImGui::EndPopup();
				}
			}
		}

		ImGui::TreePop();
	}
}
