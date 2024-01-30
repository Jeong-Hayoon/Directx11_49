#pragma once

// friend�� ���س����� �ڽ� Ŭ�������� �����ڿ� �Ҹ��ڸ� ���ܳ��� ������ ��ü ������ �ȵ�
#define SINGLE(classtype)  	private:\
								classtype();\
								~classtype();\
							friend class HYSingleton<classtype>;

// ���Ǹ� ���� define 
#define DEVICE HYDevice::GetInst()->GetDevice()
#define CONTEXT HYDevice::GetInst()->GetContext()

// Ű�Ŵ��� ��ũ��
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


// ��ȣ�� ���� 6���� ������ ǥ���� �� ����
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
	TRANSFORM,	// ������Ʈ�� ��ġ,ũ��,ȸ��

	COLLIDER2D,	// 2���� �浹ü
	COLLIDER3D, // 3���� �浹ü

	ANIMATOR2D,	// ��������Ʈ Animation
	ANIMATOR3D,	// Bone Skinning Animation

	LIGHT2D,	// 2���� ����
	LIGHT3D,	// 3���� ����

	CAMERA,		// ī�޶� ���

	// Render Component
	MESHRENDER,
	TILEMAP,
	PARTICLESYSTEM,
	SKYBOX,
	DECAL,
	LANDSCAPE,

	END,		// END�� �������� ������ �������� �����ϴ� �⺻ Component
				// �Ʒ��� ���� �߰��ϴ� Custom Component

	SCRIPT,		// Script�� Component�ȿ� �������� �����Ƿ� END �������� ��ġ
};

// ������� ����
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
	CULL_BACK,		// Default �ɼ�
	CULL_FRONT,
	CULL_NONE,
	WIRE_FRAME,		// �������� PS ���, ��� Triangle List�� �� ��� �밢���� ����

	END,
};

// DepthStencil State Type
enum class DS_TYPE
{
	LESS,				// ������ ���			���� ��� O
	LESS_EQUAL,			// �۰ų� ������ ���		���� ��� O

	GRATER,				// �� �ָ� ���			���� ��� O
	GRATER_EQUAL,		// �� �ְų� ������ ���	���� ��� O

	NO_TEST,			// ���� �׽�Ʈ X			���� ��� O
	NO_WRITE,			// ���� �׽�Ʈ O			���� ��� X

	NO_TEST_NO_WRITE,	// ���� �׽�Ʈ X			���� ��� X

	END,
};

// Blend State
enum class BS_TYPE
{
	DEFAULT,

	ALPHA_BLEND,

	ONE_ZERO,

	ONE_ONE,		// 1�� 1 ȥ�պ��� ����

	END,
};

// ScalarParam : Material�� ���� �Ѱ��� �� �ִ� �Ϲ� ��� �Ķ������ Ÿ��
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

// Material�� Shader�� �ʿ���ϴ� Texture�� ������ �� �Ķ����
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
	// ���⼺ ����(���� ���� ����), �¾�
	DIRECTIONAL,
	// ������, ������� ���� 
	POINT,
	SPOT,
	END
};

// �ۼ��ϴ� ���� == ������ ����(Layer ������ �ƴϰ� ��)
// �ش� Shader�� � ������ �׷����� ���� �з��ϴ� Shader�� Flag��
enum class SHADER_DOMAIN
{
	DOMAIN_OPAQUE,			// ������(Mesh�� �� ä��� ��)
	DOMAIN_MASKED,			// ������ or ����(���� ���ϴ� ���·� discard)
	DOMAIN_TRANSPARENT,		// ������(���� ���� ���� ������ ������)
	DOMAIN_POSTPROCESS,		// ��ó��(�߰��� ������ �ϴ� ���̴�)

	DOMAIN_DEBUG,			// Level ���� ��ü�� �� �׸��� ���� ������, Level ���� �����ϸ� �ȵ�
};

enum class PARTICLE_MODULE
{
	SPAWN,	// ��ƼŬ ���� ����
	DRAG,	// ���� ���� ���
	SCALE,	
	ADD_VELOCITY,

	END,
};