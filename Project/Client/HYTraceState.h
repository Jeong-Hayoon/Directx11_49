#pragma once

#include <Engine/HYState.h>

class HYTraceState :
    public HYState
{
public:
    virtual void finaltick() override;
    virtual void Enter() override;
    virtual void Exit() override;

public:
    HYTraceState();
    ~HYTraceState();
};

