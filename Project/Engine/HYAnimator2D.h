#pragma once
#include "HYComponent.h"

#include "HYTexture.h"

class HYAnim;

// Animation(��ü) ��� �� ������ ��� ��� -> �����ϸ� Atlas ����� ��
class HYAnimator2D :
    public HYComponent
{
private:
    map<wstring, HYAnim*>   m_mapAnim;
    HYAnim*                 m_CurAnim;
    bool                    m_bRepeat;

public:
    virtual void finaltick() override;

    void UpdateData();
    static void Clear();

    // _LeftTop, _SliceSize, _Offset : Pixel Unit�� ���ڸ� ������ �Լ� ���ο��� UV��ǥ�� ��ȯ
    // _FrmCount : �� ��¥������
    // _FPS: �ʴ� �� ��¥�� �ִϸ��̼�����
    void Create(const wstring& _strKey, Ptr<HYTexture> _AltasTex, Vec2 _LeftTop, Vec2 _vSliceSize, Vec2 _OffsetSize, int _FrmCount, float _FPS);

    HYAnim* FindAnim(const wstring& _strAnimName);
    void Play(const wstring& _strAnimName);


public:
    HYAnimator2D();
    ~HYAnimator2D();
};

