#pragma once
#include "HYAsset.h"


class HYShader :
    public HYAsset
{
   
protected:
    // �ڽ� Ŭ������ ����� �� �ְԲ�
    ComPtr<ID3DBlob>    m_ErrBlob;


public:
    virtual void UpdateData() = 0;


public:
    HYShader(ASSET_TYPE _Type);
    ~HYShader();
};

