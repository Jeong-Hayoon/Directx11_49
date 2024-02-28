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
	, ui_SliceSize(-1, -1)
	, ui_vLeftTop(-1, -1)
	, ui_vBackground(-1, -1)
	, ui_FrmCount(-1)
	, ui_FPS(-1)
	, ui_vOffset(-1000, -1000)
{	  
}

HYAnim::HYAnim(const HYAnim& _OriginAnim)
	: m_Animator(nullptr)
	, m_vecFrm(_OriginAnim.m_vecFrm)
	, m_CurFrmIdx(_OriginAnim.m_CurFrmIdx)
	, m_bFinish(_OriginAnim.m_bFinish)
	, m_AtlasTex(_OriginAnim.m_AtlasTex)
	, m_AccTime(_OriginAnim.m_AccTime)
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
			m_CurFrmIdx = (int)m_vecFrm.size() - 1;
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

// _FPS : �������� ����
void HYAnim::Create(HYAnimator2D* _Animator, Ptr<HYTexture> _Atlas, Vec2 _vLeftTop
	, Vec2 _vSliceSize, Vec2 _vOffset, Vec2 _vBackground, int _FrmCount, float _FPS)
{
	m_Animator = _Animator;
	m_AtlasTex = _Atlas;

	for (int i = 0; i < _FrmCount; ++i)
	{
		tAnimFrm frm = {};

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


void HYAnim::SaveToFile(FILE* _File)
{
	// �ִϸ��̼� �̸� ����
	SaveWString(GetName(), _File);

	// ��� ������ ����, ���� ����
	size_t FrmSize = m_vecFrm.size();
	fwrite(&FrmSize, sizeof(size_t), 1, _File);
	fwrite(m_vecFrm.data(), sizeof(tAnimFrm), m_vecFrm.size(), _File);

	// �ִϸ��̼��� �����ϴ� �ؽ��� ���� ����
	SaveAssetRef(m_AtlasTex, _File);
}

void HYAnim::LoadFromFile(FILE* _File)
{
	// �ִϸ��̼� �̸� �ε�
	wstring strName;
	LoadWString(strName, _File);
	SetName(strName);

	// ��� ������ ���� �ε�
	size_t FrmSize = 0;
	fread(&FrmSize, sizeof(size_t), 1, _File);
	m_vecFrm.resize(FrmSize);
	fread(m_vecFrm.data(), sizeof(tAnimFrm), m_vecFrm.size(), _File);

	// �ִϸ��̼��� �����ϴ� �ؽ��� ���� �ε�
	LoadAssetRef(m_AtlasTex, _File);
}

