#pragma once
#include "AssetUI.h"

class SoundUI :
    public AssetUI
{
private:
    Ptr<HYSound>     m_RecentSound;     // 가장 최근에 플레이한 사운드

public:
    virtual void render_update() override;

public:
    SoundUI();
    ~SoundUI();
};

