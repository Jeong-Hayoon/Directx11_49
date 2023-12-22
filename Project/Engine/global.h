// 배포용 헤더
#pragma once

#include <stdlib.h>
#include <Windows.h>

// template에서 타입을 구분하는 기능을 사용하기 위해 참조
#include <typeinfo>

#include <string>

#include <vector>
#include <list>
#include <map>

using std::vector;
using std::list;
using std::map;
using std::make_pair;

using std::wstring;
using std::string;

// GPU 제어를 위한 라이브러리
#include <wrl.h>
using namespace Microsoft::WRL;


// Directx 11 헤더 및 라이브러리 참조
// 구조체나 Buffer 사이즈 계산 시 제일 큰 자료형의 사이즈를
// 기준으로 했었지만 #pragma pack이 d3d11.h를 참조하면 포함되어 있기
// 때문에 그냥 실제 사이즈로 계산을 하면 됨
#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>

using namespace DirectX;

#pragma comment(lib, "d3d11")
#pragma comment(lib, "d3dcompiler")

// DirectxTex : 텍스처 로딩 관련 라이브러리
#include <DirectXTex/DirectXTex.h>

#ifdef _DEBUG
#pragma comment(lib, "DirectXTex\\DirectXTex_debug")
#else
#pragma comment(lib, "DirectXTex\\DirectXTex")
#endif

// SimpleMath
#include "SimpleMath.h"
using namespace DirectX::SimpleMath;

// Vector2가 너무 길어서 Vec2로 줄여놓음
typedef Vector2 Vec2;
typedef Vector3 Vec3;
typedef Vector4 Vec4;

// 엔진 헤더 참조
#include "singleton.h"
#include "define.h"
#include "struct.h"
#include "func.h"