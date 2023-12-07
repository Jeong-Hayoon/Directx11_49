#pragma once
#include "HYAsset.h"


class HYShader :
    public HYAsset
{
   
protected:
    // 자식 클래스가 사용할 수 있게끔
    ComPtr<ID3DBlob>    m_ErrBlob;


public:
    virtual void UpdateData() = 0;


public:
    HYShader(ASSET_TYPE _Type);
    ~HYShader();
};

