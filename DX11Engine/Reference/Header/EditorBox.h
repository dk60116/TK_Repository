#pragma once

#include "Object.h"

NS_BEGIN(Engine)

class ENGINE_DLL CEditorBox abstract : public UObject
{
public:
	typedef struct EditorBoxOptionsDescription
	{
		vector2Int position;
		vector2Int size;
	}EDITORBOXDESC;

protected:
	explicit CEditorBox();
	~CEditorBox();

public:
	virtual HRESULT Initialize(EDITORBOXDESC _option);
	virtual void Render();
};

NS_END

