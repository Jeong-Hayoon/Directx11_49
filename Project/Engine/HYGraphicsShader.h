#pragma once
#include "HYShader.h"

struct tScalarParam
{
    SCALAR_PARAM    Type;
    string          Desc;   // 파라미터의 정보
};

struct tTexParam
{
    TEX_PARAM       Type;
    string          Desc;
};

// 랜더링할 때 필요한 모든 요소를 모아두고
// 랜더링 파이프라인에 맞춰 각 과정을 어떻게 진행할지를 전체적으로 명시
class HYGraphicsShader :
    public HYShader
{
private:
    // Shader의 실제 Binary Code들을 저장
    ComPtr<ID3DBlob>    m_VSBlob;
    ComPtr<ID3DBlob>    m_HSBlob;
    ComPtr<ID3DBlob>    m_DSBlob;
    ComPtr<ID3DBlob>    m_GSBlob;
    ComPtr<ID3DBlob>    m_PSBlob;

    ComPtr<ID3D11VertexShader>      m_VS;
    ComPtr<ID3D11HullShader>        m_HS;
    ComPtr<ID3D11DomainShader>      m_DS;
    ComPtr<ID3D11GeometryShader>    m_GS;
    ComPtr<ID3D11PixelShader>       m_PS;

    // 정점의 구조는 하나로 통일(그래야 레이아웃을 하나만 만들어도 됨)
    ComPtr<ID3D11InputLayout>       m_Layout;

    // Vertex Shader에서 정점을 출력시키면 Rasterizer에서 정점을 어떻게 분석할건지 알려줌
    D3D11_PRIMITIVE_TOPOLOGY        m_Topology;

    // Rasterizer State
    RS_TYPE                         m_RSType;

    // OutputMerge
    // DepthStencilState
    DS_TYPE                         m_DSType;

    // BlendState
    BS_TYPE                         m_BSType;

    // Shader Domain
    SHADER_DOMAIN                   m_Domain;

    // Shader 파라미터 목록
    vector<tScalarParam>            m_ScalarParam;
    vector<tTexParam>               m_TexParam;

public:
    int CreateVertexShader(const wstring& _strRelativePath, const string& _strFuncName);
    int CreateHullShader();
    int CreateDomainShader();
    int CreateGeometryShader(const wstring& _strRelativePath, const string& _strFuncName);
    int CreatePixelShader(const wstring& _strRelativePath, const string& _strFuncName);

    D3D11_PRIMITIVE_TOPOLOGY GetTopology() { return m_Topology; }

    void SetTopology(D3D11_PRIMITIVE_TOPOLOGY _Topology) { m_Topology = _Topology; }
    void SetRSType(RS_TYPE _Type) { m_RSType = _Type; }
    void SetDSType(DS_TYPE _Type) { m_DSType = _Type; }
    void SetBSType(BS_TYPE _Type) { m_BSType = _Type; }

    void SetDomain(SHADER_DOMAIN _domain) { m_Domain = _domain; }

    SHADER_DOMAIN GetDomain() { return m_Domain; }

    void AddScalarParam(SCALAR_PARAM _Param, const string& _Desc) { m_ScalarParam.push_back(tScalarParam{ _Param , _Desc }); }
    void AddTexParam(TEX_PARAM _Param, const string& _Desc) { m_TexParam.push_back(tTexParam{ _Param , _Desc }); }
    const vector<tScalarParam>& GetScalarParam() { return  m_ScalarParam; }
    const vector<tTexParam>& GetTexParam() { return m_TexParam; }

public:
    virtual int UpdateData();

public:
    HYGraphicsShader();
    ~HYGraphicsShader();
};

