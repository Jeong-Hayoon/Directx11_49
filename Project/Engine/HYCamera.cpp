#include "pch.h"
#include "HYCamera.h"

#include "HYDevice.h"
#include "HYTransform.h"

#include "HYRenderMgr.h"
#include "HYLevelMgr.h"
#include "HYLevel.h"
#include "HYLayer.h"
#include "HYGameObject.h"

HYCamera::HYCamera()
	: HYComponent(COMPONENT_TYPE::CAMERA)
	, m_ProjType(PROJ_TYPE::ORTHOGRAPHIC)
	, m_FOV(XM_PI / 2.f) // 90�� 
	, m_Width(0.f)
	, m_Scale(1.f)
	, m_AspectRatio(1.f)
	, m_Far(10000.f)
	, m_LayerCheck(0)
{
	Vec2 vResol = HYDevice::GetInst()->GetRenderResolution();

	// ��Ⱦ�� : ���� �� ���ο� ���� ������(���ΰ� ���� ��� ��� ���� �����ΰ�)
	m_AspectRatio = vResol.x / vResol.y;

}

HYCamera::~HYCamera()
{
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
	HYRenderMgr::GetInst()->RegisterCamera(this, _Priority);
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

void HYCamera::render()
{
	// ����� view ��İ� proj ����� ���������� ��Ƶд�.
	g_Transform.matView = m_matView;
	g_Transform.matProj = m_matProj;

	HYLevel* pCurLevel = HYLevelMgr::GetInst()->GetCurrentLevel();

	for (int i = 0; i < LAYER_MAX; ++i) 
	{
		// m_LayerCheck�� i��° ��Ʈ���� Ȯ���ؼ�
		// ī�޶� �ﵵ�� ������ Layer �� �ƴϸ� ����
		// 1 << i : 1�� �������� i��°��ŭ �̵�
		if (false == (m_LayerCheck & (1 << i)))
			continue;

		HYLayer* pLayer = pCurLevel->GetLayer(i);
		const vector<HYGameObject*>& vecObjects = pLayer->GetLayerObjects();
		for (size_t i = 0; i < vecObjects.size(); ++i)
		{
			vecObjects[i]->render();
		}
	}
}




