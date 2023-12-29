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
	int iArr[4];
	float fArr[4];
	Vec2 v2Arr[4];
	Vec4 v4Arr[4];
	Matrix matArr[4];

	int	bTex[TEX_PARAM::END];

	// �е�
	int iPadding[2];
};