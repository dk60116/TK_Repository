#include "epch.h"
#include "NaviMesh.h"

using namespace EngineAI;

CNaviMesh::CNaviMesh()
{
}

CNaviMesh::~CNaviMesh()
{
}

CNaviMesh* CNaviMesh::Create()
{
	return new CNaviMesh();
}