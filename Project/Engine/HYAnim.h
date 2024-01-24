#pragma once
#include "HYEntity.h"

#include "HYTexture.h"

// Frame 정보
struct tAnimFrm
{
    // 좌상단의 좌표
    Vec2    vLeftTop;
    // 자를 사이즈
    Vec2    vSlice;
    Vec2    vOffset;
    // 백그라운드 크기 설정
    Vec2    vBackground;
    // 해당 프레임을 몇 초 유지할 것인지
    float   Duration;
};

class HYAnimator2D;

class HYAnim :
    public HYEntity
{
private:

    // 나를 소유하고 있는 Animator
    HYAnimator2D* m_Animator;

    vector<tAnimFrm>    m_vecFrm;
    // 현재 프레임 인덱스
    int                 m_CurFrmIdx;
    // Finish Check
    bool                m_bFinish;          

    Ptr<HYTexture>      m_AtlasTex;

    float               m_AccTime;

    // UI 관련
    Vec2                ui_SliceSize;
    Vec2                ui_vLeftTop;
    Vec2                ui_vBackground;
    int                 ui_FrmCount;
    float               ui_FPS;
    Vec2                ui_vOffset;


public:
    void finaltick();
    void UpdateData();

    // 애니메이션이 끝났는지 알려주는 함수
    bool IsFinish() { return m_bFinish; }
    void Reset()
    {
        m_bFinish = false;
        m_CurFrmIdx = 0;
        m_AccTime = 0.f;
    }

    static void Clear();
    void Create(HYAnimator2D* _Animator, Ptr<HYTexture> _Atlas, Vec2 _vLeftTop
        , Vec2 _vSliceSize, Vec2 _vOffset, Vec2 _vBackground, int _FrmCount, float _FPS);


    // UI 관련
    void SetSliceSize(Vec2 _vSliceSize) { ui_SliceSize = _vSliceSize; }
    void Set(Vec2 _vSliceSize) { ui_SliceSize = _vSliceSize; }

    // 임시
    Vec2 GetOffset() { m_vecFrm[m_CurFrmIdx].vOffset; }



public:
    HYAnim();
    ~HYAnim();
};

