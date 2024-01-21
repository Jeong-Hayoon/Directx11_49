#pragma once
#include "HYRenderComponent.h"

class HYTileMap :
    public HYRenderComponent
{
private:
    UINT            m_FaceX;            // 가로 타일 개수
    UINT            m_FaceY;            // 세로 타일 개수
    Vec2            m_vTileRenderSize;  // 타일 1칸 사이즈

    Vec3            m_vTileMapWorldPos;

    Ptr<HYTexture>   m_TileAtlas;
    Vec2            m_vTilePixelSize;   // 실제 텍스처에서 한 타일의 사이즈

    UINT            m_MaxCol;           // 실제 텍스처에서 총 열
    UINT            m_MaxRow;           // 실제 텍스처에서 총 행

    UINT            m_TileIdx;          // 타일 인덱스


public:
    void SetTileAtlas(Ptr<HYTexture> _Atlas, Vec2 _TilePixelSize);
    Ptr<HYTexture> GetTileAtlas() { return m_TileAtlas; }

public:
    virtual void finaltick() override;
    virtual void UpdateData() override;
    virtual void render() override;

public:
    HYTileMap();
    ~HYTileMap();
};


