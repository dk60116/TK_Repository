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

        CTransform* transform = selectedObj->Get_Transform();

        if (transform)
        {
            ImGui::Text("Transform");

            // Position
            _float3 position = transform->Get_Position();
            if (ImGui::DragFloat3("Position", &position.x, 0.1f))
            {
                transform->Set_LocalPosition(position);
            }

            // Rotation
            _float3 rotation = transform->Get_EulerAngles();
            if (ImGui::DragFloat3("Rotation", &rotation.x, 0.5f))
            {
                transform->Set_LocalEulerAngles(rotation);
            }

            // Scale
            XMFLOAT3 scale = transform->Get_LocalScale();
            if (ImGui::DragFloat3("Scale", &scale.x, 0.1f))
            {
                transform->Set_LocalScale(scale);
            }
        }
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
