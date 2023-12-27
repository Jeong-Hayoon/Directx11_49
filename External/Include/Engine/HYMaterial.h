#pragma once
#include "HYAsset.h"

class HYGraphicsShader;
class HYTexture;

// �Ķ���͸� ������ ���� Ŭ����
// Shader���� �ʿ���ϴ� ������ ����
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

