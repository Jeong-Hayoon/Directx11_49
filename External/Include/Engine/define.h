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
// double
#define DTd	HYTimeMgr::GetInst()->GetDeltaTime_d()

#define KEY_TAP(Key) KEY_CHECK(Key, TAP)
#define KEY_PRESSED(Key) KEY_CHECK(Key, PRESSED)
#define KEY_RELEASED(Key) KEY_CHECK(Key, RELEASED)
#define KEY_NONE(Key) KEY_CHECK(Key, NONE)

#define LAYER_MAX 32

#define TILE_WIDTH 128
#define TILE_HEIGHT 128


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
	END,
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

	END,		// END를 기준으로 위쪽은 엔진에서 제공하는 기본 Component
				// 아래는 따로 추가하는 Custom Component

	SCRIPT,		// Script는 Component안에 속해있지 않으므로 END 다음으로 배치
};

// 상수버퍼 종류
enum class CB_TYPE
{
	TRANSFORM,
	MATERIAL_CONST,
	ANIM2D_DATA,
	GLOBAL_DATA,
	ANIMATION,

	END,
};

// Rasterizer State Type
enum class RS_TYPE
{
	CULL_BACK,		// Default 옵션
	CULL_FRONT,
	CULL_NONE,
	WIRE_FRAME,		// 테투리만 PS 출력, 대신 Triangle List로 할 경우 대각선이 생김

	END,
};

// DepthStencil State Type
enum class DS_TYPE
{
	LESS,				// 작은게 통과			깊이 기록 O
	LESS_EQUAL,			// 작거나 같으면 통과		깊이 기록 O

	GRATER,				// 더 멀면 통과			깊이 기록 O
	GRATER_EQUAL,		// 더 멀거나 같으면 통과	깊이 기록 O

	NO_TEST,			// 깊이 테스트 X			깊이 기록 O
	NO_WRITE,			// 깊이 테스트 O			깊이 기록 X

	NO_TEST_NO_WRITE,	// 깊이 테스트 X			깊이 기록 X

	END,
};

// Blend State
enum class BS_TYPE
{
	DEFAULT,

	ALPHA_BLEND,

	ONE_ZERO,

	ONE_ONE,		// 1대 1 혼합비율 블랜드

	END,
};

// ScalarParam : Material을 통해 넘겨줄 수 있는 일반 상수 파라미터의 타입
enum SCALAR_PARAM
{
	INT_0,
	INT_1,
	INT_2,
	INT_3,

	FLOAT_0,
	FLOAT_1,
	FLOAT_2,
	FLOAT_3,

	VEC2_0,
	VEC2_1,
	VEC2_2,
	VEC2_3,

	VEC4_0,
	VEC4_1,
	VEC4_2,
	VEC4_3,

	MAT_0,
	MAT_1,
	MAT_2,
	MAT_3,
};

// Material이 Shader가 필요로하는 Texture를 전달할 때 파라미터
enum TEX_PARAM
{
	TEX_0,
	TEX_1,
	TEX_2,
	TEX_3,
	TEX_4,
	TEX_5,

	TEXCUBE_0,
	TEXCUBE_1,

	TEXARR_0,
	TEXARR_1,

	END,
};

enum class DEBUG_SHAPE
{
	// 2D
	RECT,
	CIRCLE,
	CROSS,

	// 3D
	CUBE,
	SPHERE,
};

enum class LIGHT_TYPE
{
	// 방향성 광원(월드 전역 광원), 태양
	DIRECTIONAL,
	// 점광원, 사실적인 광원 
	POINT,
	SPOT,
	END
};

// 작성하는 순서 == 랜더링 순서(Layer 순서가 아니게 됨)
// 해당 Shader가 어떤 시점에 그려져야 할지 분류하는 Shader의 Flag값
enum class SHADER_DOMAIN
{
	DOMAIN_OPAQUE,			// 불투명(Mesh를 꽉 채우는 것)
	DOMAIN_MASKED,			// 불투명 or 투명(내가 원하는 형태로 discard)
	DOMAIN_TRANSPARENT,		// 반투명(알파 블랜드 쓰면 무조건 반투명)
	DOMAIN_POSTPROCESS,		// 후처리(추가적 가공을 하는 쉐이더)

	DOMAIN_DEBUG,			// Level 안의 물체를 다 그리고 나서 랜더링, Level 내에 존재하면 안됨
};

enum class PARTICLE_MODULE
{
	SPAWN,	// 파티클 생성 관련
	DRAG,	// 감속 관련 모듈
	SCALE,	
	ADD_VELOCITY,

	END,
};