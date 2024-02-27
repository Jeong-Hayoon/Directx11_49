#include "pch.h"
#include "HYTileMap.h"

#include "HYStructuredBuffer.h"

#include "HYAssetMgr.h"
#include "HYMesh.h"
		  
#include "HYTransform.h"

HYTileMap::HYTileMap()
	: HYRenderComponent(COMPONENT_TYPE::TILEMAP)
	, m_FaceX(1)
	, m_FaceY(1)
	, m_vTileRenderSize(Vec2(128.f, 128.f))
	, m_TileInfoBuffer(nullptr)
{
	// Mesh�� Material�� ������ �� �����س��� ���� �����Ű�� ����
	SetMesh(HYAssetMgr::GetInst()->FindAsset<HYMesh>(L"RectMesh"));
	SetMaterial(HYAssetMgr::GetInst()->FindAsset<HYMaterial>(L"TileMapMtrl"));

	m_TileInfoBuffer = new HYStructuredBuffer;

	SetFace(m_FaceX, m_FaceY);
}

HYTileMap::HYTileMap(const HYTileMap& _OriginTileMap)
	: HYRenderComponent(_OriginTileMap)
	, m_FaceX(_OriginTileMap.m_FaceX)
	, m_FaceY(_OriginTileMap.m_FaceY)
	, m_vTileRenderSize(_OriginTileMap.m_vTileRenderSize)
	, m_vTileMapWorldPos(_OriginTileMap.m_vTileMapWorldPos)
	, m_TileAtlas(_OriginTileMap.m_TileAtlas)
	, m_vTilePixelSize(_OriginTileMap.m_vTilePixelSize)
	, m_vSliceSizeUV(_OriginTileMap.m_vSliceSizeUV)
	, m_MaxCol(_OriginTileMap.m_MaxCol)
	, m_MaxRow(_OriginTileMap.m_MaxRow)
	, m_vecTileInfo(_OriginTileMap.m_vecTileInfo)
	, m_TileInfoBuffer(nullptr)
{
	if (nullptr != _OriginTileMap.m_TileInfoBuffer)
	{
		m_TileInfoBuffer = _OriginTileMap.m_TileInfoBuffer->Clone();
	}
}

HYTileMap::~HYTileMap()
{
	if (nullptr != m_TileInfoBuffer)
		delete m_TileInfoBuffer;
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
	GetMaterial()->SetTexParam(TEX_PARAM::TEX_0, m_TileAtlas);
		
	// Ÿ���� ���� ���� ����
	GetMaterial()->SetScalarParam(SCALAR_PARAM::INT_0, m_FaceX);
	GetMaterial()->SetScalarParam(SCALAR_PARAM::INT_1, m_FaceY);

	// ��Ʋ�� �̹������� Ÿ�� 1���� �ڸ��� ������(UV ����)
	GetMaterial()->SetScalarParam(SCALAR_PARAM::VEC2_0, m_vSliceSizeUV);

	// �� Ÿ�� ������ ����ȭ ���۷� �̵�
	m_TileInfoBuffer->SetData(m_vecTileInfo.data(), m_vecTileInfo.size());

	// Ÿ�� ����ȭ ���۸� t20 �� ���ε�
	m_TileInfoBuffer->UpdateData(20);

	// ���� ������Ʈ
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

	// ��Ʋ�� ���� Ÿ���� �� �� �ִ��� ���
	m_MaxCol = m_TileAtlas->GetWidth() / (UINT)m_vTilePixelSize.x;
	m_MaxRow = m_TileAtlas->GetHeight() / (UINT)m_vTilePixelSize.y;

	// Ÿ�� �ϳ��� ����� UV �������� ���
	m_vSliceSizeUV = Vec2(m_vTilePixelSize.x / m_TileAtlas->GetWidth()
		, m_vTilePixelSize.y / m_TileAtlas->GetHeight());
}

// _FaceX * _FaceY�� Ÿ���� ������ Set�ϴ� �Լ�
void HYTileMap::SetFace(UINT _FaceX, UINT _FaceY)
{
	m_FaceX = _FaceX;
	m_FaceY = _FaceY;

	// �� ������ ���缭 vector ������ �ø�
	vector<tTileInfo> vecTemp;
	m_vecTileInfo.swap(vecTemp);
	m_vecTileInfo.resize(_FaceX * _FaceY);

	// tTileInfo�� _FaceX * _FaceY��ŭ ����
	m_TileInfoBuffer->Create(sizeof(tTileInfo), _FaceX * _FaceY, SB_TYPE::READ_ONLY, true);
}
// _Row�� _Col���� ������ �̹����� �ε����� �����ϴ� �Լ�
void HYTileMap::SetTileIndex(UINT _Row, UINT _Col, UINT _ImgIdx)
{
	if (nullptr == m_TileAtlas)
		return;

	// ���� �迭 ���� ������ �ε���
	UINT idx = _Row * m_FaceX + _Col;

	// �������� Ÿ�� ��� �� ������ ���� �»���� �˾Ƴ�
	UINT iRow = _ImgIdx / m_MaxCol;
	UINT iCol = _ImgIdx % m_MaxCol;

	// �»�� ������ UV �������� ���(�ػ󵵷� �����°� UV �������� �Ϸ���)
	m_vecTileInfo[idx].vLeftTopUV = Vec2((iCol * m_vTilePixelSize.x) / m_TileAtlas->GetWidth()
		, (iRow * m_vTilePixelSize.y) / m_TileAtlas->GetHeight());

	m_vecTileInfo[idx].bRender = 1;
}

//void HYTileMap::SetTileIndex()
//{
//	if (nullptr == m_TileAtlas)
//		return;
//
//	// �迭 ���� ������ �ε���
//	UINT idx = UIROW * m_FaceX + UICOL;
//
//	// �������� Ÿ�� ����
//	UINT iRow = UIIDX / m_MaxCol;
//	UINT iCol = UIIDX % m_MaxCol;
//
//	// �»�� ������ UV �������� ���
//	m_vecTileInfo[idx].vLeftTopUV = Vec2((iCol * m_vTilePixelSize.x) / m_TileAtlas->GetWidth()
//		, (iRow * m_vTilePixelSize.y) / m_TileAtlas->GetHeight());
//
//	m_vecTileInfo[idx].bRender = 1;
//}
