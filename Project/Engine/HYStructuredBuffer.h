#pragma once
#include "HYEntity.h"

// ����ȭ ������ Ÿ��
// System mem�� ���� X
enum class SB_TYPE
{
    // SRV ���� - GPU ���ε� �ɾ��� �� �б� ���� ����ȭ ����
    READ_ONLY,
    // SRV + UAV ���� - Compute Shader ������ ������ �� �а� ���Ⱑ ���ÿ� ������ ����ȭ ����
    READ_WRITE,
};

class HYStructuredBuffer :
    public HYEntity
{
private:
    ComPtr<ID3D11Buffer>                m_SB;
    // �ؽ�ó �������Ϳ� �����ϱ� ���� �� �ʿ�
    ComPtr<ID3D11ShaderResourceView>    m_SRV;
    // Compute Shader���� ���
    ComPtr<ID3D11UnorderedAccessView>   m_UAV;

    ComPtr<ID3D11Buffer>                m_SB_Read;
    ComPtr<ID3D11Buffer>                m_SB_Write;

    // ���(����ü)�� ũ��
    UINT                                m_ElementSize;
    // ���(����ü)�� ����
    UINT                                m_ElementCount;

    SB_TYPE                             m_Type;
    // System mem���� ���� ���� ����
    bool                                m_bSysMemMove; 

    UINT                                m_RegentSRV;    // �ֱٿ� ���ε��� t�������� ��ȣ
    UINT                                m_RegentUAV;    // �ֱٿ� ���ε��� u�������� ��ȣ

public:
    // _pSysMem - �ʱ� ������ �������� _ElementSize�� _ElementCount��ŭ ������
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

