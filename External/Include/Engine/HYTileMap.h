#pragma once
#include "HYRenderComponent.h"

// Render Component�� ��ӹ޾ұ� ������ Mesh Render �ʿ� ����
class HYTileMap :
    public HYRenderComponent
{
private:
    UINT                m_FaceX;            // ���� Ÿ�� ����
    UINT                m_FaceY;            // ���� Ÿ�� ����
    Vec2                m_vTileRenderSize;  // �������� Ÿ���� 1ĭ ������

    Vec3                m_vTileMapWorldPos;

    Ptr<HYTexture>      m_TileAtlas;
    Vec2                m_vTilePixelSize;   // ���� �ؽ�ó���� Ÿ�� �� ĭ�� ������

    UINT                m_MaxCol;           // ���� �ؽ�ó���� �� ��
    UINT                m_MaxRow;           // ���� �ؽ�ó���� �� ��

    UINT                m_TileIdx;          // Ÿ�� �ε���


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


