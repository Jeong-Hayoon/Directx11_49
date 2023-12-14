#pragma once

// friend로 안해놓으면 자식 클래스들은 생성자와 소멸자를 숨겨놨기 때문에 객체 생성이 안됨
#define SINGLE(classtype)  	private:\
								classtype();\
								~classtype();\
							friend class HYSingleton<classtype>;

// 편의를 위해 define 
#define DEVICE HYDevice::GetInst()->GetDevice()
#define CONTEXT HYDevice::GetInst()->GetContext()

// 키매니저 매크로
#define KEY_CHECK(Key, State) HYKeyMgr::GetInst()->GetKeyState(Key) == State
#define DT	HYTimeMgr::GetInst()->GetDeltaTime()

#define KEY_TAP(Key) KEY_CHECK(Key, TAP)
#define KEY_PRESSED(Key) KEY_CHECK(Key, PRESSED)
#define KEY_RELEASED(Key) KEY_CHECK(Key, RELEASED)
#define KEY_NONE(Key) KEY_CHECK(Key, NONE)

// 부호를 통해 6가지 방향을 표현할 수 있음
enum class DIR_TYPE
{
	RIGHT,
	UP,
	FRONT,
};

enum class ASSET_TYPE
{
	MESH,
	MESHDATA,
	TEXTURE,
	MATERIAL,
	SOUND,
	COMPUTE_SHADER,
	GRAPHICS_SHADER,
};

enum class COMPONENT_TYPE
{
	TRANSFORM,	// 오브젝트의 위치,크기,회전

	COLLIDER2D,	// 2차원 충돌체
	COLLIDER3D, // 3차원 충돌체

	ANIMATOR2D,	// 스프라이트 Animation
	ANIMATOR3D,	// Bone Skinning Animation

	LIGHT2D,	// 2차원 광원
	LIGHT3D,	// 3차원 광원

	CAMERA,		// 카메라 기능

	// Render Component
	MESHRENDER,
	TILEMAP,
	PARTICLESYSTEM,
	SKYBOX,
	DECAL,
	LANDSCAPE,

	END,
};

// 상수버퍼 종류
enum class CB_TYPE
{
	TRANSFORM,
	MATERIAL_CONST,
	GLOBAL_DATA,
	ANIMATION,

	END,
};