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

	Matrix	matWorld;
	Matrix	matView;
	Matrix	matProj;
};

extern tTransform g_Transform;