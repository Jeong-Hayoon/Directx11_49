#include "pch.h"
#include "TilemapUI.h"

#include <Engine/HYTilemap.h>
#include <Engine/HYKeyMgr.h>
#include <Engine/HYTransform.h>
#include <Engine/HYGameObject.h>
#include <Engine/HYLevelMgr.h>
#include <Engine/HYAssetMgr.h>
#include <Engine/HYLevel.h>


#include "HYImGuiMgr.h"
#include "ListUI.h"
#include "Inspector.h"
#include "TilemapEditor.h"

TilemapUI::TilemapUI()
	: ComponentUI("Tilemap", "##Tilemap", COMPONENT_TYPE::TILEMAP)
{
	SetSize(ImVec2(0.f, 240.f));	
	SetComopnentTitle("Tilemap");
}

TilemapUI::~TilemapUI()
{
}

void TilemapUI::render_update()
{
	ComponentUI::render_update();

	ImGui::Text("Tilemap Editor Open");	ImGui::SameLine();

	if (ImGui::Button("##Tilemap Editor", ImVec2(20, 20)))
	{
		TilemapEditor* pTilemapEditor = (TilemapEditor*)HYImGuiMgr::GetInst()->FindUI("##TilemapEditor");
		pTilemapEditor->Activate();
	}

	HYGameObject* pTarget = GetTargetObject();

	int FACEX[1] = {pTarget->TileMap()->GetFaceX()};
	ImGui::Text("FACE X	"); ImGui::SameLine(); ImGui::InputInt("##SetFaceX", FACEX);
	pTarget->TileMap()->SetFaceX(FACEX[0]);

	int FACEY[1] = { pTarget->TileMap()->GetFaceY() };
	ImGui::Text("FACE Y	"); ImGui::SameLine(); ImGui::InputInt("##SetFaceY", FACEY);
	pTarget->TileMap()->SetFaceY(FACEY[0]);

	/*int ROW[1] = { pTarget->TileMap()->GetUIROW() };
	ImGui::Text("ROW"); ImGui::SameLine(); ImGui::InputInt("##SetROW", ROW);
	pTarget->TileMap()->SetUIROW(ROW[0]);

	int COL[1] = { pTarget->TileMap()->GetUICOL() };
	ImGui::Text("COL"); ImGui::SameLine(); ImGui::InputInt("##SetCOL", COL);
	pTarget->TileMap()->SetUICOL(COL[0]);

	int IDX[1] = { pTarget->TileMap()->GetUIIDX() };
	ImGui::Text("INDEX"); ImGui::SameLine(); ImGui::InputInt("##SetINDEX", IDX);
	pTarget->TileMap()->SetUIIDX(IDX[0]);

	pTarget->TileMap()->SetTileIndex();*/
	//////////////////////////////////////////
	/*int ROW[1] = { pTarget->TileMap()->GetUIROW() };
	ImGui::Text("ROW"); ImGui::SameLine(); ImGui::InputInt("##SetROW", ROW);
	pTarget->TileMap()->SetUIROW(ROW[0]);

	int COL[1] = { pTarget->TileMap()->GetUICOL() };
	ImGui::Text("COL"); ImGui::SameLine(); ImGui::InputInt("##SetCOL", COL);
	pTarget->TileMap()->SetUICOL(COL[0]);

	int IDX[1] = { pTarget->TileMap()->GetUIIDX() };
	ImGui::Text("INDEX"); ImGui::SameLine(); ImGui::InputInt("##SetINDEX", IDX);
	pTarget->TileMap()->SetUIIDX(IDX[0]);

	ImGui::Text("TileSet");	ImGui::SameLine();

	if (ImGui::Button("##TileSet", ImVec2(20, 20)))
	{
		pTarget->TileMap()->SetTileIndex(ROW[0], COL[0], IDX[0]);
	}*/
	//////////////////////////////////////////

	int TileInfo[3] = { pTarget->TileMap()->GetUIROW(),  pTarget->TileMap()->GetUICOL(), pTarget->TileMap()->GetUIIDX()};
	ImGui::Text("ROW_COL_IDX"); ImGui::SameLine(); ImGui::InputInt3("##TileInfo_1", TileInfo);
	pTarget->TileMap()->SetTileIndex(TileInfo[0], TileInfo[1], TileInfo[2]);

	if (KEY_TAP(KEY::LBTN) && GetFocus())
	{
		Vector2 mousePos = HYKeyMgr::GetInst()->GetMousePos();

		// 마우스 pos값 반올림
		float roundmousePosX = round(mousePos.x);
		float roundmousePosY = round(mousePos.y);

		// 마우스 커서의 위치를 타일의 인덱스로 바꿔주는 작업
		int idxX = (int)((roundmousePosX) / (TILE_WIDTH));
		int idxY = (int)((roundmousePosY) / (TILE_HEIGHT));

		Vector2 offset = Vector2(0, 0);

		HYGameObject* tile = new HYGameObject();

		tile->AddComponent(new HYTransform);
		tile->AddComponent(new HYTileMap);
		//Ptr<HYTexture> pTileAtlas = HYAssetMgr::GetInst()->Load<HYTexture>(L"TileAtlasTex", L"texture\\TILE.bmp");
		//// 타일 텍스처와 타일 하나 사이즈를 세팅해줌
		//tile->TileMap()->SetTileAtlas(pTileAtlas, Vec2(64.f, 64.f));
		tile->Transform()->SetRelativePos(Vector3((idxX * (TILE_WIDTH)+offset.x
			), -(idxY * (TILE_HEIGHT)+offset.y), 300.f));

		tile->TileMap()->SetTileIndex((UINT)idxX, (UINT)idxY, 8);

		HYLevel* pCurLevel = HYLevelMgr::GetInst()->GetCurrentLevel();
		pCurLevel->AddObject(tile, L"Tile", false);


	}

}