#pragma once

#include <string>
// ������ ����ü Ÿ�� �տ� t

// 3���� ������ ��ġ�Ǵ� ����
struct Vtx
{
	Vec3 vPos;		 // ������ ��ǥ
	Vec4 vColor;	 // ������ ���� ����
	Vec2 vUV;		 // UV ��ǥ�� or Texture Coodinate
};

// �ʿ��� ���� ��ȭ��
struct tDebugShapeInfo
{
	// ������ ���
	DEBUG_SHAPE	eShape;

	// ������ ��ġ
	Vec3		vWorldPos;
	// ������ ũ��
	Vec3		vWorldScale;
	// ȸ����
	Vec3		vWorldRot;
	// �� ���ļ� ��Ʈ����
	Matrix		matWorld;

	// ���� ����
	Vec3		vColor;
	// ���� �ð�
	float		fLifeTime;
	// ���� �ð�
	float		fDuration;
	// ���� �׽�Ʈ ����
	bool		bDepthTest;
};

// ������ ���� ����
struct tLightInfo
{
	Vec4	vColor;		// ���� ���� - ������ ���� ����
	Vec4	vSpecular;	// �ݻ籤 - ������ ��ü ǥ�鿡 �ݻ�� ����
	Vec4	vAmbient;	// ȯ�汤(�ֺ���) - ������ ���ؼ� ����Ǵ� �ּ����� ��, �������� ���� �߻��ϴ� �⺻���� ��� 

	Vec3	vWorldPos;	// ������ ��ġ
	Vec3	vWorldDir;	// ������ ���ϴ� ����
	float	fRadius;	// ������ ���� �ݰ�, �Ÿ� ����
	float	fAngle;		// ������ ���� ����

	int		LightType;	// ���� Ÿ��

	Vec3	vPadding;
};

struct tPixel
{
	BYTE r, g, b, a;
};

// ����ȭ ������ ���(Elements)�� �� -> 16byte ���� �ʿ�
// ��ƼŬ �ϳ��� ����ü
struct tParticle
{
	Vec4	vLocalPos;		// ���� ��ġ
	Vec4	vWorldPos;		// ���� ��ġ
	Vec4	vWorldScale;	// ũ��
	Vec4	vWorldRotation;	// ȸ����
	Vec3	vVelocity;		// �ӵ�
	Vec4	vColor;			// ����
	Vec4	vForce;			// ���ڿ� ����� ���� �� �ѷ�
	Vec3	vNoiseForce;	// NoiseForce ���� ���� ���� ��
	float	NoiseForceTime;	// NoiseForce �� ���ù��� �ð� ��

	float	NormalizeAge;	// Age �� Life �������� ����ȭ�� ��
	float	Mass;			// ����
	float	Age;			// ���� ����(��ƼŬ�� �������� �󸶳� ��������)
	float	Life;			// ����
	int		Active;			// Ȱ��ȭ, ��Ȱ��ȭ ����
};

// ��ƼŬ�� � ����� ����ϴ� ���� ��Ʈ��, ��ƼŬ ��ü ������Ʈ �� ����� ������Ʈ ���
struct tParticleModule
{
	// Spawn ��� - ���� ����
	// ���� �ּ�, �ִ밪�� �ִ� �������� �� ������ ������ ������ ���� ���� �� ����
	Vec4	vSpawnColor;	// �ʱ� �÷�
	Vec4	vSpawnMinScale;	// �ʱ� �ּ� ũ��
	Vec4	vSpawnMaxScale;	// �ʱ� �ִ� ũ��

	float	MinLife;		// �ּ� ����
	float	MaxLife;		// �ִ� ����
	float	MinMass;		// �ּ� ���� - ũ��� ������ ������� �������� ���� �ɼ��� �� �� ����
	float	MaxMass;		// �ִ� ���� - �� �ɼ��� Ű�� ũ�Ⱑ Ŭ���� ������ ū ��
	int		SpawnRate;		// �ʴ� ���� ����

	// ��ǥ�� : Local�̸� ������Ʈ�� ����ٴ�, World�� �� ����ٴ�
	int		SpaceType;		// ��ǥ��(0 : LocalSpace, 1 : WorldSpace)

