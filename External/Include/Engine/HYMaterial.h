#pragma once
#include "HYAsset.h"

class HYGraphicsShader;
class HYTexture;

// 파라미터를 가지고 있을 클래스
// Shader에서 필요로하는 변수를 제공
class HYMaterial :
    public HYAsset
{
public:
    tMtrlConst          m_Const;
    HYTexture* m_arrTex[(UINT)TEX_PARAM::END];

    HYGraphicsShader* m_pShader;


public:
    void SetShader(HYGraphicsShader* _Shader) { m_pShader = _Shader; }
    HYGraphicsShader* GetShader() { return m_pShader; }

    void UpdateData();


public:
    HYMaterial();
    ~HYMaterial();
};

