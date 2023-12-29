#pragma once
#include "HYAsset.h"


class HYTexture :
    public HYAsset
{
private:
    // DirectTex에서 제공해주는 클래스, 지정된 확장자에 맞는 함수로 메모리 로딩한 후 GPU로 보냄
    ScratchImage                        m_Image;    // 텍스쳐 로딩 및 시스템메모리 관리(시스템 메모리상에 로딩된 이미지를 들고 있음)
    ComPtr<ID3D11Texture2D>             m_Tex2D;    // 텍스쳐 데이터를 GPU 메모리에 관리
    D3D11_TEXTURE2D_DESC                m_Desc;     // 텍스쳐 생성 정보

    // 뷰를 통해 지칭
    // 하나의 텍스처는 여러개의 뷰를 가질 수 있음
    // 단 하나의 텍스처가 모든것을 다할 수는 없고
    // RenderTargetView / UnorderedAccessView / ShaderResourceView는
    // 여러 조합이 가능하나 DepthStencilView는 공존이 불가능
    ComPtr<ID3D11RenderTargetView>      m_RTV;      // 렌더타겟 용도
    ComPtr<ID3D11DepthStencilView>      m_DSV;      // 뎊스 스텐실 용도
    ComPtr<ID3D11ShaderResourceView>    m_SRV;      // 쉐이더에서 사용하는 용도(텍스쳐 레지스터(t) 바인딩할 때)
    ComPtr<ID3D11UnorderedAccessView>   m_UAV;      // GPGPU(General Purpose GPU) - ComputeShader, 읽기 쓰기 동시가능, (Unordered Register(u) 에 바인딩 가능)

private:
    // AssetMgr한테만 friend로 공개가 되어 있어서
    // 함수 호출을 하고 싶다면 AssetMgr를 통해야 함
    virtual int Load(const wstring& _strFilePath) override;


public:
    void UpdateData(int _RegisterNum);

    // 정적 멤버함수로 두게 되면 객체 생성 없이 호출 가능
    static void Clear(int _RegisterNum);

   
public:
    HYTexture();
    ~HYTexture();

    friend class HYAssetMgr;

};

