#include "pch.h"
#include "HYAnim.h"

#include "HYTimeMgr.h"
		  
#include "HYDevice.h"
#include "HYConstBuffer.h"

HYAnim::HYAnim()
	: m_Animator(nullptr)
	, m_CurFrmIdx(0)
	, m_bFinish(false)
	, m_AccTime(0.f)
{
}

HYAnim::~HYAnim()
{
}

void HYAnim::finaltick()
{
	m_AccTime += DT;

	// ������ �ð��� Duration�� ������
	if (m_vecFrm[m_CurFrmIdx].Duration < m_AccTime)
	{
		// Frame Index�� ������ �ٽ� 0���� �ʱ�ȭ
		++m_CurFrmIdx;
		if (m_vecFrm.size() <= m_CurFrmIdx)
		{
			m_CurFrmIdx = m_vecFrm.size() - 1;
			m_bFinish = true;
		}
		m_AccTime = 0.f;
	}
}

void HYAnim::UpdateData()
{
	// ���� ������ ������ ������۷� �ű�� b2 �������ͷ� ���ε�
	static HYConstBuffer* pCB = HYDevice::GetInst()->GetConstBuffer(CB_TYPE::ANIM2D_DATA);
	tAnimData2D data = {};
	data.UseAnim2D = 1;
	data.vLeftTop = m_vecFrm[m_CurFrmIdx].vLeftTop;
	data.vOffset = m_vecFrm[m_CurFrmIdx].vOffset;
	data.vSlizeSize = m_vecFrm[m_CurFrmIdx].vSlice;
	data.vBackGround = m_vecFrm[m_CurFrmIdx].vBackground;

	pCB->SetData(&data);
	pCB->UpdateData();

	// ��Ʋ�� �ؽ��� t10 �� ���ε�
	m_AtlasTex->UpdateData(10);
}

void HYAnim::Clear()
{
	static HYConstBuffer* pCB = HYDevice::GetInst()->GetConstBuffer(CB_TYPE::ANIM2D_DATA);
	tAnimData2D data = {};
	data.UseAnim2D = 0;

	pCB->SetData(&data);
	pCB->UpdateData();
}

void HYAnim::Create(HYAnimator2D* _Animator, Ptr<HYTexture> _Atlas, Vec2 _vLeftTop
	, Vec2 _vSliceSize, Vec2 _vOffset, Vec2 _vBackground, int _FrmCount, float _FPS)
{
	m_Animator = _Animator;
	m_AtlasTex = _Atlas;

	for (int i = 0; i < _FrmCount; ++i)
	{
		tAnimFrm frm = {};

		// Pixel Unit -> UV Unit���� ��ȯ

		frm.vSlice = Vec2(_vSliceSize.x / (float)_Atlas->GetWidth(), _vSliceSize.y / (float)_Atlas->GetHeight());

		frm.vLeftTop = Vec2(_vLeftTop.x / (float)_Atlas->GetWidth() + frm.vSlice.x * i, _vLeftTop.y / (float)_Atlas->GetHeight());

		frm.vOffset = Vec2(_vOffset.x / (float)_Atlas->GetWidth(), _vOffset.y / (float)_Atlas->GetHeight());
		// �ʴ� 60 ������ -> �� ������ �� ���� �ð� 60���� 1��
		frm.Duration = 1.f / _FPS;

		frm.vBackground = Vec2(_vBackground.x / (float)_Atlas->GetWidth(), _vBackground.y / (float)_Atlas->GetHeight());

		m_vecFrm.push_back(frm);
	}

	// Ư�� �̹����� �����Ͽ� ������ ����(�ӽ�)
	//m_vecFrm[1].vOffset.x = 5.0f / (float)_Atlas->GetWidth();
}
