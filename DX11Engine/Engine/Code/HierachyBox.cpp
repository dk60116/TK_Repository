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

	// Pos + Pivot 을 같이 지정
	ImGui::SetNextWindowPos
	(
		ImVec2(viewport->Pos.x + viewport->Size.x, viewport->Pos.y),
		0,
		ImVec2(1.0f, 0.0f)  // Pivot: 오른쪽 상단 기준
	);

	ImGui::SetNextWindowSize(panelSize);

	ImGui::Begin("Hierachy",
		nullptr,
		ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoCollapse);

	ImGui::End();
}
