#pragma once

#include "EditorBox.h"

NS_BEGIN(Engine)

class ENGINE_DLL CHierarchyBox final : public CEditorBox
{
	friend class CEditor;

protected:
	explicit CHierarchyBox();
	~CHierarchyBox();

public:
	void Render() override;
	void OnDestroy() override;

private:
	static CHierarchyBox* Create();

private:
	void RenderObjectHierarchy(CGameObject* _obj);
};

NS_END

