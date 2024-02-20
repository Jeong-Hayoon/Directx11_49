#pragma once
#include <Engine/HYGameObject.h>

class HYGameObjectEx :
    public HYGameObject
{

public:
    virtual void finaltick() override;
};

