#pragma once
#include "HYAsset.h"

class HYTexture :
    public HYAsset
{
private:
    // DirectTex���� �������ִ� Ŭ����, ������ Ȯ���ڿ� �´� �Լ��� �޸� �ε��� �� GPU�� ����
    ScratchImage                        m_Image;    // �ؽ��� �ε� �� �ý��۸޸�(CPU) ����(�ý��� �޸𸮻� �ε��� �̹����� ��� ����)
    ComPtr<ID3D11Texture2D>             m_Tex2D;    // �ؽ��� �����͸� GPU �޸𸮿� ����
    D3D11_TEXTURE2D_DESC                m_Desc;     // �ؽ��� ���� ����

    // �並 ���� ��Ī
    // �ϳ��� �ؽ�ó�� �������� �並 ���� �� ����
    // �� �ϳ��� �ؽ�ó�� ������ ���� ���� ����
    // RenderTargetView / UnorderedAccessView / ShaderResourceView��
    // ���� ������ �����ϳ� DepthStencilView�� ������ �Ұ���
    ComPtr<ID3D11RenderTargetView>      m_RTV;      // ����Ÿ�� �뵵
    ComPtr<ID3D11DepthStencilView>      m_DSV;      // �X�� ���ٽ� �뵵
    ComPtr<ID3D11ShaderResourceView>    m_SRV;      // ���̴����� ����ϴ� �뵵(�ؽ��� ��������(t) ���ε��� ��)
    ComPtr<ID3D11UnorderedAccessView>   m_UAV;      // GPGPU(General Purpose GPU) - ComputeShader, �б� ���� ���ð���, (Unordered Register(u) �� ���ε� ����)

    // ���� �ֱٿ� ������Ʈ�ߴ� ��ȣ ����ؼ� Clear�� �� ���
    UINT                                m_RecentNum_SRV;
    UINT                                m_RecentNum_UAV;

private:
    // AssetMgr���׸� friend�� ������ �Ǿ� �־
    // �Լ� ȣ���� �ϰ� �ʹٸ� AssetMgr�� ���ؾ� ��
    virtual int Load(const wstring& _strFilePath) override;

    // �ؽ�ó�� �ε��ϴ� ���� �ƴ϶� �޸� �� ����� �Լ�
    // ���� �Ŵ��������� ���� ���� �ϵ��� private
    int Create(UINT _Width, UINT _Height
        , DXGI_FORMAT _Format, UINT _BindFlag
        , D3D11_USAGE _Usage = D3D11_USAGE_DEFAULT);

    int Create(ComPtr<ID3D11Texture2D> _tex2D);

public:
    // ������ ���������ο����� u �������͸� ����� �� ���� ������ �б⸦ ������ ���� -> ������ �ÿ��� �ؽ�ó�� �б⸸ �� �� ����
    void UpdateData(int _RegisterNum);
    // Compute Shader �������� ���� �� ������ ���꿡 �ʿ��� �ؽ�ó�ν� �б⸸ �� �뵵����
    // u�������Ϳ� ������ ��Ű���� �����̳Ŀ� ���� �бⰡ ����
    int UpdateData_CS_SRV(int _RegisterNum);
    int UpdateData_CS_UAV(int _RegisterNum);

    // ���� ����Լ��� �ΰ� �Ǹ� ��ü ���� ���� ȣ�� ����
    static void Clear(int _RegisterNum);
    void Clear_CS_SRV();
    void Clear_CS_UAV();


    // ScratchImage, Desc �� �� ����, ���� ���� ����
    // ScratchImage : �ε��� ���� ����, ���� GPU�� ������ ���� �������� ����
    // Desc�� � ��Ȳ������ ä������ ������ Desc ���
    UINT GetWidth() { return m_Desc.Width; }
    UINT GetHeight() { return m_Desc.Height; }

    // �並 ��ȯ���ִ� �Լ�
    ComPtr<ID3D11RenderTargetView>    GetRTV() { return m_RTV; }
    ComPtr<ID3D11DepthStencilView>    GetDSV() { return m_DSV; }
    ComPtr<ID3D11ShaderResourceView>  GetSRV() { return m_SRV; }
    ComPtr<ID3D11UnorderedAccessView> GetUAV() { return m_UAV; }

    // Texture2D�� ��ȯ�ϴ� �Լ�
    ComPtr<ID3D11Texture2D>           GetTex2D() { return m_Tex2D; }

    // �ؽ�ó���� �ȼ� �������� 
    tPixel* GetPixels();

    CLONE_DISABLE(HYTexture);
   
public:
    HYTexture();
    ~HYTexture();

    friend class HYAssetMgr;

};

