#pragma once
#include "HYAsset.h"

class HYGraphicsShader;
class HYTexture;

// 파라미터를 가지고 있을 클래스
// 1. Shader에서 필요로하는 변수(상수 데이터)를 제공
// 2. Shader에서 필요로하는 텍스처 전달
class HYMaterial :
    public HYAsset
{
public:
    // Material이 Shader한테 보내주는 상수 데이터 묶음
    tMtrlConst          m_Const;
    // Shader에서 필요로하는 텍스처
    HYTexture* m_arrTex[(UINT)TEX_PARAM::END];

    // 재질이 Shader를 참조해야 하므로 멤버 변수로 가지고 있음 
    HYGraphicsShader* m_pShader;


public:
    void SetShader(HYGraphicsShader* _Shader) { m_pShader = _Shader; }
    HYGraphicsShader* GetShader() { return m_pShader; }


    // Material이 들고 있는 데이터를 GPU로 보내는 함수
    void UpdateData();


public:
    HYMaterial();
    ~HYMaterial();
};

