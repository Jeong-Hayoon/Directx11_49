#pragma once
#include "HYAsset.h"

// 랜더링 파이프라인에서 이 물체가 랜더링 되기 위해서 각 단계별로 해야 될 일
class HYShader :
    public HYAsset
{
    // 자식 클래스가 사용할 수 있게끔
protected:
    // Shader 컴파일 시 실패 잡아내도록 
    ComPtr<ID3DBlob>    m_ErrBlob;

public:
    virtual void UpdateData() = 0;


public:
    HYShader(ASSET_TYPE _Type);
    ~HYShader();
};

