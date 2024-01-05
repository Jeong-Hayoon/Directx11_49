#pragma once
#include "HYEntity.h"

#include "HYTexture.h"

// Frame ����
struct tAnimFrm
{
    // �»���� ��ǥ
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

    // ���� �����ϰ� �ִ� Animator
    HYAnimator2D* m_Animator;

    vector<tAnimFrm>    m_vecFrm;
    // ���� ������ �ε���
    int                 m_CurFrmIdx;
    // �ݺ� ��� ����
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

