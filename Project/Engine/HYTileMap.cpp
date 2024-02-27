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
	// Mesh와 Material은 생성될 때 고정해놓고 굳이 변경시키지 않음
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
	// (타일 개수 * 타일 사이즈) 로 랜더링 될 타일 사이즈 변경
	Vec3 vTileMapSize = Vec3(m_FaceX * m_vTileRenderSize.x, m_FaceY * m_vTileRenderSize.y, 1.f);
	Transform()->SetRelativeScale(vTileMapSize);
}

void HYTileMap::render()
{
	// 재질에 아틀라스 텍스쳐 전달.
	GetMaterial()->SetTexParam(TEX_PARAM::TEX_0, m_TileAtlas);
		
	// 타일의 가로 세로 개수
	GetMaterial()->SetScalarParam(SCALAR_PARAM::INT_0, m_FaceX);
	GetMaterial()->SetScalarParam(SCALAR_PARAM::INT_1, m_FaceY);

	// 아틀라스 이미지에서 타일 1개의 자르는 사이즈(UV 기준)
	GetMaterial()->SetScalarParam(SCALAR_PARAM::VEC2_0, m_vSliceSizeUV);

	// 각 타일 정보를 구조화 버퍼로 이동
	m_TileInfoBuffer->SetData(m_vecTileInfo.data(), m_vecTileInfo.size());

	// 타일 구조화 버퍼를 t20 에 바인딩
	m_TileInfoBuffer->UpdateData(20);

	// 재질 업데이트
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

	// 아틀라스 내에 타일이 몇 개 있는지 계산
	m_MaxCol = m_TileAtlas->GetWidth() / (UINT)m_vTilePixelSize.x;
	m_MaxRow = m_TileAtlas->GetHeight() / (UINT)m_vTilePixelSize.y;

	// 타일 하나의 사이즈를 UV 기준으로 계산
	m_vSliceSizeUV = Vec2(m_vTilePixelSize.x / m_TileAtlas->GetWidth()
		, m_vTilePixelSize.y / m_TileAtlas->GetHeight());
}

// _FaceX * _FaceY의 타일을 만들지 Set하는 함수
void HYTileMap::SetFace(UINT _FaceX, UINT _FaceY)
{
	m_FaceX = _FaceX;
	m_FaceY = _FaceY;

	// 면 개수에 맞춰서 vector 사이즈 늘림
	vector<tTileInfo> vecTemp;
	m_vecTileInfo.swap(vecTemp);
	m_vecTileInfo.resize(_FaceX * _FaceY);

	// tTileInfo가 _FaceX * _FaceY만큼 있음
	m_TileInfoBuffer->Create(sizeof(tTileInfo), _FaceX * _FaceY, SB_TYPE::READ_ONLY, true);
}
// _Row행 _Col열에 세팅할 이미지의 인덱스를 지정하는 함수
void HYTileMap::SetTileIndex(UINT _Row, UINT _Col, UINT _ImgIdx)
{
	if (nullptr == m_TileAtlas)
		return;

	// 벡터 배열 내에 접근할 인덱스
	UINT idx = _Row * m_FaceX + _Col;

	// 렌더링할 타일 헹과 열 정보를 통해 좌상단을 알아냄
	UINT iRow = _ImgIdx / m_MaxCol;
	UINT iCol = _ImgIdx % m_MaxCol;

	// 좌상단 정보를 UV 기준으로 계산(해상도로 나누는건 UV 기준으로 하려고)
	m_vecTileInfo[idx].vLeftTopUV = Vec2((iCol * m_vTilePixelSize.x) / m_TileAtlas->GetWidth()
		, (iRow * m_vTilePixelSize.y) / m_TileAtlas->GetHeight());

	m_vecTileInfo[idx].bRender = 1;
}

//void HYTileMap::SetTileIndex()
//{
//	if (nullptr == m_TileAtlas)
//		return;
//
//	// 배열 내에 접근할 인덱스
//	UINT idx = UIROW * m_FaceX + UICOL;
//
//	// 렌더링할 타일 정보
//	UINT iRow = UIIDX / m_MaxCol;
//	UINT iCol = UIIDX % m_MaxCol;
//
//	// 좌상단 정보를 UV 기준으로 계산
//	m_vecTileInfo[idx].vLeftTopUV = Vec2((iCol * m_vTilePixelSize.x) / m_TileAtlas->GetWidth()
//		, (iRow * m_vTilePixelSize.y) / m_TileAtlas->GetHeight());
//
//	m_vecTileInfo[idx].bRender = 1;
//}
