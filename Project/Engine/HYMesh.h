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

    // 정점 개수 정보
    UINT                    m_VtxCount;
    UINT                    m_IdxCount;

    // 버퍼가 만들어질 때 사용된 시스템 메모리상에서의 정점 정보
    // 가끔씩 GPU 메모리에 있는 데이터가 소실됨
    // 원본 정점 데이터가 있어야 버퍼를 만들 수 있기 때문에 이렇게
    // 저장을 따로 해놓아야 버퍼에 문제가 안생김
    void* m_VtxSysMem;
    void* m_IdxSysMem;

private:
    // Mesh 바인딩해주는 함수
    void UpdateData();

public:
    int Create(void* _Vtx, UINT _VtxCount, void* _Idx, UINT _IdxCount);
    void render();
    // 파티클로써 랜더링될 때
    void render_asparticle(UINT _ParticleCount);

    CLONE_DISABLE(HYMesh);

public:
    HYMesh(bool _Engine = false);
    ~HYMesh();
};

