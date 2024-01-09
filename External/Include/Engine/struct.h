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