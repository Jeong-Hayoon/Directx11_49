#pragma once
#include "AssetUI.h"
class MaterialUI :
    public AssetUI
{
private:
    // 선택한 Parameter가 무엇인지
    TEX_PARAM       m_SelectTexParam;

public:
    virtual void render_update() override;

public:
    void SelectTexture(DWORD_PTR _dwData);

public:
    MaterialUI();
    ~MaterialUI();
};

