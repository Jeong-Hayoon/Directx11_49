#pragma once
#include "HYComponent.h"

#include "HYTexture.h"

class HYAnim;

// Animation(객체) 등록 및 랜더링 기능 담당 -> 가능하면 Atlas 사용할 것
class HYAnimator2D :
    public HYComponent
{
private:
    map<wstring, HYAnim*>   m_mapAnim;
    HYAnim*                 m_CurAnim;
    bool                    m_bRepeat;

    // UI 관련
    wstring                 m_CurAnimName;

public:
    virtual void finaltick() override;

    void UpdateData();
    static void Clear();

    // _LeftTop, _SliceSize, _Offset : Pixel Unit로 인자를 받지만 함수 내부에서 UV좌표료 변환
    // _FrmCount : 몇 장짜리인지
    // _FPS: 초당 몇 장짜리 애니메이션인지
    // _Background : 백그라운드 사이즈
    void Create(const wstring& _strKey, Ptr<HYTexture> _AltasTex, Vec2 _LeftTop, Vec2 _vSliceSize, Vec2 _OffsetSize, Vec2 _Background, int _FrmCount, float _FPS);

    HYAnim* FindAnim(const wstring& _strAnimName);
    void Play(const wstring& _strAnimName, bool _bRepeat = true);

    // UI 관련
    void GetAnimationName(vector<string>& _Out);
    wstring GetCurAnimName() { return m_CurAnimName; }

    //bool IsRepeat() { return m_bRepeat; }
    //void SetRepeat(bool _repeat) { m_bRepeat = _repeat; }

    bool IsRepeat() { return m_bRepeat; }
    void SetRepeat(bool _repeat) { m_bRepeat = _repeat; }

    virtual void SaveToFile(FILE* _File) override;
    virtual void LoadFromFile(FILE* _File) override;

    CLONE(HYAnimator2D);

public:
    HYAnimator2D();
    HYAnimator2D(const HYAnimator2D& _OriginAnimator);
    ~HYAnimator2D();
};

