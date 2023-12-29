#pragma once
#include "HYAsset.h"


class HYTexture :
    public HYAsset
{
private:
    // DirectTex���� �������ִ� Ŭ����, ������ Ȯ���ڿ� �´� �Լ��� �޸� �ε��� �� GPU�� ����
    ScratchImage                        m_Image;    // �ؽ��� �ε� �� �ý��۸޸� ����(�ý��� �޸𸮻� �ε��� �̹����� ��� ����)
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

private:
    // AssetMgr���׸� friend�� ������ �Ǿ� �־
    // �Լ� ȣ���� �ϰ� �ʹٸ� AssetMgr�� ���ؾ� ��
    virtual int Load(const wstring& _strFilePath) override;


public:
    void UpdateData(int _RegisterNum);

    // ���� ����Լ��� �ΰ� �Ǹ� ��ü ���� ���� ȣ�� ����
    static void Clear(int _RegisterNum);

   
public:
    HYTexture();
    ~HYTexture();

    friend class HYAssetMgr;

};

