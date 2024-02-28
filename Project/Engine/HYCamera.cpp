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
	, m_FOV(XM_PI / 2.f) // 90�� 
	, m_Width(0.f)
	, m_Scale(1.f)
	, m_AspectRatio(1.f)
	, m_Far(10000.f)
	, m_LayerCheck(0)
	, m_CameraPriority(-1)
{
	Vec2 vResol = HYDevice::GetInst()->GetRenderResolution();

	// ��Ⱦ�� : ���� �� ���ο� ���� ������(���ΰ� ���� ��� ��� ���� �����ΰ�)
	m_AspectRatio = vResol.x / vResol.y;

}

HYCamera::~HYCamera()
{
}

void HYCamera::begin()
{
	// ī�޶� �켱�������� �°� RenderMgr �� ��Ͻ�Ŵ
	HYRenderMgr::GetInst()->RegisterCamera(this, m_CameraPriority);
}


// �����, ������� ����
void HYCamera::finaltick()
{
	Vec3 vCamPos = Transform()->GetRelativePos();
	// ��ü�� �ݴ�������� �̵�
	Matrix matTrans = XMMatrixTranslation(-vCamPos.x, -vCamPos.y, -vCamPos.z);

	// ī�޶��� �� ��, ��, ���� ������ �������̶� ��ġ��Ű���� ȸ���ϴ� ȸ�����
	Vec3 vRight = Transform()->GetWorldDir(DIR_TYPE::RIGHT);
	Vec3 vUp	= Transform()->GetWorldDir(DIR_TYPE::UP);
	Vec3 vFront = Transform()->GetWorldDir(DIR_TYPE::FRONT);

	// XMMatrixIdentity : ������ķ� �ʱ�ȭ
	Matrix matRotate = XMMatrixIdentity();
	matRotate._11 = vRight.x; matRotate._12 = vUp.x; matRotate._13 = vFront.x;
	matRotate._21 = vRight.y; matRotate._22 = vUp.y; matRotate._23 = vFront.y;
	matRotate._31 = vRight.z; matRotate._32 = vUp.z; matRotate._33 = vFront.z;

	// �̵��� ȸ���� �� �� �ؾ� World Space -> View Space�� �Ǿ��ٰ� ��
	// �̵� x ȸ��(ī�޶� �ٶ󺸴� ������ z���̾�� �ϹǷ�) = view ���
	// �����ִ� ������ ī�޶� ��ȯ�� ����Ǵ� ��ü ���忡���� �̵��� �Ŀ�
	// ȸ���� �ϱ� ������ �̷��� ��
	m_matView = matTrans * matRotate;

	// ���� ��Ŀ� ���� ���� ����� ���
	m_matProj = XMMatrixIdentity();

	// ��������
	if (PROJ_TYPE::ORTHOGRAPHIC == m_ProjType)
	{

		// ���� ȭ�� �ػ�
		Vec2 vResol = HYDevice::GetInst()->GetRenderResolution();
		// LH : �޼� ��ǥ��
		// View Width : View Space���� �����ϰ� ���� ���� ���� -> �ػ󵵿� ��ġ��Ŵ
		// ���� �������� ��ǥ 1�� 1�ȼ��� ��
		// View Height : ��Ⱦ�� ���� ����
		// Far�� �Ѿ�� �ֵ��� ���� ������ ������ �� 1�� �Ѿ����
		// ����,���� ���� ������ 1~10000���̿� �ִ� �ֵ鸸 ���������� �����Ͷ���������
		// �Ѱ����� NDC��ǥ�� ������ ����
		// m_Scale : �����ϰ� �ִ� ������ �а��ϰų� ������ �� ���
		// �����ϴ� ������ Ŭ���� ȭ�鿡�� ���̴� ���� �� �۾���
		m_matProj = XMMatrixOrthographicLH(vResol.x * m_Scale, (vResol.x / m_AspectRatio) * m_Scale, 1.f, m_Far);
	}
	// ��������
	else
	{
		// �þ߰��� Ȱ���Ͽ� ȭ�鿡�� ���̴� ũ�⸦ ������ �� ����
		// ȭ���� Ȯ Ȯ��ǰų� �־����� �� ���� ������ �� �� ����
		m_matProj = XMMatrixPerspectiveFovLH(m_FOV, m_AspectRatio, 1.f, m_Far);
	}

	// ����� view ��İ� proj ����� ���������� ��Ƶд�.
	g_Transform.matView = m_matView;
	g_Transform.matProj = m_matProj;
}

// Camera�� �켱 ���� ����
void HYCamera::SetCameraPriority(int _Priority)
{
	m_CameraPriority = _Priority;
}

// LayerIdx�� ���� ���� Level�� �� �ʿ䰡 ����
void HYCamera::LayerCheck(UINT _LayerIdx, bool _bCheck)
{
	// ��Ʈ ����
	if (_bCheck)
	{
		// ���� �� ���� Or ����
		m_LayerCheck |= (1 << _LayerIdx);
	}    
	else
	{
		// �������� �� ���� ����, And ���� -> ���ϴ� ��Ʈ�� ����
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

	// ����� �ϴ� Layer���� Check
	for (int i = 0; i < LAYER_MAX; ++i)
	{
		// m_LayerCheck�� i��° ��Ʈ���� Ȯ���ؼ�
		// ī�޶� �ﵵ�� ������ Layer �� �ƴϸ� ����
		// 1 << i : 1�� �������� i��°��ŭ �̵�
		if (false == (m_LayerCheck & (1 << i)))
			continue;

		HYLayer* pLayer = pCurLevel->GetLayer(i);
		const vector<HYGameObject*>& vecObjects = pLayer->GetLayerObjects();
		for (size_t j = 0; j < vecObjects.size(); ++j)
		{
			// �з��� �Ѵٴ� �� ��ü�� ������ ����� �����Ѵٴ� ���̹Ƿ�
			// GetRenderComopnent�� ���� ���� �����ϰ� �ִ� Component 
			// ������ ����� �� ����� �ִٸ� 
			// �޽�, ����, ���̴� Ȯ��
			if (!(vecObjects[j]->GetRenderComopnent()
				&& vecObjects[j]->GetRenderComopnent()->GetMesh().Get()
				&& vecObjects[j]->GetRenderComopnent()->GetMaterial().Get()
				&& vecObjects[j]->GetRenderComopnent()->GetMaterial()->GetShader().Get()))
			{
				continue;
			}

			// ���̴� �������� Ȯ���ؼ� �� �ش� ���Ϳ� �־���
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
	// ����� view ��İ� proj ����� ���������� ��Ƶд�.
	g_Transform.matView = m_matView;
	g_Transform.matProj = m_matProj;

	// Domain ������� ������
	render(m_vecOpaque);
	render(m_vecMaked);
	render(m_vecTransparent);

	// ��ó�� �۾�(���� ���������� �ٸ��� �߰��� ����� �ϴ� �۾��� �־ ���� ����)
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
		// ���� ������ �̹����� ��ó�� Ÿ�ٿ� ����
		HYRenderMgr::GetInst()->CopyRenderTargetToPostProcessTarget();

		// ������� ��ó�� �ؽ��ĸ� t13 �������Ϳ� ���ε�
		Ptr<HYTexture> pPostProcessTex = HYRenderMgr::GetInst()->GetPostProcessTex();
		// ��ó�� ���������� ���ο� �ؽ�ó�� �ʿ��� ���� �ֱ� ������ ���� �������Ϳ� ���ε�
		pPostProcessTex->UpdateData(13);

		// ��ó�� ������Ʈ ������
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
