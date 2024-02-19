#pragma once
#include "HYEntity.h"

// 구조화 버퍼의 타입
// System mem와 연관 X
enum class SB_TYPE
{
    // SRV 생성 - GPU 바인딩 걸었을 때 읽기 전용 구조화 버퍼
    READ_ONLY,
    // SRV + UAV 생성 - Compute Shader 시점에 전달할 때 읽고 쓰기가 동시에 가능한 구조화 버퍼
    READ_WRITE,
};

class HYStructuredBuffer :
    public HYEntity
{
private:
    ComPtr<ID3D11Buffer>                m_SB;
    // 텍스처 레지스터에 전달하기 위해 꼭 필요
    ComPtr<ID3D11ShaderResourceView>    m_SRV;
    // Compute Shader에서 사용
    ComPtr<ID3D11UnorderedAccessView>   m_UAV;

    ComPtr<ID3D11Buffer>                m_SB_Read;
    ComPtr<ID3D11Buffer>                m_SB_Write;

    // 요소(구조체)의 크기
    UINT                                m_ElementSize;
    // 요소(구조체)의 개수
    UINT                                m_ElementCount;

    SB_TYPE                             m_Type;
    // System mem와의 소통 가능 여부
    bool                                m_bSysMemMove; 

    UINT                                m_RegentSRV;    // 최근에 바인딩한 t레지스터 번호
    UINT                                m_RegentUAV;    // 최근에 바인딩한 u레지스터 번호

public:
    // _pSysMem - 초기 데이터 세팅으로 _ElementSize의 _ElementCount만큼 가져감
    int Create(UINT _ElementSize, UINT _ElementCount, SB_TYPE _Type, bool _bSysMemMove = false, void* _pSysMem = nullptr);
    void UpdateData(UINT _RegisterNum);

    int UpdateData_CS_SRV(UINT _RegisterNum);
    int UpdateData_CS_UAV(UINT _RegisterNum);

    void Clear(UINT _RegisterNum);
    void Clear_CS_SRV();
    void Clear_CS_UAV();

    void SetData(void* _SysMem, UINT _ElementCount = 0);
    void GetData(void* _Dest, UINT _ElementCount = 0);

    UINT GetElementSize() { return m_ElementSize; }
    UINT GetElementCount() { return m_ElementCount; }

    CLONE(HYStructuredBuffer);

public:
    HYStructuredBuffer();
    HYStructuredBuffer(const HYStructuredBuffer& _OriginBuffer);
    ~HYStructuredBuffer();
};

