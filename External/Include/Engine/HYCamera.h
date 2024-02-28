#pragma once
#include "HYComponent.h"

// 투영 방식
enum class PROJ_TYPE
{
    ORTHOGRAPHIC, // 직교 투영
    PERSPECTIVE,  // 원근 투영
    END
};

// 카메라를 매니저가 아닌 카메라 역할 하는 오브젝트가 카메라로 취급
// 앞으로 랜더링 기능은 카메라가 함
class HYCamera :
    public HYComponent
{
private:
    PROJ_TYPE   m_ProjType;         // 투영 방식

    // 원근투영(Perspective)을 할 경우
    float       m_FOV;              // 시야 각

    // 직교투영(Orthographic)을 할 경우
    float       m_Width;            // 직교투영 가로 길이
    float       m_Scale;            // 직교투영 배율

    // Both(원근투영과 직교투영 둘 다 필요한 데이터)
    float       m_AspectRatio;      // 종횡비, 투영 가로세로 비율(화면 해상도에 따라 맵핑되는 픽셀이 달라짐)
    float       m_Far;              // 투영 최대 거리

    // 변환 행렬
    Matrix      m_matView;          // 뷰 행렬
    Matrix      m_matProj;          // 투영 행렬

    UINT        m_LayerCheck;       

    int         m_CameraPriority;   // Camera 우선 순위 기록

    map<string, PROJ_TYPE>   m_mapCameraType[(UINT)PROJ_TYPE::END];

    // 물체 분류
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

    // View, Projection 행렬을 반환해주는 함수
    const Matrix& GetViewMat() { return m_matView; }
    const Matrix& GetProjMat() { return m_matProj; }

    void SetCameraPriority(int _Priority);

    // 찍고자 하는 레이어를 비트값으로 Check(인덱스 방식, 이름 방식 - 기능은 같음)
    // 지정된 레이어만 찍을 수 있도록, 레이어 중 누구를 볼지
    // _bCheck : True - 해당 레이어를 찍음 / False - 해당 레이어를 찍지 않음
    void LayerCheck(UINT _LayerIdx, bool _bCheck);
    void LayerCheck(const wstring& _strLayerName, bool _bCheck);
    void LayerCheckAll() { m_LayerCheck = 0xffffffff; }

    // ComponentUI 관련
    void GetCameraTypeName(vector<string>& _Out);
    string GetCameraTypeName(PROJ_TYPE _Type);
    PROJ_TYPE GetCameraType(string str);

public:
    virtual void begin() override;

    virtual void finaltick() override;

    void SortObject();

    // Camera 시점으로 Rendering
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

