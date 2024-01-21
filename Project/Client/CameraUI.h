#pragma once
#include "ComponentUI.h"

class CameraUI :
    public ComponentUI
{
public:
    void ProjTypeSelect(DWORD_PTR _ptr);


public:
    virtual void render_update() override;
public:
    CameraUI();
    ~CameraUI();
};

