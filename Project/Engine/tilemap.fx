#ifndef _TILEMAP
#define _TILEMAP

#include "value.fx"
#include "struct.fx"

// ==========================
// TileMap Shader

// Parameter
#define FACE_X      g_int_0     // �������� Ÿ���� ����
#define FACE_Y      g_int_1     // �������� Ÿ���� ����

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
        // �� ������ŭ _in.vUV �� ������ �ø� -> �ؽ�ó�� ���� ������ �� ������ŭ ����
        float2 vUV = _in.vUV * float2(FACE_X, FACE_Y);
        // ���� �κи��� ���� ��� ����
        // floor : �Ҽ��� ���� ó���ϴ� �Լ�
        // modf : �������� �Ǽ��κи��� ���ϴ� �Լ�
        int2 Integer = (int2) floor(vUV);
        // �Ǽ� �κ�(0~1 ����)
        vUV = vUV - Integer;
        // Integer.x : ��, Integer.y : ��(UV ��ǥ ��������)
        int bufferidx = Integer.y * FACE_X + Integer.x;
        
        // Render flag�� false�̸�(���� Ÿ���̸�)
        if (!g_TileInfo[bufferidx].bRender)
            discard;
        
        // �����Ϸ��� ���� ����
        vUV = g_TileInfo[bufferidx].vLeftTopUV + (vSliceUV * vUV);
        vColor = TileAtlas.Sample(g_sam_0, vUV);
    }
    
    return vColor;
}

#endif


