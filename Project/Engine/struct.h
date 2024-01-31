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

// 광원의 공통 정보
struct tLightInfo
{
	Vec4	vColor;		// 빛의 색상 - 광원의 순수 색상
	Vec4	vSpecular;	// 반사광 - 광원이 물체 표면에 반사된 색상
	Vec4	vAmbient;	// 환경광(주변광) - 광원에 의해서 보장되는 최소한의 빛, 광원으로 인해 발생하는 기본적인 밝기 

	Vec3	vWorldPos;	// 광원의 위치
	Vec3	vWorldDir;	// 광원이 향하는 방향
	float	fRadius;	// 광원의 영향 반경, 거리 정보
	float	fAngle;		// 광원의 범위 각도

	int		LightType;	// 광원 타입

	Vec3	vPadding;
};

struct tPixel
{
	BYTE r, g, b, a;
};

// 구조화 버퍼의 요소(Elements)가 됨 -> 16byte 정렬 필요
// 파티클 하나의 구조체
struct tParticle
{
	Vec4	vLocalPos;		// 로컬 위치
	Vec4	vWorldPos;		// 월드 위치
	Vec4	vWorldScale;	// 크기
	Vec4	vWorldRotation;	// 회전값
	Vec3	vVelocity;		// 속도
	Vec4	vColor;			// 색상
	Vec4	vForce;			// 입자에 적용된 누적 힘 총량
	Vec3	vNoiseForce;	// NoiseForce 모듈로 인한 랜덤 힘
	float	NoiseForceTime;	// NoiseForce 를 세팅받은 시간 값

	float	NormalizeAge;	// Age 를 Life 기준으로 정규화한 값
	float	Mass;			// 질량
	float	Age;			// 현재 나이(파티클이 생성된지 얼마나 지났는지)
	float	Life;			// 수명
	int		Active;			// 활성화, 비활성화 여부
};

// 파티클의 어떤 기능을 담당하는 작은 파트들, 파티클 전체 업데이트 시 사용할 업데이트 방식
struct tParticleModule
{
	// Spawn 모듈 - 생성 관련
	// 각각 최소, 최대값이 있는 변수들은 그 사이의 랜덤한 값으로 값을 가질 수 있음
	Vec4	vSpawnColor;	// 초기 컬러
	Vec4	vSpawnMinScale;	// 초기 최소 크기
	Vec4	vSpawnMaxScale;	// 초기 최대 크기

	float	MinLife;		// 최소 수명
	float	MaxLife;		// 최대 수명
	float	MinMass;		// 최소 질량 - 크기와 질량이 비례할지 안할지에 대한 옵션을 줄 수 있음
	float	MaxMass;		// 최대 질량 - 그 옵션을 키면 크기가 클수록 질량이 큰 것
	int		SpawnRate;		// 초당 생성 개수

	// 좌표계 : Local이면 오브젝트를 따라다님, World면 안 따라다님
	int		SpaceType;		// 좌표계(0 : LocalSpace, 1 : WorldSpace)

	int		SpawnShape;		// 스폰 범위(0 : Sphere, 1 : Box)
	float	Radius;			// SpawnShape 가 Sphere 인 경우, 반지름 길이 - 반지름 이내 영역 안에 랜덤 위치에 스폰
	Vec4	vSpawnBoxScale;	// SpawnShape 가 Box 인 경우, Box 의 크기

	// Add Velocity
	int		AddVelocityType;// 0 : From Center, 1: To Center, 2: Fix Direction
	float	MinSpeed;
	float	MaxSpeed;
	float	FixedAngle;		// 해당 방향에서 랜덤범위 각도(Fix Direction)
	Vec4	FixedDirection;	// 지정 방향(Fix Direction)

	// Scale
	Vec4	vScaleRatio;

	// Noise Force
	float	NoiseForceScale; // 적용되는 힘의 크기
	float	NoiseForceTerm;	 // 랜덤 힘을 추출하는 텀(프레임 단위 X)

	// 각 모듈의 on/off
	int arrModuleCheck[(UINT)PARTICLE_MODULE::END];
};

// 파티클이 활성화되기 위해 몇 개가 살아나야 되는지
struct tSpawnCount
{
	int	SpawnCount;
	int iPadding[3];
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

// 랜더링할 때 자주 사용할 것 같은 잡다한 상수값을 일괄적으로 몰아서 
// 상수 버퍼 대응용으로 만든 구조체
struct tGlobalData
{
	Vec2	g_RenderResolution;		// 렌더링 해상도(윈도우 해상도와 늘 일치하지 않음, 윈도우 해상도보다 클일은 X, 만약 랜더링 해상도를 변경한다면 스왑체인/랜더타겟 등을 재생성해야함)
	Vec2	g_NoiseTexResolution;	// 노이즈 텍스처의 해상도
	float	g_dt;					// Delta Time
	float	g_time;					// 누적 시간 : 게임을 킨 순간부터 시간이 얼마나 흘렀는지
	int		g_Light2DCount;			// 2D 광원 개수 : Level 안에(구조화 버퍼) 광원이 몇 개 들어있는지
	int		g_Light3DCount;			// 3D 광원 개수
};

extern tGlobalData g_global;