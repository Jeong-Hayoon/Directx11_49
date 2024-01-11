#pragma once
#include "HYAsset.h"

// ������ ���������ο��� �� ��ü�� ������ �Ǳ� ���ؼ� �� �ܰ躰�� �ؾ� �� ��
class HYShader :
    public HYAsset
{
    // �ڽ� Ŭ������ ����� �� �ְԲ�
protected:
    // Shader ������ �� ���� ��Ƴ����� 
    ComPtr<ID3DBlob>    m_ErrBlob;

public:
    virtual void UpdateData() = 0;


public:
    HYShader(ASSET_TYPE _Type);
    ~HYShader();
};

