#include "pch.h"
#include "HYCamera.h"

#include "HYDevice.h"
#include "HYTransform.h"

#include "HYRenderMgr.h"
#include "HYLevelMgr.h"
#include "HYLevel.h"
#include "HYLayer.h"
#include "HYGameObject.h"
#include "HYRenderComponent.h"

#include "HYAssetMgr.h"


HYCamera::HYCamera()
	: HYComponent(COMPONENT_TYPE::CAMERA)
	, m_ProjType(PROJ_TYPE::ORTHOGRAPHIC)
	, m_FOV(XM_PI / 2.f) // 90도 
	, m_Width(0.f)
	, m_Scale(1.f)
	, m_AspectRatio(1.f)
	, m_Far(10000.f)
	, m_LayerCheck(0)
	, m_CameraPriority(-1)
{
	Vec2 vResol = HYDevice::GetInst()->GetRenderResolution();

	// 종횡비 : 가로 대 세로에 대한 비율값(가로가 세로 대비 어느 정도 비율인가)
	m_AspectRatio = vResol.x / vResol.y;

}

HYCamera::~HYCamera()
{
}

void HYCamera::begin()
{
	// 카메라를 우선순위값에 맞게 RenderMgr 에 등록시킴
	HYRenderMgr::GetInst()->RegisterCamera(this, m_CameraPriority);
}


// 뷰행렬, 투영행렬 생성
void HYCamera::finaltick()
{
	Vec3 vCamPos = Transform()->GetRelativePos();
	// 물체가 반대방향으로 이동
	Matrix matTrans = XMMatrixTranslation(-vCamPos.x, -vCamPos.y, -vCamPos.z);

	// 카메라의 각 우, 상, 전방 방향을 기저축이랑 일치시키도록 회전하는 회전행렬
	Vec3 vRight = Transform()->GetWorldDir(DIR_TYPE::RIGHT);
	Vec3 vUp	= Transform()->GetWorldDir(DIR_TYPE::UP);
	Vec3 vFront = Transform()->GetWorldDir(DIR_TYPE::FRONT);

	// XMMatrixIdentity : 단위행렬로 초기화
	Matrix matRotate = XMMatrixIdentity();
	matRotate._11 = vRight.x; matRotate._12 = vUp.x; matRotate._13 = vFront.x;
	matRotate._21 = vRight.y; matRotate._22 = vUp.y; matRotate._23 = vFront.y;
	matRotate._31 = vRight.z; matRotate._32 = vUp.z; matRotate._33 = vFront.z;

	// 이동과 회전을 둘 다 해야 World Space -> View Space가 되었다고 봄
	// 이동 x 회전(카메라가 바라보는 방향이 z축이어야 하므로) = view 행렬
	// 곱해주는 순서는 카메라 변환도 적용되는 물체 입장에서는 이동한 후에
	// 회전을 하기 때문에 이렇게 됨
	m_matView = matTrans * matRotate;

	// 투영 방식에 따른 투영 행렬을 계산
	m_matProj = XMMatrixIdentity();

	// 직교투영
	if (PROJ_TYPE::ORTHOGRAPHIC == m_ProjType)
	{

		// 실제 화면 해상도
		Vec2 vResol = HYDevice::GetInst()->GetRenderResolution();
		// LH : 왼손 좌표계
		// View Width : View Space에서 투영하고 싶은 가로 길이 -> 해상도와 일치시킴
		// 게임 내에서의 좌표 1이 1픽셀이 됨
		// View Height : 종횡비를 통해 구함
		// Far를 넘어서는 애들은 직교 투영을 곱했을 때 1을 넘어버림
		// 가로,세로 내에 들어오고 1~10000사이에 있는 애들만 최종적으로 래스터라이저에게
		// 넘겨져서 NDC좌표계 안으로 들어옴
		// m_Scale : 투영하고 있는 범위를 넓게하거나 좁게할 때 사용
		// 투영하는 범위가 클수록 화면에서 보이는 것은 더 작아짐
		m_matProj = XMMatrixOrthographicLH(vResol.x * m_Scale, (vResol.x / m_AspectRatio) * m_Scale, 1.f, m_Far);
	}
	// 원근투영
	else
	{
		// 시야각을 활용하여 화면에서 보이는 크기를 조절할 수 있음
		// 화면이 확 확대되거나 멀어지는 것 같은 느낌을 줄 수 있음
		m_matProj = XMMatrixPerspectiveFovLH(m_FOV, m_AspectRatio, 1.f, m_Far);
	}

	// 계산한 view 행렬과 proj 행렬을 전역변수에 담아둔다.
	g_Transform.matView = m_matView;
	g_Transform.matProj = m_matProj;
}

