#pragma once
#include "UI.h"

// 현재 Level에 있는 모든 Object들 나열
class Outliner :
    public UI
{
public:
    virtual void render_update() override;

public:
    Outliner();
    ~Outliner();
};