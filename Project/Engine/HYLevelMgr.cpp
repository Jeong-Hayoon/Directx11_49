#include "pch.h"
#include "HYLevelMgr.h"
		  
#include "HYDevice.h"
#include "HYAssetMgr.h"
		  
#include "HYLevel.h"
#include "HYLayer.h"
#include "HYGameObject.h"
#include "components.h"
#include "HYPlayerScript.h"
#include "HYCameraMoveScript.h"
#include "HYBackgroundScript.h"
#include "HYSpotLightScript.h"
		  
#include "HYMesh.h"
#include "HYGraphicsShader.h"
#include "HYTexture.h"
#include "HYCollisionMgr.h"

HYLevelMgr::HYLevelMgr()
	: m_CurLevel(nullptr)
{
}

HYLevelMgr::~HYLevelMgr()
{
	if (nullptr != m_CurLevel)
		delete m_CurLevel;
}

void HYLevelMgr::init()
{
	// �ʱ� ���� �����ϱ�
	m_CurLevel = new HYLevel;
	m_CurLevel->GetLayer(0)->SetName(L"Default");
	m_CurLevel->GetLayer(1)->SetName(L"Background");
	m_CurLevel->GetLayer(2)->SetName(L"Tile");
	m_CurLevel->GetLayer(3)->SetName(L"Player");
	m_CurLevel->GetLayer(4)->SetName(L"Monster");
	m_CurLevel->GetLayer(31)->SetName(L"UI");
	m_CurLevel->GetLayer(5)->SetName(L"Light");
	m_CurLevel->GetLayer(6)->SetName(L"Tile");

	// �浹 ����
	HYCollisionMgr::GetInst()->LayerCheck(L"Player", L"Monster");
	HYCollisionMgr::GetInst()->LayerCheck(L"Monster", L"Monster");

	// Main Camera Object ���� -> ī�޶� ��� ����
	HYGameObject* pCamObj = new HYGameObject;
	pCamObj->SetName(L"MainCamera");
	pCamObj->AddComponent(new HYTransform);
	pCamObj->AddComponent(new HYCamera);
	pCamObj->AddComponent(new HYCameraMoveScript);

	pCamObj->Transform()->SetRelativePos(Vec3(0.5f, 0.f, 0.f));
	pCamObj->Transform()->SetRelativeRotation(Vec3(0.f, 0.f, 0.f));

	pCamObj->Camera()->SetCameraPriority(0);
	pCamObj->Camera()->LayerCheckAll();
	// Main Camera�� UI�� �Ⱥ��� ����
	pCamObj->Camera()->LayerCheck(L"UI", false);

	m_CurLevel->AddObject(pCamObj, 0);

	// Sub Camera
	pCamObj = new HYGameObject;
	pCamObj->SetName(L"UICamera");
	pCamObj->AddComponent(new HYTransform);
	pCamObj->AddComponent(new HYCamera);

	pCamObj->Transform()->SetRelativePos(Vec3(0.5f, 0.f, 0.f));
	pCamObj->Transform()->SetRelativeRotation(Vec3(0.f, 0.f, 0.f));

	pCamObj->Camera()->SetCameraPriority(1);
	// Sub Camera�� �������� UI�� �ٶ󺸵���
	pCamObj->Camera()->LayerCheck(L"UI", true);

	m_CurLevel->AddObject(pCamObj, 0);

	// ���� ���� �߰�
	HYGameObject* pLight = new HYGameObject;
	pLight->SetName(L"Directional Light");
	pLight->AddComponent(new HYTransform);
	pLight->AddComponent(new HYMeshRender);
	pLight->AddComponent(new HYLight2D);

	pLight->Light2D()->SetLightType(LIGHT_TYPE::DIRECTIONAL);
	pLight->Light2D()->SetAmbient(Vec3(1.f, 1.f, 1.f));

	m_CurLevel->AddObject(pLight, L"Light");

	HYGameObject* pObj = nullptr;

	// ����Ʈ ����Ʈ
	/*HYGameObject* pLight = new HYGameObject;
	pLight->SetName(L"Directional Light");

	pLight->AddComponent(new HYTransform);
	pLight->AddComponent(new HYMeshRender);
	pLight->AddComponent(new HYLight2D);
	pLight->AddComponent(new HYSpotLightScript);

	pLight->Light2D()->SetLightType(LIGHT_TYPE::SPOT);
	pLight->Light2D()->SetLightColor(Vec3(1.f, 1.f, 1.f));
	pLight->Light2D()->SetRadius(300.f);
	pLight->Light2D()->SetAngle(100.f);

	pLight->Transform()->SetRelativePos(Vec3(-200.f, 0.f, 200.f));
	m_CurLevel->AddObject(pLight, L"Light");*/

	// �ι�° ���� �߰�
	pLight = new HYGameObject;
	pLight->AddComponent(new HYTransform);
	pLight->AddComponent(new HYMeshRender);
	pLight->AddComponent(new HYLight2D);

	pLight->Light2D()->SetLightType(LIGHT_TYPE::POINT);
	pLight->Light2D()->SetLightColor(Vec3(0.3f, 0.3f, 1.f));
	pLight->Light2D()->SetRadius(300.f);

	pLight->Transform()->SetRelativePos(Vec3(200.f, 0.f, 200.f));
	m_CurLevel->AddObject(pLight, L"Light");

	pObj = nullptr;

	// Backgruond Object ����
	pObj = new HYGameObject;
	pObj->SetName(L"Background");

	pObj->AddComponent(new HYTransform);
	pObj->AddComponent(new HYMeshRender);
	pObj->AddComponent(new HYBackgroundScript);

	pObj->Transform()->SetRelativePos(Vec3(0.f, 0.f, 600.f));
	pObj->Transform()->SetRelativeScale(Vec3(1300.f, 800.f, 1.f));

	pObj->MeshRender()->SetMesh(HYAssetMgr::GetInst()->FindAsset<HYMesh>(L"RectMesh"));
	// �ٸ� ������ ����ؾ� �ٸ� �ؽ�ó�� ����� �� ����
	pObj->MeshRender()->SetMaterial(HYAssetMgr::GetInst()->FindAsset<HYMaterial>(L"BackgroundMtrl"));

	Ptr<HYTexture> pTex = HYAssetMgr::GetInst()->Load<HYTexture>(L"BackgroundTex", L"texture\\Background.png");
	pObj->MeshRender()->GetMaterial()->SetTexParam(TEX_0, pTex);

	m_CurLevel->AddObject(pObj, L"Background", false);

	// TileMap Object
	//pObj = new HYGameObject;
	//pObj->SetName(L"TileMap");

	//pObj->AddComponent(new HYTransform);
	//pObj->AddComponent(new HYTileMap);

	//pObj->Transform()->SetRelativePos(Vec3(0.f, 0.f, 300.f));

	//Ptr<HYTexture> pTileAtlas = HYAssetMgr::GetInst()->Load<HYTexture>(L"TileAtlasTex", L"texture\\TILE.bmp");
	//// Ÿ�� �ؽ�ó�� Ÿ�� �ϳ� ����� ��������
	//pObj->TileMap()->SetTileAtlas(pTileAtlas, Vec2(64.f, 64.f));

	//m_CurLevel->AddObject(pObj, L"Tile", false);

	// Player Object ����
	pObj = new HYGameObject;
	pObj->SetName(L"Player");

	pObj->AddComponent(new HYTransform);
	pObj->AddComponent(new HYMeshRender);
	pObj->AddComponent(new HYCollider2D);
	pObj->AddComponent(new HYAnimator2D);
	pObj->AddComponent(new HYPlayerScript);

	pObj->Transform()->SetRelativePos(Vec3(0.f, 0.f, 500.f));
	pObj->Transform()->SetRelativeScale(Vec3(500.f, 500.f, 1.f));


	pObj->Collider2D()->SetAbsolute(true);
	pObj->Collider2D()->SetOffsetScale(Vec2(50.f, 50.f));
	pObj->Collider2D()->SetOffsetPos(Vec2(100.f, 0.f));

	pObj->MeshRender()->SetMesh(HYAssetMgr::GetInst()->FindAsset<HYMesh>(L"RectMesh"));
	pObj->MeshRender()->SetMaterial(HYAssetMgr::GetInst()->FindAsset<HYMaterial>(L"Std2DMtrl"));
	pObj->MeshRender()->GetMaterial()->SetTexParam(TEX_0, HYAssetMgr::GetInst()->Load<HYTexture>(L"PlayerTexture", L"texture\\Character.png"));

	pObj->AddComponent(new HYLight2D);

	pObj->Light2D()->SetLightType(LIGHT_TYPE::POINT);
	pObj->Light2D()->SetLightColor(Vec3(1.f, 0.3f, 0.3f));
	pObj->Light2D()->SetRadius(300.f);

	m_CurLevel->AddObject(pObj, L"Player", false);

	// Monster Object ����
	pObj = new HYGameObject;
	pObj->SetName(L"Monster");

	pObj->AddComponent(new HYTransform);
	pObj->AddComponent(new HYMeshRender);
	pObj->AddComponent(new HYCollider2D);

	pObj->Transform()->SetRelativePos(Vec3(500.f, 0.f, 500.f));
	pObj->Transform()->SetRelativeScale(Vec3(200.f, 200.f, 1.f));

	pObj->Collider2D()->SetAbsolute(true);
	pObj->Collider2D()->SetOffsetScale(Vec2(50.f, 50.f));
	pObj->Collider2D()->SetOffsetPos(Vec2(100.f, 0.f));

	pObj->MeshRender()->SetMesh(HYAssetMgr::GetInst()->FindAsset<HYMesh>(L"RectMesh"));
	pObj->MeshRender()->SetMaterial(HYAssetMgr::GetInst()->FindAsset<HYMaterial>(L"Std2DMtrl"));
	pObj->MeshRender()->GetMaterial()->SetScalarParam(FLOAT_0, 0.f);

	m_CurLevel->AddObject(pObj, L"Monster", false);

	pObj = new HYGameObject;
	pObj->SetName(L"UI");

	pObj->AddComponent(new HYTransform);
	pObj->AddComponent(new HYMeshRender);

	pObj->Transform()->SetRelativePos(Vec3(-590, 310.f, 500.f));
	pObj->Transform()->SetRelativeScale(Vec3(50.f, 50.f, 1.f));

	pObj->MeshRender()->SetMesh(HYAssetMgr::GetInst()->FindAsset<HYMesh>(L"RectMesh"));
	pObj->MeshRender()->SetMaterial(HYAssetMgr::GetInst()->FindAsset<HYMaterial>(L"Std2DMtrl"));

	m_CurLevel->AddObject(pObj, L"UI", false);

	// PostProcess ������Ʈ �߰�
	//pObj = new HYGameObject;
	//pObj->SetName(L"GrayFilter");

	//// Ʈ������ ������Ʈ�� �־��ֱ�� ������ ������ ��ü ������ �����Ŷ� ��ġ, ũ�� �� �������
	//pObj->AddComponent(new HYTransform);
	//pObj->AddComponent(new HYMeshRender);

	//pObj->MeshRender()->SetMesh(HYAssetMgr::GetInst()->FindAsset<HYMesh>(L"RectMesh"));
	//pObj->MeshRender()->SetMaterial(HYAssetMgr::GetInst()->FindAsset<HYMaterial>(L"GrayFilterMtrl"));

	//m_CurLevel->AddObject(pObj, L"Default", false);

	// Distortion ȿ�� �߰�
	/*pObj = new HYGameObject;
	pObj->SetName(L"Distortion Object");

	pObj->AddComponent(new HYTransform);
	pObj->AddComponent(new HYMeshRender);

	pObj->Transform()->SetRelativePos(Vec3(0.f, 0.f, 200.f));
	pObj->Transform()->SetRelativeScale(Vec3(300.f, 300.f, 1.f));

	pObj->MeshRender()->SetMesh(HYAssetMgr::GetInst()->FindAsset<HYMesh>(L"RectMesh"));
	pObj->MeshRender()->SetMaterial(HYAssetMgr::GetInst()->FindAsset<HYMaterial>(L"DistortionMtrl"));
	pObj->MeshRender()->GetMaterial()->SetTexParam(TEX_0, HYAssetMgr::GetInst()->Load<HYTexture>(L"NoiseTex", L"texture\\noise\\noise_03.jpg"));

	m_CurLevel->AddObject(pObj, L"Default", false);*/

	// Wave ȿ�� �߰�
	/*pObj = new HYGameObject;
	pObj->SetName(L"Wave Object");

	pObj->AddComponent(new HYTransform);
	pObj->AddComponent(new HYMeshRender);

	pObj->Transform()->SetRelativePos(Vec3(0.f, 0.f, 200.f));
	pObj->Transform()->SetRelativeScale(Vec3(300.f, 300.f, 1.f));

	pObj->MeshRender()->SetMesh(HYAssetMgr::GetInst()->FindAsset<HYMesh>(L"CircleMesh"));
	pObj->MeshRender()->SetMaterial(HYAssetMgr::GetInst()->FindAsset<HYMaterial>(L"WaveFilterMtrl"));
	pObj->MeshRender()->GetMaterial()->SetTexParam(TEX_0, HYAssetMgr::GetInst()->Load<HYTexture>(L"ChaosTex", L"texture\\ChaosRiftOpen6.png"));

	m_CurLevel->AddObject(pObj, L"Default", false);*/

	// �ܰ��� ȿ��
	/*pObj = new HYGameObject;
	pObj->SetName(L"OutLine Object");

	pObj->AddComponent(new HYTransform);
	pObj->AddComponent(new HYMeshRender);

	pObj->Transform()->SetRelativePos(Vec3(0.f, 0.f, 200.f));
	pObj->Transform()->SetRelativeScale(Vec3(800.f, 800.f, 1.f));

	pObj->MeshRender()->SetMesh(HYAssetMgr::GetInst()->FindAsset<HYMesh>(L"RectMesh"));
	pObj->MeshRender()->SetMaterial(HYAssetMgr::GetInst()->FindAsset<HYMaterial>(L"OutLineMtrl"));
	pObj->MeshRender()->GetMaterial()->SetTexParam(TEX_0, HYAssetMgr::GetInst()->Load<HYTexture>(L"NoiseTex", L"texture\\noise\\noise_03.jpg"));

	m_CurLevel->AddObject(pObj, L"Default", false);*/


	// Level ����
	m_CurLevel->begin();

}

void HYLevelMgr::tick()
{
	if (nullptr == m_CurLevel)
		return;

	// ���� �����ӿ� ��ϵ� ������Ʈ�� clear
	// �� �����Ӹ��� ������ �ڱ� �ڽ��� �ǽð����� ����� �� ���̱� ������
	// clear�� ����� ��(���� �������� �� ������ �Ŵϱ�)
	m_CurLevel->clear();

	m_CurLevel->tick();
	m_CurLevel->finaltick();
}

