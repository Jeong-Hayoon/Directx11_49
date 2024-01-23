#pragma once
#include "HYShader.h"

#include "HYStructuredBuffer.h"
#include "HYTexture.h"

// 모든 Compute Shader 객체의 부모 클래스
// 특정 기능 하나를 수행하기 위해 만들어짐 -> Compute Shader는 객체마다 하는 일이 다 다름
class HYComputeShader :
    public HYShader
{
private:
    // HLSL 문법으로 작성 -> 컴파일을 진행 -> 완성된 컴파일된 코드가 Blob에 들어감
    ComPtr<ID3DBlob>            m_CSBlob;
    ComPtr<ID3D11ComputeShader> m_CS;

    // 그룹 개수
    UINT                        m_GroupX;
    UINT                        m_GroupY;
    UINT                        m_GroupZ;

    // 스레드 개수
protected:
    const UINT                  m_ThreadX;
    const UINT                  m_ThreadY;
    const UINT                  m_ThreadZ;

    // 연산에 필요한 값들을 재질 상수 버퍼를 이용해서 전달
    tMtrlConst                  m_Const;

public:
    int Create(const wstring& _strRelativePath, const string& _strFuncName);

public:
    void Execute();

protected:
    UINT GetGroupX() { return m_GroupX; }
    UINT GetGroupY() { return m_GroupY; }
    UINT GetGroupZ() { return m_GroupZ; }

    void SetGroupX(UINT _Group) { m_GroupX = _Group; }
    void SetGroupY(UINT _Group) { m_GroupY = _Group; }
    void SetGroupZ(UINT _Group) { m_GroupZ = _Group; }

private:
    virtual int UpdateData() = 0;
    virtual void Clear() = 0;

public:
    HYComputeShader(UINT _ThreadX, UINT _ThreadY, UINT _ThreadZ);
    ~HYComputeShader();
};


