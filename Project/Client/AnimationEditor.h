#pragma once
#include "Animator2DUI.h"
class AnimationEditor :
    public Animator2DUI
{
private:
    bool			m_bActive;

    virtual void render_update() override;

public:
    void Load(const wstring& _strRelativePath);
    void Save(const wstring& _strRelativePath);



public:
    AnimationEditor();
    ~AnimationEditor();
};
