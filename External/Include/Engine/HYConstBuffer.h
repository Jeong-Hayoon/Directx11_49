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

    CB_TYPE                 m_Type;

public:
    // ���۸� ���� �� ũ�⸦ ���ڷ�
    // _ElementSize : ������Ʈ �ϳ��� ����(��� ������)
    // _ElementCount : ��� ����
    // ��� ������ ��(�ִ�) ũ�� : _ElementSize * _ElementCount
    int Create(UINT _ElementSize, UINT _ElementCount);

    // �����͸� ������ۿ� �����ϴ� �Լ�
    // _Src = source ���� ������, _ElementCount = 0 : �ּҷκ��� 
    // _ElementSize * _ElementCount �ִ� ũ��� �����ϰڴٴ� ��
    // UINT _ElementCount = 0 : ������ �����شٸ� �ִ�ũ�Ⱑ default��
    void SetData(void* _Src, UINT _ElementCount = 0);

    void UpdateData();

public:
    // ������ �� ��������� Ÿ���� ��������� ��(Ÿ���� �� �������� ��� ��ȣ)
    HYConstBuffer(CB_TYPE _Type);
    ~HYConstBuffer();
};