// Camera의 우선 순위 지정
void HYCamera::SetCameraPriority(int _Priority)
{
	m_CameraPriority = _Priority;
}

// LayerIdx로 들어온 것은 Level을 알 필요가 없음
void HYCamera::LayerCheck(UINT _LayerIdx, bool _bCheck)
{
	// 비트 연산
	if (_bCheck)
	{
		// 왼쪽 민 다음 Or 연산
		m_LayerCheck |= (1 << _LayerIdx);
	}    
	else
	{
		// 왼쪽으로 민 다음 반전, And 연산 -> 원하는 비트가 빠짐
		m_LayerCheck &= ~(1 << _LayerIdx);
	}
}

void HYCamera::LayerCheck(const wstring& _strLayerName, bool _bCheck)
{
	HYLevel* pCurLevel = HYLevelMgr::GetInst()->GetCurrentLevel();
	HYLayer* pLayer = pCurLevel->GetLayer(_strLayerName);

	if (nullptr == pLayer)
		return;

	int idx = pLayer->GetLayerIdx();
	LayerCheck(idx, _bCheck);
}

void HYCamera::SortObject()
{
	HYLevel* pCurLevel = HYLevelMgr::GetInst()->GetCurrentLevel();

	// 찍고자 하는 Layer인지 Check
	for (int i = 0; i < LAYER_MAX; ++i)
	{
		// m_LayerCheck와 i번째 비트값을 확인해서
		// 카메라가 찍도록 설정된 Layer 가 아니면 무시
		// 1 << i : 1을 왼쪽으로 i번째만큼 이동
		if (false == (m_LayerCheck & (1 << i)))
			continue;

		HYLayer* pLayer = pCurLevel->GetLayer(i);
		const vector<HYGameObject*>& vecObjects = pLayer->GetLayerObjects();
		for (size_t j = 0; j < vecObjects.size(); ++j)
		{
			// 분류를 한다는 것 자체가 랜더링 기능이 존재한다는 것이므로
			// GetRenderComopnent를 통해 내가 보유하고 있는 Component 
			// 랜더링 기능이 다 제대로 있다면 
			// 메쉬, 재질, 쉐이더 확인
			if (!(vecObjects[j]->GetRenderComopnent()
				&& vecObjects[j]->GetRenderComopnent()->GetMesh().Get()
				&& vecObjects[j]->GetRenderComopnent()->GetMaterial().Get()
				&& vecObjects[j]->GetRenderComopnent()->GetMaterial()->GetShader().Get()))
			{
				continue;
			}

			// 쉐이더 도메인을 확인해서 각 해당 벡터에 넣어줌
			SHADER_DOMAIN domain = vecObjects[j]->GetRenderComopnent()->GetMaterial()->GetShader()->GetDomain();

			switch (domain)
			{
			case SHADER_DOMAIN::DOMAIN_OPAQUE:
				m_vecOpaque.push_back(vecObjects[j]);
				break;
			case SHADER_DOMAIN::DOMAIN_MASKED:
				m_vecMaked.push_back(vecObjects[j]);
				break;
			case SHADER_DOMAIN::DOMAIN_TRANSPARENT:
				m_vecTransparent.push_back(vecObjects[j]);
				break;
			case SHADER_DOMAIN::DOMAIN_POSTPROCESS:
				m_vecPostProcess.push_back(vecObjects[j]);
				break;
			case SHADER_DOMAIN::DOMAIN_DEBUG:
				break;
			}
		}
	}
}


