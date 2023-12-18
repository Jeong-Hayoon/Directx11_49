#pragma once

#include <string>
// 앞으로 구조체 타입 앞에 t

// 3차원 공간에 배치되는 정점
struct Vtx
{
	Vec3 vPos;		 // 정점의 좌표
	Vec4 vColor;	 // 정점의 색상 정보
	Vec2 vUV;		 // UV 좌표계 or Texture Coodinate
};

// ====================
// 상수버퍼 대응 구조체
// ====================
// 16바이트 단위로 만들어줘야 함(GPU가 16바이트로 메모리를 읽어가기 때문에)
struct tTransform
{
	// x,y,z지만 16byte 단위로 정렬을 해야해서 Vec4
	// Vec4	vWorldPos;
	// 사이즈 조절
	// Vec4    vWorldScale;
	// 앞으로는 이러한 정보들을 일일히 다 저장하는 것이 아니라
	// 행렬 안에 오브젝트의 상태값을 모두 저장시킴
	// -> 이렇게 하게 되면 Shader 코드에서도 
	// 받아오는 정보가 분할된 정보가 아닌 하나의 행렬 형태로
	// 정보가 들어옴

	Matrix	matWorld;
	Matrix	matView;
	Matrix	matProj;
};

extern tTransform g_Transform;