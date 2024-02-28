#pragma once
#include "HYRenderComponent.h"

class HYStructuredBuffer;

struct tTileInfo
{
    // UV 기준 좌상단
    Vec2 vLeftTopUV;
    // 
    int  bRender;
    int  padding;
};

// Render Component를 상속받았기 때문에 Mesh Render 필요 없음
class HYTileMap :
    public HYRenderComponent
{
private:
    UINT                m_FaceX;            // 가로 타일 개수
    UINT                m_FaceY;            // 세로 타일 개수
    Vec2                m_vTileRenderSize;  // 랜더링될 타일의 1칸 사이즈

    // Vec3                m_vTileMapWorldPos; -> 사용이 안됨

    Ptr<HYTexture>      m_TileAtlas;
    Vec2                m_vTilePixelSize;   // 실제 텍스처에서 타일 한 칸의 사이즈
    Vec2                m_vSliceSizeUV;

    UINT                m_MaxCol;           // 실제 텍스처에서 총 열
    UINT                m_MaxRow;           // 실제 텍스처에서 총 행

    vector<tTileInfo>   m_vecTileInfo;
    HYStructuredBuffer* m_TileInfoBuffer;

    // UI 관련
   int                UIROW;
   int                UICOL;
   int                UIIDX;

public:
    void SetTileAtlas(Ptr<HYTexture> _Atlas, Vec2 _TilePixelSize);
    Ptr<HYTexture> GetTileAtlas() { return m_TileAtlas; }

    void SetFace(UINT _FaceX, UINT _FaceY);

    UINT GetFaceX() { return m_FaceX; }
    UINT GetFaceY() { return m_FaceY; }

    void SetTileIndex(UINT _Row, UINT _Col, UINT _ImgIdx);

    // UI 관련
    void SetFaceX(int _FaceX) { m_FaceX = _FaceX; }
    void SetFaceY(int _FaceY) { m_FaceY = _FaceY; }

    //void SetTileIndex();
    void SetUIROW(int _row) 
    { 
        UIROW = _row;
    }
    void SetUICOL(int _row)
    { 
        UIROW = _row;
    }
    void SetUIIDX(int _row) 
    { 
        UIROW = _row;
    }
    int GetUIROW() { return UIROW; }
    int GetUICOL() { return UICOL; }
    int GetUIIDX() { return UIIDX; }

public:
    virtual void finaltick() override;
    virtual void UpdateData() override;
    virtual void render() override;

    virtual void SaveToFile(FILE* _File) override;
    virtual void LoadFromFile(FILE* _File) override;

    CLONE(HYTileMap);

public:
    HYTileMap();
    HYTileMap(const HYTileMap& _OriginTileMap);
    ~HYTileMap();
};


