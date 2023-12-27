#pragma once
#include "HYAsset.h"

class HYGraphicsShader;
class HYTexture;

// �Ķ���͸� ������ ���� Ŭ����
// 1. Shader���� �ʿ���ϴ� ����(��� ������)�� ����
// 2. Shader���� �ʿ���ϴ� �ؽ�ó ����
class HYMaterial :
    public HYAsset
{
public:
    // Material�� Shader���� �����ִ� ��� ������ ����
    tMtrlConst          m_Const;
    // Shader���� �ʿ���ϴ� �ؽ�ó
    HYTexture* m_arrTex[(UINT)TEX_PARAM::END];

    // ������ Shader�� �����ؾ� �ϹǷ� ��� ������ ������ ���� 
    HYGraphicsShader* m_pShader;


public:
    void SetShader(HYGraphicsShader* _Shader) { m_pShader = _Shader; }
    HYGraphicsShader* GetShader() { return m_pShader; }


    // Material�� ��� �ִ� �����͸� GPU�� ������ �Լ�
    void UpdateData();


public:
    HYMaterial();
    ~HYMaterial();
};

