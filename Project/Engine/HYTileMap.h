#pragma once
#include "HYRenderComponent.h"

class HYStructuredBuffer;

struct tTileInfo
{
    // UV ���� �»��
    Vec2 vLeftTopUV;
    // 
    int  bRender;
    int  padding;
};

// Render Component�� ��ӹ޾ұ� ������ Mesh Render �ʿ� ����
class HYTileMap :
    public HYRenderComponent
{
private:
    UINT                m_FaceX;            // ���� Ÿ�� ����
    UINT                m_FaceY;            // ���� Ÿ�� ����
    Vec2                m_vTileRenderSize;  // �������� Ÿ���� 1ĭ ������

    // Vec3                m_vTileMapWorldPos; -> ����� �ȵ�

    Ptr<HYTexture>      m_TileAtlas;
    Vec2                m_vTilePixelSize;   // ���� �ؽ�ó���� Ÿ�� �� ĭ�� ������
    Vec2                m_vSliceSizeUV;

    UINT                m_MaxCol;           // ���� �ؽ�ó���� �� ��
    UINT                m_MaxRow;           // ���� �ؽ�ó���� �� ��

    vector<tTileInfo>   m_vecTileInfo;
    HYStructuredBuffer* m_TileInfoBuffer;

    // UI ����
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

    // UI ����
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


