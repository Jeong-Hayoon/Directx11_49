#ifndef _STRUCT
#define _STRUCT

struct tLightColor
{
    float4 vColor; // 빛의 색상	- 광원의 순수 색상
    float4 vSpecular; // 반사광	- 광원이 물체 표변에 반사된 색상
    float4 vAmbient; // 환경광(주변광) - 광원에 의해서 보장되는 최소한의 빛
};

struct tLightInfo
{
    tLightColor Color;
    
    float3 vWorldPos; // 광원의 위치
    float3 vWorldDir; // 광원이 향하는 방향
    float fRadius; // 광원의 영향 반경, 거리 정보
    float fAngle; // 광원의 범위 각도

    int LightType; // 광원 타입 (0 : Directional, 1 : Point, 2 : Spot)
    float3 vPadding;
};

struct tTileInfo
{
    float2 vLeftTopUV;
    int bRender;
    int padding;
};

struct tParticle
{
    float4 vWorldPos; // 위치
    float4 vWorldScale; // 크기
    float4 vWorldRotation; // 회전값
    float4 vVelocity; // 속도
    float4 vColor; // 색상

    float Mass; // 질량
    float Age; // 현재 나이
    float Life; // 수명
    int Active; // 활성화, 비활성화 여부
};

struct tParticleModule
{
	// Sapwn 모듈
    float4 vSpawnColor; // 초기 컬러
    float4 vSpawnMinScale; // 초기 최소 크기
    float4 vSpawnMaxScale; // 초기 최대 크기

    float MinLife; // 최소 수명
    float MaxLife; // 최대 수명
    int SpawnRate; // 초당 생성 개수
    int SpaceType; // 좌표계(0 : LocalSpace, 1 : WorldSpace)
};


struct tSpawnCount
{
    int iSpawnCount;
    int3 iPadding;
};

#endif
