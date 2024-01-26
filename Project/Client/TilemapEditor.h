#pragma once
#include "UI.h"

class TilemapEditor :
    public UI
{
private:

    virtual void render_update() override;

public:
    void Load(const wstring& _strRelativePath);
    void Save(const wstring& _strRelativePath);

    bool LoadTextureFromFile(const char* filename, ID3D11ShaderResourceView** out_srv, int* out_width, int* out_height);


public:
    TilemapEditor();
    ~TilemapEditor();
};
