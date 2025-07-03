#include "epch.h"
#include "InspectorBox.h"

CInspectorBox::CInspectorBox()
{
}

CInspectorBox::~CInspectorBox()
{
	OnDestroy();
}

CInspectorBox* CInspectorBox::Create()
{
	CInspectorBox* newBox = new CInspectorBox();

	if (FAILED(newBox->Initialize()))
	{
		delete(newBox);
		newBox = nullptr;
		return nullptr;
	}

	newBox->m_strBoxName = L"Inspector";

	return newBox;
}

void CInspectorBox::Render()
{
	CEditor& editor = CEditor::GetInstance();
	const CEditor::EDITORWINOPTION& editorOption = editor.Get_Options();
	CScene* currentScene = CSceneManager::GetInstance().Get_CrtScene();

	_float width = static_cast<_float>(editorOption.inspectorWidth);

	ImGuiViewport* viewport = ImGui::GetMainViewport();

	ImVec2 panelSize = ImVec2(width, viewport->Size.y);

	ImGui::SetNextWindowPos
	(
		ImVec2(viewport->Pos.x + viewport->Size.x, viewport->Pos.y),
		0,
		ImVec2(1.0f, 0.0f)
	);

	ImGui::SetNextWindowSize(panelSize);

	ImGui::Begin(CEngineString::WStringToString(m_strBoxName).c_str(),
		nullptr,
		ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoCollapse);

    CGameObject* selectedObj = editor.Get_SelectedGameObject();

    if (selectedObj)
    {
        ImGui::Text("%s", CEngineString::WStringToString(selectedObj->Get_ObjectName()).c_str());

        ShowTransform(selectedObj);
    }
    else
    {
        ImGui::Text("No object selected.");
    }

	ImGui::End();
}

void CInspectorBox::OnDestroy()
{
}

void CInspectorBox::ShowTransform(CGameObject* _obj)
{
    CTransform* transform = _obj->Get_Transform();

    if (transform)
    {
        ImGui::Text("Transform");

        const _float LabelWidth = 60.f;
        const _float boxWidth = 34.f;

        // Position
        _float3 position = transform->Get_LocalPosition();
        if (ImGui::BeginTable("Position Table", 2, ImGuiTableFlags_BordersInnerV))
        {
            ImGui::TableSetupColumn("Label", ImGuiTableColumnFlags_WidthFixed, LabelWidth);
            ImGui::TableSetupColumn("Value");

            ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0);
            ImGui::Text("Position");
            ImGui::TableSetColumnIndex(1);
            
            // X
            ImGui::TextUnformatted("X"); ImGui::SameLine();
            ImGui::PushItemWidth(boxWidth);
            if (ImGui::DragFloat("##X", &position.x, 0.1f))
                transform->Set_LocalPosition(position);
            ImGui::PopItemWidth();

            ImGui::SameLine();

            // Y
            ImGui::TextUnformatted("Y"); ImGui::SameLine();
            ImGui::PushItemWidth(boxWidth);
            if (ImGui::DragFloat("##Y", &position.y, 0.1f))
                transform->Set_LocalPosition(position);
            ImGui::PopItemWidth();

            ImGui::SameLine();

            // Z
            ImGui::TextUnformatted("Z"); ImGui::SameLine();
            ImGui::PushItemWidth(boxWidth);
            if (ImGui::DragFloat("##Z", &position.z, 0.1f))
                transform->Set_LocalPosition(position);
            ImGui::PopItemWidth();

            ImGui::EndTable();
        }

        // Rotation
        _float3 rotation = transform->Get_LocalEulerAngles();
        if (ImGui::BeginTable("Rotation Table", 2, ImGuiTableFlags_BordersInnerV))
        {
            ImGui::TableSetupColumn("Label", ImGuiTableColumnFlags_WidthFixed, LabelWidth);
            ImGui::TableSetupColumn("Value");

            ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0);
            ImGui::Text("Rotation");
            ImGui::TableSetColumnIndex(1);

            // X
            ImGui::TextUnformatted("X"); ImGui::SameLine();
            ImGui::PushItemWidth(boxWidth);
            if (ImGui::DragFloat("##X", &rotation.x, 0.1f))
                transform->Set_LocalEulerAngles(rotation);
            ImGui::PopItemWidth();

            ImGui::SameLine();

            // Y
            ImGui::TextUnformatted("Y"); ImGui::SameLine();
            ImGui::PushItemWidth(boxWidth);
            if (ImGui::DragFloat("##Y", &rotation.y, 0.1f))
                transform->Set_LocalEulerAngles(rotation);
            ImGui::PopItemWidth();

            ImGui::SameLine();

            // Z
            ImGui::TextUnformatted("Z"); ImGui::SameLine();
            ImGui::PushItemWidth(boxWidth);
            if (ImGui::DragFloat("##Z", &rotation.z, 0.1f))
                transform->Set_LocalEulerAngles(rotation);
            ImGui::PopItemWidth();

            ImGui::EndTable();
        }

        // Scale
        _float3 scale = transform->Get_LocalScale();
        if (ImGui::BeginTable("Rotation Table", 2, ImGuiTableFlags_BordersInnerV))
        {
            ImGui::TableSetupColumn("Label", ImGuiTableColumnFlags_WidthFixed, LabelWidth);
            ImGui::TableSetupColumn("Value");

            ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0);
            ImGui::Text("Scale");
            ImGui::TableSetColumnIndex(1);

            // X
            ImGui::TextUnformatted("X"); ImGui::SameLine();
            ImGui::PushItemWidth(boxWidth);
            if (ImGui::DragFloat("##X", &scale.x, 0.1f))
                transform->Set_LocalScale(scale);
            ImGui::PopItemWidth();

            ImGui::SameLine();

            // Y
            ImGui::TextUnformatted("Y"); ImGui::SameLine();
            ImGui::PushItemWidth(boxWidth);
            if (ImGui::DragFloat("##Y", &scale.y, 0.1f))
                transform->Set_LocalScale(scale);
            ImGui::PopItemWidth();

            ImGui::SameLine();

            // Z
            ImGui::TextUnformatted("Z"); ImGui::SameLine();
            ImGui::PushItemWidth(boxWidth);
            if (ImGui::DragFloat("##Z", &scale.z, 0.1f))
                transform->Set_LocalScale(scale);
            ImGui::PopItemWidth();

            ImGui::EndTable();
        }
    }
}
