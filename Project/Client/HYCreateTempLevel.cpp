#include "pch.h"
#include "HYCreateTempLevel.h"

#include <Engine/HYCollisionMgr.h>
				 
#include <Engine/HYLevelMgr.h>
#include <Engine/HYLevel.h>
#include <Engine/HYLayer.h>
#include <Engine/HYGameObject.h>
#include <Engine/components.h>

#include <Scripts/HYPlayerScript.h>
#include <Scripts/HYBackgroundScript.h>
#include <Scripts/HYMonsterScript.h>

#include <Engine/HYMesh.h>
#include <Engine/HYGraphicsShader.h>
#include <Engine/HYTexture.h>
#include <Engine/HYSetColorShader.h>

#include "HYLevelSaveLoad.h"

#include <Scripts/HYMissileScript.h>

#include <Engine/HYAssetMgr.h>
#include <Engine/HYPrefab.h>
#include <Engine/HYFSM.h>

#include "HYIdleState.h"
#include "HYTraceState.h"

void HYCreateTempLevel::Init()
{
	// Prefab Test(위치는 Spawn 하는 오브젝트가 설정)
	// Missile Prefab 생성
	//HYGameObject* pObj = nullptr;
	//
	//pObj = new HYGameObject;
	//pObj->SetName(L"Missile");
	//pObj->AddComponent(new HYTransform);
	//pObj->AddComponent(new HYMeshRender);
	//pObj->AddComponent(new HYMissileScript);
	//
	//pObj->Transform()->SetRelativeScale(Vec3(50.f, 50.f, 1.f));
	//
	//pObj->MeshRender()->SetMesh(HYAssetMgr::GetInst()->FindAsset<HYMesh>(L"RectMesh"));
	//pObj->MeshRender()->SetMaterial(HYAssetMgr::GetInst()->FindAsset<HYMaterial>(L"Std2DMtrl"));
	//
	//Ptr<HYPrefab> pMissilePrefab = new HYPrefab(pObj);
	//HYAssetMgr::GetInst()->AddAsset<HYPrefab>(L"MissilePrefab", pMissilePrefab.Get());
	//
	//pMissilePrefab->Save(L"prefab\\missile.pref");
	
	// 임시 FSM 객체 에셋 하나 생성하기
	Ptr<HYFSM>	pFSM = new HYFSM(true);

	pFSM->AddState(L"IdleState", new HYIdleState);
	pFSM->AddState(L"TraceState", new HYTraceState);

	HYAssetMgr::GetInst()->AddAsset<HYFSM>(L"NormalMonsterFSM", pFSM.Get());
}

