#pragma once
#include "HYAsset.h"


class HYMesh :
    public HYAsset
{
private:
    ComPtr<ID3D11Buffer>	m_VB;
    ComPtr<ID3D11Buffer>	m_IB;

    D3D11_BUFFER_DESC       m_VBDesc;
    D3D11_BUFFER_DESC       m_IBDesc;

    // ���� ���� ����
    UINT                    m_VtxCount;
    UINT                    m_IdxCount;

    // ���۰� ������� �� ���� �ý��� �޸𸮻󿡼��� ���� ����
    // ������ GPU �޸𸮿� �ִ� �����Ͱ� �ҽǵ�
    // ���� ���� �����Ͱ� �־�� ���۸� ���� �� �ֱ� ������ �̷���
    // ������ ���� �س��ƾ� ���ۿ� ������ �Ȼ���
    void* m_VtxSysMem;
    void* m_IdxSysMem;

private:
    // Mesh ���ε����ִ� �Լ�
    void UpdateData();

public:
    int Create(void* _Vtx, UINT _VtxCount, void* _Idx, UINT _IdxCount);
    void render();

public:
    HYMesh();
    ~HYMesh();
};

