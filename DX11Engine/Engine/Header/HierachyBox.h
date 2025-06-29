#pragma once

#include "EditorBox.h"

NS_BEGIN(Engine)

class ENGINE_DLL CHierachyBox final : public CEditorBox
{
	friend class CEditor;

protected:
	explicit CHierachyBox();
	~CHierachyBox();

public:
	void Render() override;

private:
	static CHierachyBox* Create(EDITORBOXDESC _option);
};

NS_END

