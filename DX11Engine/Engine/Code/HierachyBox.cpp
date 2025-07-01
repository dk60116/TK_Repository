#include "epch.h"
#include "HierachyBox.h"

CHierachyBox::CHierachyBox()
{
}

CHierachyBox::~CHierachyBox()
{
}

CHierachyBox* CHierachyBox::Create(EDITORBOXDESC _option)
{
	CHierachyBox* newBox = new CHierachyBox();

	if (FAILED(newBox->Initialize(_option)))
	{
		delete(newBox);
		newBox = nullptr;
		return nullptr;
	}

	return newBox;
}

void CHierachyBox::Render()
{
	_float width = (float)CEditor::GetInstance().Get_Options().hierachyWidth;

	ImGuiViewport* viewport = ImGui::GetMainViewport();

	ImVec2 panelSize = ImVec2(width, viewport->Size.y);

	ImGui::SetNextWindowPos
	(
		ImVec2(viewport->Pos.x + viewport->Size.x, viewport->Pos.y),
		0,
		ImVec2(1.0f, 0.0f) 
	);

	ImGui::SetNextWindowSize(panelSize);

	ImGui::Begin("Hierachy",
		nullptr,
		ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoCollapse);

	// Hierarchy Root 가져오기
	CScene* currentScene = CSceneManager::GetInstance().Get_CrtScene();

	if (currentScene)
	{
		for (auto& obj : currentScene->Get_RootObjects())
		{
			RenderObjectHierarchy(obj);
		}
	}

	ImGui::End();
}

void CHierachyBox::RenderObjectHierarchy(CGameObject* _obj)
{
	if (!_obj)
		return;

	string name = CEngineString::WStringToString(_obj->Get_ObjectName());

	_bool hasChildren = !_obj->Get_Transform()->Get_ChldList().empty();

	ImGuiTreeNodeFlags flags = 0;

	if (!hasChildren)
		flags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;

	_bool nodeOpen = ImGui::TreeNodeEx(name.c_str(), flags);

	if (hasChildren && nodeOpen)
	{
		for (auto* child : _obj->Get_Transform()->Get_ChldList())
			RenderObjectHierarchy(child->Get_GameObject());

		ImGui::TreePop();
	}
}
