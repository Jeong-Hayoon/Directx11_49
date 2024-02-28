#pragma once
#include "HYComponent.h"

// ���� ���
enum class PROJ_TYPE
{
    ORTHOGRAPHIC, // ���� ����
    PERSPECTIVE,  // ���� ����
    END
};

// ī�޶� �Ŵ����� �ƴ� ī�޶� ���� �ϴ� ������Ʈ�� ī�޶�� ���
// ������ ������ ����� ī�޶� ��
class HYCamera :
    public HYComponent
{
private:
    PROJ_TYPE   m_ProjType;         // ���� ���

    // ��������(Perspective)�� �� ���
    float       m_FOV;              // �þ� ��

    // ��������(Orthographic)�� �� ���
    float       m_Width;            // �������� ���� ����
    float       m_Scale;            // �������� ����

    // Both(���������� �������� �� �� �ʿ��� ������)
    float       m_AspectRatio;      // ��Ⱦ��, ���� ���μ��� ����(ȭ�� �ػ󵵿� ���� ���εǴ� �ȼ��� �޶���)
    float       m_Far;              // ���� �ִ� �Ÿ�

    // ��ȯ ���
    Matrix      m_matView;          // �� ���
    Matrix      m_matProj;          // ���� ���

    UINT        m_LayerCheck;       

    int         m_CameraPriority;   // Camera �켱 ���� ���

    map<string, PROJ_TYPE>   m_mapCameraType[(UINT)PROJ_TYPE::END];

    // ��ü �з�
    vector<HYGameObject*>    m_vecOpaque;
    vector<HYGameObject*>    m_vecMaked;
    vector<HYGameObject*>    m_vecTransparent;
    vector<HYGameObject*>    m_vecPostProcess;

public:
    PROJ_TYPE GetProjType() { return m_ProjType; }
    void SetProjType(PROJ_TYPE _Type) { m_ProjType = _Type; }

    float GetScale() { return m_Scale; }
    void SetScale(float _Scale) { m_Scale = _Scale; }

    float GetFOV() { return m_FOV; }
    void SetFOV(float _FOV) { m_FOV = _FOV; }

    float GetFar() { return m_Far; }
    void SetFar(float _Far) { m_Far = _Far; }

    // View, Projection ����� ��ȯ���ִ� �Լ�
    const Matrix& GetViewMat() { return m_matView; }
    const Matrix& GetProjMat() { return m_matProj; }

    void SetCameraPriority(int _Priority);

    // ����� �ϴ� ���̾ ��Ʈ������ Check(�ε��� ���, �̸� ��� - ����� ����)
    // ������ ���̾ ���� �� �ֵ���, ���̾� �� ������ ����
    // _bCheck : True - �ش� ���̾ ���� / False - �ش� ���̾ ���� ����
    void LayerCheck(UINT _LayerIdx, bool _bCheck);
    void LayerCheck(const wstring& _strLayerName, bool _bCheck);
    void LayerCheckAll() { m_LayerCheck = 0xffffffff; }

    // ComponentUI ����
    void GetCameraTypeName(vector<string>& _Out);
    string GetCameraTypeName(PROJ_TYPE _Type);
    PROJ_TYPE GetCameraType(string str);

public:
    virtual void begin() override;

    virtual void finaltick() override;

    void SortObject();

    // Camera �������� Rendering
    void render();


private:
    void render(vector<HYGameObject*>& _vecObj);
    void render_postprocess();

    virtual void SaveToFile(FILE* _File) override;
    virtual void LoadFromFile(FILE* _File) override;

    CLONE(HYCamera);

public:
    HYCamera();
    ~HYCamera();
};

