#pragma once
#include "HYEntity.h"

#include "HYTexture.h"

// Frame 정보
struct tAnimFrm
{
    // 좌상단의 좌표
    Vec2    vLeftTop;
    Vec2    vSlice;
    Vec2    vOffset;
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
    // 반복 재생 여부
    bool                m_bFinish;          

    Ptr<HYTexture>       m_AtlasTex;

    float               m_AccTime;

public:
    void finaltick();
    void UpdateData();
    static void Clear();
    void Create(HYAnimator2D* _Animator, Ptr<HYTexture> _Atlas, Vec2 _vLeftTop
        , Vec2 _vSliceSize, Vec2 _vOffset, int _FrmCount, float _FPS);



public:
    HYAnim();
    ~HYAnim();
};