void HYCamera::render()
{
	// 계산한 view 행렬과 proj 행렬을 전역변수에 담아둔다.
	g_Transform.matView = m_matView;
	g_Transform.matProj = m_matProj;

	// Domain 순서대로 렌더링
	render(m_vecOpaque);
	render(m_vecMaked);
	render(m_vecTransparent);

	// 후처리 작업(기존 랜더링과는 다르게 추가로 해줘야 하는 작업이 있어서 따로 빼줌)
	render_postprocess();
}

void HYCamera::render(vector<HYGameObject*>& _vecObj)
{
	for (size_t i = 0; i < _vecObj.size(); ++i)
	{
		_vecObj[i]->render();
	}
	_vecObj.clear();
}

void HYCamera::render_postprocess()
{
	for (size_t i = 0; i < m_vecPostProcess.size(); ++i)
	{
		// 최종 렌더링 이미지를 후처리 타겟에 복사
		HYRenderMgr::GetInst()->CopyRenderTargetToPostProcessTarget();

		// 복사받은 후처리 텍스쳐를 t13 레지스터에 바인딩
		Ptr<HYTexture> pPostProcessTex = HYRenderMgr::GetInst()->GetPostProcessTex();
		// 후처리 과정에서도 새로운 텍스처가 필요할 수도 있기 때문에 전용 레지스터에 바인딩
		pPostProcessTex->UpdateData(13);

		// 후처리 오브젝트 렌더링
		m_vecPostProcess[i]->render();
	}

	m_vecPostProcess.clear();
}

void HYCamera::GetCameraTypeName(vector<string>& _Out)
{
	_Out.push_back("OrthoGraphic");
	_Out.push_back("Perspective");
}

string HYCamera::GetCameraTypeName(PROJ_TYPE _Type)
{
	switch (_Type)
	{
	case PROJ_TYPE::ORTHOGRAPHIC:
		return "OrthoGraphic";
	case PROJ_TYPE::PERSPECTIVE:
		return "Perspective";
	default:
		return "";
	}
}

PROJ_TYPE HYCamera::GetCameraType(string str)
{
	if (str == "OrthoGraphic")
	{
		return PROJ_TYPE::ORTHOGRAPHIC;
	}
	else if (str == "Perspective")
	{
		return PROJ_TYPE::PERSPECTIVE;
	}
}

void HYCamera::SaveToFile(FILE* _File)
{
	fwrite(&m_ProjType, sizeof(PROJ_TYPE), 1, _File);
	fwrite(&m_FOV, sizeof(float), 1, _File);
	fwrite(&m_Width, sizeof(float), 1, _File);
	fwrite(&m_Scale, sizeof(float), 1, _File);
	fwrite(&m_AspectRatio, sizeof(float), 1, _File);
	fwrite(&m_Far, sizeof(float), 1, _File);
	fwrite(&m_LayerCheck, sizeof(UINT), 1, _File);
	fwrite(&m_CameraPriority, sizeof(int), 1, _File);
}

void HYCamera::LoadFromFile(FILE* _File)
{
	fread(&m_ProjType, sizeof(PROJ_TYPE), 1, _File);
	fread(&m_FOV, sizeof(float), 1, _File);
	fread(&m_Width, sizeof(float), 1, _File);
	fread(&m_Scale, sizeof(float), 1, _File);
	fread(&m_AspectRatio, sizeof(float), 1, _File);
	fread(&m_Far, sizeof(float), 1, _File);
	fread(&m_LayerCheck, sizeof(UINT), 1, _File);
	fread(&m_CameraPriority, sizeof(int), 1, _File);
}
