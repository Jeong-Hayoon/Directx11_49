#pragma once
#include "Animator2DUI.h"
class AnimationEditor :
    public UI
{
private:
    bool			m_bActive;

    virtual void render_update() override;

public:
    void Load(const wstring& _strRelativePath);
    void Save(const wstring& _strRelativePath);

    bool LoadTextureFromFile(const char* filename, ID3D11ShaderResourceView** out_srv, int* out_width, int* out_height);


public:
    AnimationEditor();
    ~AnimationEditor();
};
