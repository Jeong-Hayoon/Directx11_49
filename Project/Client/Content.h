#pragma once
#include "UI.h"

// AssetMgr 에 로딩되어 있는 모든 Asset들을 나열
class Content :
    public UI
{

public:
    virtual void render_update() override;

public:
    Content();
    ~Content();
};

