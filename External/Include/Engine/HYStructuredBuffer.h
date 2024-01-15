#pragma once
#include "HYEntity.h"

// System mem와 연관 X
enum class SB_TYPE
{
    READ_ONLY,
    READ_WRITE,
};

class HYStructuredBuffer :
    public HYEntity
{
private:
    ComPtr<ID3D11Buffer>                m_SB;
    // 텍스처 레지스터에 전달하기 위해 꼭 필요
    ComPtr<ID3D11ShaderResourceView>    m_SRV;

    ComPtr<ID3D11Buffer>                m_SB_Read;
    ComPtr<ID3D11Buffer>                m_SB_Write;

    // 요소(구조체)의 크기
    UINT                                m_ElementSize;
    // 요소(구조체)의 개수
    UINT                                m_ElementCount;

    SB_TYPE                             m_Type;
    // System mem와의 소통 가능 여부
    bool                                m_bSysMemMove; 

public:
    // _pSysMem - 초기 데이터 세팅으로 _ElementSize의 _ElementCount만큼 가져감
    int Create(UINT _ElementSize, UINT _ElementCount, SB_TYPE _Type, bool _bSysMemMove = false, void* _pSysMem = nullptr);
    void UpdateData(UINT _RegisterNum);
    void SetData(void* _SysMem, UINT _ElementCount = 0);
    void GetData(void* _Dest, UINT _ElementCount = 0);

    UINT GetElementSize() { return m_ElementSize; }
    UINT GetElementCount() { return m_ElementCount; }

public:
    HYStructuredBuffer();
    ~HYStructuredBuffer();
};

