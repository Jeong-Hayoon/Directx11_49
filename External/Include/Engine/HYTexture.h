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

    // 텍스처를 로딩하는 것이 아니라 메모리 상에 만드는 함수
    // 에셋 매니저에서만 접근 가능 하도록 private
    int Create(UINT _Width, UINT _Height
        , DXGI_FORMAT _Format, UINT _BindFlag
        , D3D11_USAGE _Usage = D3D11_USAGE_DEFAULT);

    int Create(ComPtr<ID3D11Texture2D> _tex2D);



public:
    void UpdateData(int _RegisterNum);

    // 정적 멤버함수로 두게 되면 객체 생성 없이 호출 가능
    static void Clear(int _RegisterNum);


    // ScratchImage, Desc 둘 다 가로, 세로 정보 존재
    // ScratchImage : 로딩할 때만 가능, 직접 GPU에 생성할 때는 존재하지 않음
    // Desc는 어떤 상황에서든 채워지기 때문에 Desc 사용
    UINT GetWidth() { return m_Desc.Width; }
    UINT GetHeight() { return m_Desc.Height; }

    // 뷰를 반환해주는 함수
    ComPtr<ID3D11RenderTargetView>    GetRTV() { return m_RTV; }
    ComPtr<ID3D11DepthStencilView>    GetDSV() { return m_DSV; }
    ComPtr<ID3D11ShaderResourceView>  GetSRV() { return m_SRV; }
    ComPtr<ID3D11UnorderedAccessView> GetUAV() { return m_UAV; }

    // Texture2D를 반환하는 함수
    ComPtr<ID3D11Texture2D>           GetTex2D() { return m_Tex2D; }
   
public:
    HYTexture();
    ~HYTexture();

    friend class HYAssetMgr;

};

