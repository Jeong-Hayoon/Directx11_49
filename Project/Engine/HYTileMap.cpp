#include "pch.h"
#include "HYTileMap.h"

#include "HYAssetMgr.h"
#include "HYMesh.h"
		  
#include "HYTransform.h"

HYTileMap::HYTileMap()
	: HYRenderComponent(COMPONENT_TYPE::TILEMAP)
	, m_FaceX(2)
	, m_FaceY(2)
	, m_vTileRenderSize(Vec2(128.f, 128.f))
	, m_TileIdx(47)
{
	// Mesh�� Material�� ������ �� �����س��� ���� �����Ű�� ����
	SetMesh(HYAssetMgr::GetInst()->FindAsset<HYMesh>(L"RectMesh"));
	SetMaterial(HYAssetMgr::GetInst()->FindAsset<HYMaterial>(L"TileMapMtrl"));
}

HYTileMap::~HYTileMap()
{
}

void HYTileMap::finaltick()
{
	// (Ÿ�� ���� * Ÿ�� ������) �� ������ �� Ÿ�� ������ ����
	Vec3 vTileMapSize = Vec3(m_FaceX * m_vTileRenderSize.x, m_FaceY * m_vTileRenderSize.y, 1.f);
	Transform()->SetRelativeScale(vTileMapSize);
}

void HYTileMap::render()
{
	// ������ ��Ʋ�� �ؽ��� ����.
	GetMaterial()->SetTexParam(TEX_0, m_TileAtlas);

	// �������� Ÿ�� ����
	UINT iRow = m_TileIdx / m_MaxCol;
	UINT iCol = m_TileIdx % m_MaxCol;

	// �»�� ������ UV �������� ���
	Vec2 vLeftTopUV = Vec2((iCol * m_vTilePixelSize.x) / m_TileAtlas->GetWidth()
		, (iRow * m_vTilePixelSize.y) / m_TileAtlas->GetHeight());

	// Ÿ�� �ϳ��� ����� UV �������� ���
	Vec2 vSliceSizeUV = Vec2(m_vTilePixelSize.x / m_TileAtlas->GetWidth()
		, m_vTilePixelSize.y / m_TileAtlas->GetHeight());

	// Shader�� ������ �� ����
	GetMaterial()->SetScalarParam(VEC2_0, vLeftTopUV);
	GetMaterial()->SetScalarParam(VEC2_1, vSliceSizeUV);

	GetMaterial()->UpdateData();

	Transform()->UpdateData();

	GetMesh()->render();
}


void HYTileMap::UpdateData()
{

}


void HYTileMap::SetTileAtlas(Ptr<HYTexture> _Atlas, Vec2 _TilePixelSize)
{
	m_TileAtlas = _Atlas;
	m_vTilePixelSize = _TilePixelSize;

	m_MaxCol = m_TileAtlas->GetWidth() / (UINT)m_vTilePixelSize.x;
	m_MaxRow = m_TileAtlas->GetHeight() / (UINT)m_vTilePixelSize.y;
}