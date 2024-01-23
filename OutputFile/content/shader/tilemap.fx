#ifndef _TILEMAP
#define _TILEMAP

#include "value.fx"
#include "struct.fx"

// ==========================
// TileMap Shader

// Parameter
#define FACE_X      g_int_0     // 랜더링될 타일의 가로
#define FACE_Y      g_int_1     // 랜더링될 타일의 세로

#define vSliceUV    g_vec2_0

#define TileAtlas   g_tex_0

StructuredBuffer<tTileInfo> g_TileInfo : register(t20);
// ==========================

struct VS_IN
{
    float3 vPos : POSITION;
    float2 vUV : TEXCOORD;
};

struct VS_OUT
{
    float4 vPosition : SV_Position;
    float2 vUV : TEXCOORD;
};

VS_OUT VS_TileMap(VS_IN _in)
{
    VS_OUT output = (VS_OUT) 0.f;
    
    output.vPosition = mul(float4(_in.vPos, 1.f), g_matWVP);
    output.vUV = _in.vUV;
    
    return output;
}

float4 PS_TileMap(VS_OUT _in) : SV_Target
{
    float4 vColor = float4(1.f, 0.f, 1.f, 1.f);
        
    if (g_btex_0)
    {
        // 면 개수만큼 _in.vUV 를 배율을 늘림 -> 텍스처의 참조 범위가 면 개수만큼 증가
        float2 vUV = _in.vUV * float2(FACE_X, FACE_Y);
        // 정수 부분만을 따로 담아 놓음
        // floor : 소수점 내림 처리하는 함수
        // modf : 정수에서 실수부분만을 취하는 함수
        int2 Integer = (int2) floor(vUV);
        // 실수 부분(0~1 사이)
        vUV = vUV - Integer;
        // Integer.x : 열, Integer.y : 행(UV 좌표 기준으로)
        int bufferidx = Integer.y * FACE_X + Integer.x;
        
        // Render flag가 false이면(공백 타일이면)
        if (!g_TileInfo[bufferidx].bRender)
            discard;
        
        // 참조하려는 영역 설정
        vUV = g_TileInfo[bufferidx].vLeftTopUV + (vSliceUV * vUV);
        vColor = TileAtlas.Sample(g_sam_0, vUV);
    }
    
    return vColor;
}

#endif


