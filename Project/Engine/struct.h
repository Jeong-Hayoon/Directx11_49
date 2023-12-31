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

// 필요한 정보 변화량
struct tDebugShapeInfo
{
	// 랜더링 모양
	DEBUG_SHAPE	eShape;

	// 랜더링 위치
	Vec3		vWorldPos;
	// 랜더링 크기
	Vec3		vWorldScale;
	// 회전값
	Vec3		vWorldRot;
	// 다 합쳐서 매트릭스
	Matrix		matWorld;

	// 색상 정보
	Vec3		vColor;
	// 실행 시간
	float		fLifeTime;
	// 유지 시간
	float		fDuration;
	// 깊이 테스트 여부
	bool		bDepthTest;
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

	// inv : 역행렬
	Matrix	matWorld;
	Matrix	matWorldInv;

	Matrix	matView;
	Matrix	matViewInv;

	Matrix	matProj;
	Matrix	matProjInv;

	// W 월드 행렬 V 뷰행렬 순서대로 미리 곱해놓은 행렬
	Matrix	matWV;
	// W 월드 행렬 V 뷰행렬 P 투영행렬을 순서대로 미리 곱해놓은 행렬
	Matrix	matWVP;
};

extern tTransform g_Transform;

// Shader에 전달시킬 Const data 구조체(Material)
struct tMtrlConst
{
	int iArr[4];				// 16byte
	float fArr[4];				// 16byte
	Vec2 v2Arr[4];				// 32byte
	Vec4 v4Arr[4];				// 16*4 byte
	Matrix matArr[4];			// 16*4 byte

	int	bTex[TEX_PARAM::END];	// 40byte

	// 패딩(16byte 정렬 해야함)
	int iPadding[2];			// 8byte
};

	
struct tAnimData2D
{
	// 좌상단 UV 정보
	Vec2 vLeftTop;
	Vec2 vSlizeSize;
	Vec2 vBackGround;
	Vec2 vOffset;
	// 애니메이션 사용 여부
	int  UseAnim2D;
	Vec3 iPadding;
};