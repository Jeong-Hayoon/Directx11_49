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

    Ptr<HYTexture>       m_AtlasTex;

    float               m_AccTime;

public:
    void finaltick();
    void UpdateData();
    static void Clear();
    void Create(HYAnimator2D* _Animator, Ptr<HYTexture> _Atlas, Vec2 _vLeftTop
        , Vec2 _vSliceSize, Vec2 _vOffset, Vec2 _vBackground, int _FrmCount, float _FPS);



public:
    HYAnim();
    ~HYAnim();
};

