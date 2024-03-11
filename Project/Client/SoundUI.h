#pragma once
#include "AssetUI.h"

class SoundUI :
    public AssetUI
{
private:
    Ptr<HYSound>     m_RecentSound;     // ���� �ֱٿ� �÷����� ����

public:
    virtual void render_update() override;

public:
    SoundUI();
    ~SoundUI();
};