void HYCreateTempLevel::CreateTempLevel()
{
	// 재질의 파라미터 미리 세팅
	//Ptr<HYMaterial> pBackgroudMtrl = HYAssetMgr::GetInst()->FindAsset<HYMaterial>(L"BackgroundMtrl");
	//Ptr<HYMaterial> pStd2DMtrl = HYAssetMgr::GetInst()->FindAsset<HYMaterial>(L"Std2DMtrl");
	//
	//pBackgroudMtrl->SetTexParam(TEX_PARAM::TEX_0, HYAssetMgr::GetInst()->Load<HYTexture>(L"texture\\Background.png", L"texture\\Background.png"));
	//pStd2DMtrl->SetTexParam(TEX_PARAM::TEX_0, HYAssetMgr::GetInst()->Load<HYTexture>(L"texture\\player.bmp", L"texture\\player.bmp"));

	// Save된 Level Load Test
	//HYLevel* pLevel = HYLevelSaveLoad::LoadLevel(L"level\\temp.lv");
	//HYLevelMgr::GetInst()->ChangeLevel(pLevel, LEVEL_STATE::PLAY);
	//return;

	HYLevel* pTempLevel = new HYLevel;

	// 재질 로드 테스트
	//HYAssetMgr::GetInst()->Load<HYMaterial>(L"material\\testmtrl.mtrl", L"material\\testmtrl.mtrl");

	// 초기 레벨 구성하기
	pTempLevel->GetLayer(0)->SetName(L"Default");
	pTempLevel->GetLayer(1)->SetName(L"Background");
	pTempLevel->GetLayer(2)->SetName(L"Tile");
	pTempLevel->GetLayer(3)->SetName(L"Player");
	pTempLevel->GetLayer(4)->SetName(L"Monster");
	pTempLevel->GetLayer(31)->SetName(L"UI");
	pTempLevel->GetLayer(5)->SetName(L"Light");
	pTempLevel->GetLayer(6)->SetName(L"Tile");

	// ComputeShader 테스트
	// 사용할 텍스쳐 생성
	Ptr<HYTexture> pTestTex = HYAssetMgr::GetInst()->CreateTexture(L"TestTex"
		, 1024, 1024, DXGI_FORMAT_R8G8B8A8_UNORM
		, D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS);

	Ptr<HYSetColorShader> pCS = (HYSetColorShader*)HYAssetMgr::GetInst()->FindAsset<HYComputeShader>(L"SetColorShader").Get();
	pCS->SetColor(Vec3(1.f, 0.f, 0.f));
	pCS->SetTargetTexture(pTestTex);
	pCS->Execute();

	// 텍스처에서 픽셀 가져오기
	//tPixel* pPixel = pTestTex->GetPixels();
	//tPixel pixel = pPixel[0];

	//tPixel* pPixel = pTestTex->GetPixels();
	// 두번째 줄 여섯번째 픽셀에 접근하고 싶은 경우
	//tPixel pixel = pPixel[pTestTex->GetWidth() * 1 + 5];

	// Main Camera Object 생성 -> 카메라 기능 수행
	HYGameObject* pCamObj = new HYGameObject;
	pCamObj->SetName(L"MainCamera");
	pCamObj->AddComponent(new HYTransform);
	pCamObj->AddComponent(new HYCamera);

	pCamObj->Transform()->SetRelativePos(Vec3(0.5f, 0.f, 0.f));
	pCamObj->Transform()->SetRelativeRotation(Vec3(0.f, 0.f, 0.f));

	pCamObj->Camera()->SetCameraPriority(0);
	pCamObj->Camera()->LayerCheckAll();
	// Main Camera는 UI를 안보게 세팅
	pCamObj->Camera()->LayerCheck(31, false);

	pTempLevel->AddObject(pCamObj, 0);

	// Sub Camera(UI 카메라 생성)
	pCamObj = new HYGameObject;
	pCamObj->SetName(L"UICamera");
	pCamObj->AddComponent(new HYTransform);
	pCamObj->AddComponent(new HYCamera);

	pCamObj->Transform()->SetRelativePos(Vec3(0.5f, 0.f, 0.f));
	pCamObj->Transform()->SetRelativeRotation(Vec3(0.f, 0.f, 0.f));

	pCamObj->Camera()->SetCameraPriority(1);
	// Sub Camera는 원점에서 UI를 바라보도록
	pCamObj->Camera()->LayerCheck(31, true);

	pTempLevel->AddObject(pCamObj, 0);

	// 전역 광원 추가
	HYGameObject* pLight = new HYGameObject;
	pLight->SetName(L"Directional Light");
	pLight->AddComponent(new HYTransform);
	//pLight->AddComponent(new HYMeshRender);
	pLight->AddComponent(new HYLight2D);

	pLight->Light2D()->SetLightType(LIGHT_TYPE::DIRECTIONAL);
	pLight->Light2D()->SetAmbient(Vec3(1.f, 1.f, 1.f));

	pTempLevel->AddObject(pLight, L"Light");

	HYGameObject* pObj = nullptr;

	// 스포트 라이트
	//pLight = new HYGameObject;
	//pLight->SetName(L"Spot Light");

	//pLight->AddComponent(new HYTransform);
	//pLight->AddComponent(new HYMeshRender);
	//pLight->AddComponent(new HYLight2D);
	//pLight->AddComponent(new HYSpotLightScript);

	//pLight->Light2D()->SetLightType(LIGHT_TYPE::SPOT);
	//pLight->Light2D()->SetLightColor(Vec3(1.f, 1.f, 1.f));
	//pLight->Light2D()->SetRadius(300.f);
	//pLight->Light2D()->SetAngle(100.f);

	////pLight->Transform()->SetRelativePos(Vec3(-200.f, 0.f, 200.f));
	//pTempLevel->AddObject(pLight, L"Light");

	// 두번째 광원 추가
	//pLight = new HYGameObject;
	//pLight->AddComponent(new HYTransform);
	//pLight->AddComponent(new HYMeshRender);
	//pLight->AddComponent(new HYLight2D);

	//pLight->Light2D()->SetLightType(LIGHT_TYPE::POINT);
	//pLight->Light2D()->SetLightColor(Vec3(1.f, 1.f, 1.f));
	//pLight->Light2D()->SetRadius(300.f);

	////pLight->Transform()->SetRelativePos(Vec3(100.f, 100.f, 100.f));
	//pTempLevel->AddObject(pLight, L"Light");

	pObj = nullptr;

	// Backgruond Object 생성
	pObj = new HYGameObject;
	pObj->SetName(L"Background");

	pObj->AddComponent(new HYTransform);
	pObj->AddComponent(new HYMeshRender);
	pObj->AddComponent(new HYBackgroundScript);

	pObj->Transform()->SetRelativePos(Vec3(0.f, 0.f, 600.f));
	pObj->Transform()->SetRelativeScale(Vec3(1600.f, 800.f, 1.f));

	pObj->MeshRender()->SetMesh(HYAssetMgr::GetInst()->FindAsset<HYMesh>(L"RectMesh"));
	// 다른 재질을 사용해야 다른 텍스처를 사용할 수 있음
	pObj->MeshRender()->SetMaterial(HYAssetMgr::GetInst()->FindAsset<HYMaterial>(L"BackgroundMtrl"));

	Ptr<HYTexture> pTex = HYAssetMgr::GetInst()->Load<HYTexture>(L"texture\\Background.png", L"texture\\Background.png");
	pObj->MeshRender()->GetMaterial()->SetTexParam(TEX_PARAM::TEX_0, pTex);
	//pObj->MeshRender()->GetMaterial()->SetTexParam(TEX_PARAM::TEX_0, pTestTex); -> ComputeShdaer

	pTempLevel->AddObject(pObj, L"Background", false);

	// TileMap Object
	//pObj = new HYGameObject;
	//pObj->SetName(L"TileMap");

	//pObj->AddComponent(new HYTransform);
	//pObj->AddComponent(new HYTileMap);
	//pObj->AddComponent(new HYCollider2D);

	//pObj->Collider2D()->SetAbsolute(true);

	//pObj->Transform()->SetRelativePos(Vec3(0.f, 0.f, 300.f));

	//Ptr<HYTexture> pTileAtlas = HYAssetMgr::GetInst()->Load<HYTexture>(L"TileAtlasTex", L"texture\\TILE.bmp");
	//// 타일 텍스처와 타일 하나 사이즈를 세팅해줌
	//pObj->TileMap()->SetTileAtlas(pTileAtlas, Vec2(64.f, 64.f));
	//pObj->TileMap()->SetFace(6, 6);
	//for (int i = 0; i < 6; ++i)
	//{
	//	for (int j = 0; j < 6; ++j)
	//	{
	//		pObj->TileMap()->SetTileIndex(i, j, i * 6 + j);
	//	}
	//}

	//pTempLevel->AddObject(pObj, L"Tile", false);

	// Player Object 생성
	pObj = new HYGameObject;
	pObj->SetName(L"Player");

	pObj->AddComponent(new HYTransform);
	pObj->AddComponent(new HYMeshRender);
	pObj->AddComponent(new HYCollider2D);
	pObj->AddComponent(new HYAnimator2D);
	pObj->AddComponent(new HYPlayerScript);

	pObj->Transform()->SetRelativePos(Vec3(0.f, 0.f, 500.f));
	pObj->Transform()->SetRelativeScale(Vec3(200.f, 360.f, 1.f));

	pObj->Collider2D()->SetAbsolute(true);
	pObj->Collider2D()->SetOffsetScale(Vec2(100.f, 100.f));
	pObj->Collider2D()->SetOffsetPos(Vec2(0.f, 0.f));

	pObj->MeshRender()->SetMesh(HYAssetMgr::GetInst()->FindAsset<HYMesh>(L"RectMesh"));
	pObj->MeshRender()->SetMaterial(HYAssetMgr::GetInst()->FindAsset<HYMaterial>(L"Std2DMtrl"));
	pObj->MeshRender()->GetMaterial()->SetTexParam(TEX_PARAM::TEX_0, HYAssetMgr::GetInst()->Load<HYTexture>(L"texture\\Character.png", L"texture\\Character.png"));

	//pObj->AddComponent(new HYLight2D);

	//pObj->Light2D()->SetLightType(LIGHT_TYPE::POINT);
	//pObj->Light2D()->SetLightColor(Vec3(1.f, 0.3f, 0.3f));
	//pObj->Light2D()->SetRadius(300.f);

	pTempLevel->AddObject(pObj, L"Player", false);

	// Player Clone
	//pObj = pObj->Clone();
	//pObj->Transform()->SetRelativePos(Vec3(-500.f, 0.f, 500.f));
	// m_CurLevel->AddObject(pObj, L"Player", false);

	// Particle Object Clone
	//HYGameObject* pParticleObj = new HYGameObject;
	//pParticleObj->SetName(L"Particle");
	//
	//pParticleObj->AddComponent(new HYTransform);
	//pParticleObj->AddComponent(new HYParticleSystem);
	//
	//pParticleObj->Transform()->SetRelativePos(Vec3(0.f, 0.f, 200.f));
	//pTempLevel->AddObject(pParticleObj, L"Default", false);
	//
	//pParticleObj = pParticleObj->Clone();
	//pParticleObj->Transform()->SetRelativePos(Vec3(500.f, 0.f, 200.f));
	//pTempLevel->AddObject(pParticleObj, L"Default", false);

	// 자식이 있는 오브젝트의 복제 Test
	//pObj->AddChild(pParticleObj);
	//pTempLevel->AddObject(pObj, L"Default", false);
	//
	//HYGameObject* pCloneObj = pObj->Clone();
	//pTempLevel->AddObject(pCloneObj, L"Default", false);

	// Monster Object 생성
	//pObj = new HYGameObject;
	//pObj->SetName(L"Monster");

	//pObj->AddComponent(new HYTransform);
	//pObj->AddComponent(new HYMeshRender);
	//pObj->AddComponent(new HYCollider2D);

	//pObj->Transform()->SetRelativePos(Vec3(500.f, 0.f, 500.f));
	//pObj->Transform()->SetRelativeScale(Vec3(200.f, 200.f, 1.f));

	//pObj->Collider2D()->SetAbsolute(true);
	//pObj->Collider2D()->SetOffsetScale(Vec2(50.f, 50.f));
	//pObj->Collider2D()->SetOffsetPos(Vec2(100.f, 0.f));

	//pObj->MeshRender()->SetMesh(HYAssetMgr::GetInst()->FindAsset<HYMesh>(L"RectMesh"));
	//pObj->MeshRender()->SetMaterial(HYAssetMgr::GetInst()->FindAsset<HYMaterial>(L"Std2DMtrl"));
	//pObj->MeshRender()->GetMaterial()->SetScalarParam(SCALAR_PARAM::FLOAT_0, 0.f);

	//pTempLevel->AddObject(pObj, L"Monster", false);

	// Monster Object 생성(MonsterScript Test)
	pObj = new HYGameObject;
	pObj->SetName(L"Monster");

	pObj->AddComponent(new HYTransform);
	pObj->AddComponent(new HYMeshRender);
	pObj->AddComponent(new HYCollider2D);
	pObj->AddComponent(new HYStateMachine);
	pObj->AddComponent(new HYMonsterScript);

	pObj->Transform()->SetRelativePos(Vec3(500.f, 0.f, 500.f));
	pObj->Transform()->SetRelativeScale(Vec3(200.f, 200.f, 1.f));

	pObj->Collider2D()->SetAbsolute(true);
	pObj->Collider2D()->SetOffsetScale(Vec2(120.f, 120.f));
	pObj->Collider2D()->SetOffsetPos(Vec2(0.f, 0.f));

	pObj->MeshRender()->SetMesh(HYAssetMgr::GetInst()->FindAsset<HYMesh>(L"RectMesh"));
	pObj->MeshRender()->SetMaterial(HYAssetMgr::GetInst()->FindAsset<HYMaterial>(L"Std2DMtrl"));
	pObj->MeshRender()->GetMaterial()->SetTexParam(TEX_PARAM::TEX_0, HYAssetMgr::GetInst()->Load<HYTexture>(L"texture\\Character.png", L"texture\\Character.png"));

	pObj->StateMachine()->SetFSM(HYAssetMgr::GetInst()->FindAsset<HYFSM>(L"NormalMonsterFSM"));

	pTempLevel->AddObject(pObj, L"Monster", false);

	//pObj = new HYGameObject;
	//pObj->SetName(L"UI");

	//pObj->AddComponent(new HYTransform);
	//pObj->AddComponent(new HYMeshRender);

	//pObj->Transform()->SetRelativePos(Vec3(-590, 310.f, 500.f));
	//pObj->Transform()->SetRelativeScale(Vec3(50.f, 50.f, 1.f));

	//pObj->MeshRender()->SetMesh(HYAssetMgr::GetInst()->FindAsset<HYMesh>(L"RectMesh"));
	//pObj->MeshRender()->SetMaterial(HYAssetMgr::GetInst()->FindAsset<HYMaterial>(L"Std2DMtrl"));

	//pTempLevel->AddObject(pObj, L"UI", false);

	// PostProcess 오브젝트 추가
	//pObj = new HYGameObject;
	//pObj->SetName(L"GrayFilter");

	//// 트랜스폼 컴포넌트를 넣어주기는 하지만 어차피 전체 영역을 덮을거라 위치, 크기 다 상관없음
	//pObj->AddComponent(new HYTransform);
	//pObj->AddComponent(new HYMeshRender);

	//pObj->MeshRender()->SetMesh(HYAssetMgr::GetInst()->FindAsset<HYMesh>(L"RectMesh"));
	//pObj->MeshRender()->SetMaterial(HYAssetMgr::GetInst()->FindAsset<HYMaterial>(L"GrayFilterMtrl"));

	//pTempLevel->AddObject(pObj, L"Default", false);

	// Distortion 효과 추가
	/*pObj = new HYGameObject;
	pObj->SetName(L"Distortion Object");

	pObj->AddComponent(new HYTransform);
	pObj->AddComponent(new HYMeshRender);

	pObj->Transform()->SetRelativePos(Vec3(0.f, 0.f, 200.f));
	pObj->Transform()->SetRelativeScale(Vec3(300.f, 300.f, 1.f));

	pObj->MeshRender()->SetMesh(HYAssetMgr::GetInst()->FindAsset<HYMesh>(L"RectMesh"));
	pObj->MeshRender()->SetMaterial(HYAssetMgr::GetInst()->FindAsset<HYMaterial>(L"DistortionMtrl"));
	pObj->MeshRender()->GetMaterial()->SetTexParam(TEX_PARAM::TEX_0, HYAssetMgr::GetInst()->Load<HYTexture>(L"texture\\noise\\noise_03.jpg", L"texture\\noise\\noise_03.jpg"));

	pTempLevel->AddObject(pObj, L"Default", false);*/

	// Wave 효과 추가
	/*pObj = new HYGameObject;
	pObj->SetName(L"Wave Object");

	pObj->AddComponent(new HYTransform);
	pObj->AddComponent(new HYMeshRender);

	pObj->Transform()->SetRelativePos(Vec3(0.f, 0.f, 200.f));
	pObj->Transform()->SetRelativeScale(Vec3(300.f, 300.f, 1.f));

	pObj->MeshRender()->SetMesh(HYAssetMgr::GetInst()->FindAsset<HYMesh>(L"CircleMesh"));
	pObj->MeshRender()->SetMaterial(HYAssetMgr::GetInst()->FindAsset<HYMaterial>(L"WaveFilterMtrl"));
	pObj->MeshRender()->GetMaterial()->SetTexParam(TEX_PARAM::TEX_0, HYAssetMgr::GetInst()->Load<HYTexture>(L"texture\\ChaosRiftOpen6.png", L"texture\\ChaosRiftOpen6.png"));

	pTempLevel->AddObject(pObj, L"Default", false);*/

	// 외곽선 효과
	/*pObj = new HYGameObject;
	pObj->SetName(L"OutLine Object");

	pObj->AddComponent(new HYTransform);
	pObj->AddComponent(new HYMeshRender);

	pObj->Transform()->SetRelativePos(Vec3(0.f, 0.f, 200.f));
	pObj->Transform()->SetRelativeScale(Vec3(800.f, 800.f, 1.f));

	pObj->MeshRender()->SetMesh(HYAssetMgr::GetInst()->FindAsset<HYMesh>(L"RectMesh"));
	pObj->MeshRender()->SetMaterial(HYAssetMgr::GetInst()->FindAsset<HYMaterial>(L"OutLineMtrl"));
	pObj->MeshRender()->GetMaterial()->SetTexParam(TEX_PARAM::TEX_0, HYAssetMgr::GetInst()->Load<HYTexture>(L"texture\\noise\\noise_03.jpg", L"texture\\noise\\noise_03.jpg"));

	pTempLevel->AddObject(pObj, L"Default", false);*/

	// Particle Object 생성
	//pObj = new HYGameObject;
	//pObj->SetName(L"Particle");

	//pObj->AddComponent(new HYTransform);
	//pObj->AddComponent(new HYParticleSystem);

	//// 어차피 오브젝트의 위치는 의미가 없음 - 각 파티클의 위치 정보로 랜더링될거기 때문에
	//pObj->Transform()->SetRelativePos(Vec3(0.f, 0.f, 200.f));

	//pTempLevel->AddObject(pObj, L"Default", false);

	// UI Test // 
	// Particle Object
	//pParticleObj = new HYGameObject;
	//pParticleObj->SetName(L"Particle");
	
	//pParticleObj->AddComponent(new HYTransform);
	//pParticleObj->AddComponent(new HYParticleSystem);
	//
	//pParticleObj->Transform()->SetRelativePos(Vec3(0.f, 0.f, 200.f));

	//pTempLevel->AddObject(pObj, L"Default", false);

	//pObj->AddChild(pParticleObj);
	//pTempLevel->AddObject(pObj, L"Default", false);

	// Level Clone Test 
	//HYLevel* pNewLevel = pTempLevel->Clone();
	//delete pTempLevel;
	//pTempLevel = pNewLevel;

	// Level 시작(Play)
	// pTempLevel->begin();

	// 처음의 한 프레임이 붕 떠버리기 때문에 일단 TaskMgr로 처리하지 않고 직접 호출
	// pTempLevel->ChangeState(LEVEL_STATE::STOP);

	// 충돌 설정
	HYCollisionMgr::GetInst()->LayerCheck(3, 4);
	HYCollisionMgr::GetInst()->LayerCheck(4, 4);

	// 현재 레벨을 pTempLevel로 하고 레벨의 상태는 STOP으로 해달라고 Level Change 요청
	HYLevelMgr::GetInst()->ChangeLevel(pTempLevel, LEVEL_STATE::STOP);

	// Level Save Test
	HYLevelSaveLoad::SaveLevel(pTempLevel, L"level\\temp.lv");
}



