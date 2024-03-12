#include "pch.h"
#include "global.h"


// extern이 붙은 변수의 실제 구현부분을 모아놓을 cpp 파일
tTransform g_Transform = {};
tGlobalData g_global = {};

const char* ASSET_TYPE_STRING[(UINT)ASSET_TYPE::END]
{
	"MESH",
	"MESHDATA",
	"PREFAB",
	"TEXTURE",
	"MATERIAL",
	"SOUND",
	"COMPUTE_SHADER",
	"GRAPHICS_SHADER",
	"FSM",
};