	int		SpawnShape;		// ���� ����(0 : Sphere, 1 : Box)
	float	Radius;			// SpawnShape �� Sphere �� ���, ������ ���� - ������ �̳� ���� �ȿ� ���� ��ġ�� ����
	Vec4	vSpawnBoxScale;	// SpawnShape �� Box �� ���, Box �� ũ��

	// Add Velocity
	int		AddVelocityType;// 0 : From Center, 1: To Center, 2: Fix Direction
	float	MinSpeed;
	float	MaxSpeed;
	float	FixedAngle;		// �ش� ���⿡�� �������� ����(Fix Direction)
	Vec4	FixedDirection;	// ���� ����(Fix Direction)

	// Scale
	Vec4	vScaleRatio;

	// Noise Force
	float	NoiseForceScale; // ����Ǵ� ���� ũ��
	float	NoiseForceTerm;	 // ���� ���� �����ϴ� ��(������ ���� X)

	// �� ����� on/off
	int arrModuleCheck[(UINT)PARTICLE_MODULE::END];
};

// ��ƼŬ�� Ȱ��ȭ�Ǳ� ���� �� ���� ��Ƴ��� �Ǵ���
struct tSpawnCount
{
	int	SpawnCount;
	int iPadding[3];
};

// ====================
// ������� ���� ����ü
// ====================
// 16����Ʈ ������ �������� ��(GPU�� 16����Ʈ�� �޸𸮸� �о�� ������)
struct tTransform
{
	// x,y,z���� 16byte ������ ������ �ؾ��ؼ� Vec4
	// Vec4	vWorldPos;
	// ������ ����
	// Vec4    vWorldScale;
	// �����δ� �̷��� �������� ������ �� �����ϴ� ���� �ƴ϶�
	// ��� �ȿ� ������Ʈ�� ���°��� ��� �����Ŵ
	// -> �̷��� �ϰ� �Ǹ� Shader �ڵ忡���� 
	// �޾ƿ��� ������ ���ҵ� ������ �ƴ� �ϳ��� ��� ���·�
	// ������ ����

	// inv : �����
	Matrix	matWorld;
	Matrix	matWorldInv;

	Matrix	matView;
	Matrix	matViewInv;

	Matrix	matProj;
	Matrix	matProjInv;

	// W ���� ��� V ����� ������� �̸� ���س��� ���
	Matrix	matWV;
	// W ���� ��� V ����� P ��������� ������� �̸� ���س��� ���
	Matrix	matWVP;
};

extern tTransform g_Transform;

// Shader�� ���޽�ų Const data ����ü(Material)
struct tMtrlConst
{
	int iArr[4];				// 16byte
	float fArr[4];				// 16byte
	Vec2 v2Arr[4];				// 32byte
	Vec4 v4Arr[4];				// 16*4 byte
	Matrix matArr[4];			// 16*4 byte

	int	bTex[TEX_PARAM::END];	// 40byte

	// �е�(16byte ���� �ؾ���)
	int iPadding[2];			// 8byte
};

struct tAnimData2D
{
	// �»�� UV ����
	Vec2 vLeftTop;
	Vec2 vSlizeSize;
	Vec2 vBackGround;
	Vec2 vOffset;
	// �ִϸ��̼� ��� ����
	int  UseAnim2D;
	Vec3 iPadding;
};

// �������� �� ���� ����� �� ���� ����� ������� �ϰ������� ���Ƽ� 
// ��� ���� ���������� ���� ����ü
struct tGlobalData
{
	Vec2	g_RenderResolution;		// ������ �ػ�(������ �ػ󵵿� �� ��ġ���� ����, ������ �ػ󵵺��� Ŭ���� X, ���� ������ �ػ󵵸� �����Ѵٸ� ����ü��/����Ÿ�� ���� ������ؾ���)
	Vec2	g_NoiseTexResolution;	// ������ �ؽ�ó�� �ػ�
	float	g_dt;					// Delta Time
	float	g_time;					// ���� �ð� : ������ Ų �������� �ð��� �󸶳� �귶����
	int		g_Light2DCount;			// 2D ���� ���� : Level �ȿ�(����ȭ ����) ������ �� �� ����ִ���
	int		g_Light3DCount;			// 3D ���� ����
};

extern tGlobalData g_global;