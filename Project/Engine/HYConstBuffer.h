#pragma once
#include "HYEntity.h"

class HYConstBuffer :
    public HYEntity
{
private:
    D3D11_BUFFER_DESC       m_Desc;
    ComPtr<ID3D11Buffer>    m_CB;

    UINT                    m_ElementSize;
    UINT                    m_ElementCount;


public:
    // 버퍼를 만들 때 크기를 인자로
    // _ElementSize : 오브젝트 하나의 간격(요소 사이즈)
    // _ElementCount : 요소 개수
    // 상수 버퍼의 총(최대) 크기 : _ElementSize * _ElementCount
    int Create(UINT _ElementSize, UINT _ElementCount);

    // 데이터를 상수버퍼에 세팅하는 함수
    // _Src = source 원본 데이터, _ElementCount = 0 : 주소로부터 
    // _ElementSize * _ElementCount 최대 크기로 설정하겠다는 뜻
    // UINT _ElementCount = 0 : 설정을 안해준다면 최대크기가 default값
    void SetData(void* _Src, UINT _ElementCount = 0);

    // 레지스터 번호를 입력해주면 그 레지스터로 바인딩해주는 함수
    void UpdateData(UINT _RegisterNum);



public:
    HYConstBuffer();
    ~HYConstBuffer();
};

