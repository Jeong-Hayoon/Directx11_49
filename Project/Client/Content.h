#pragma once
#include "UI.h"

// AssetMgr �� �ε��Ǿ� �ִ� ��� Asset���� ����
class Content :
    public UI
{

public:
    virtual void render_update() override;

public:
    Content();
    ~Content();
};

