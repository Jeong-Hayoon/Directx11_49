// ������ ���
#pragma once

#include <stdlib.h>
#include <Windows.h>

// template���� Ÿ���� �����ϴ� ����� ����ϱ� ���� ����
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

// GPU ��� ���� ���̺귯��
#include <wrl.h>
using namespace Microsoft::WRL;


// Directx 11 ��� �� ���̺귯�� ����
// ����ü�� Buffer ������ ��� �� ���� ū �ڷ����� �����
// �������� �߾����� #pragma pack�� d3d11.h�� �����ϸ� ���ԵǾ� �ֱ�
// ������ �׳� ���� ������� ����� �ϸ� ��
#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>

using namespace DirectX;

#pragma comment(lib, "d3d11")
#pragma comment(lib, "d3dcompiler")

// DirectxTex : �ؽ�ó �ε� ���� ���̺귯��
#include <DirectXTex/DirectXTex.h>

#ifdef _DEBUG
#pragma comment(lib, "DirectXTex\\DirectXTex_debug")
#else
#pragma comment(lib, "DirectXTex\\DirectXTex")
#endif

// SimpleMath
#include "SimpleMath.h"
using namespace DirectX::SimpleMath;

// Vector2�� �ʹ� �� Vec2�� �ٿ�����
typedef Vector2 Vec2;
typedef Vector3 Vec3;
typedef Vector4 Vec4;

// ���� ��� ����
#include "singleton.h"
#include "define.h"
#include "struct.h"
#include "func.h"