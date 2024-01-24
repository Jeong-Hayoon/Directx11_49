#pragma once
#include "HYEntity.h"

#include "HYTexture.h"

// Frame ����
struct tAnimFrm
{
    // �»���� ��ǥ
    Vec2    vLeftTop;
    // �ڸ� ������
    Vec2    vSlice;
    Vec2    vOffset;
    // ��׶��� ũ�� ����
    Vec2    vBackground;
    // �ش� �������� �� �� ������ ������
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
    // Finish Check
    bool                m_bFinish;          

    Ptr<HYTexture>      m_AtlasTex;

    float               m_AccTime;

    // UI ����
    Vec2                ui_SliceSize;
    Vec2                ui_vLeftTop;
    Vec2                ui_vBackground;
    int                 ui_FrmCount;
    float               ui_FPS;
    Vec2                ui_vOffset;


public:
    void finaltick();
    void UpdateData();

    // �ִϸ��̼��� �������� �˷��ִ� �Լ�
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


    // UI ����
    void SetSliceSize(Vec2 _vSliceSize) { ui_SliceSize = _vSliceSize; }
    void Set(Vec2 _vSliceSize) { ui_SliceSize = _vSliceSize; }

    // �ӽ�
    Vec2 GetOffset() { m_vecFrm[m_CurFrmIdx].vOffset; }



public:
    HYAnim();
    ~HYAnim();
};

