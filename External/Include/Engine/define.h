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

#define KEY_TAP(Key) KEY_CHECK(Key, TAP)
#define KEY_PRESSED(Key) KEY_CHECK(Key, PRESSED)
#define KEY_RELEASED(Key) KEY_CHECK(Key, RELEASED)
#define KEY_NONE(Key) KEY_CHECK(Key, NONE)

#define LAYER_MAX 32

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
	WIRE_FRAME,

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