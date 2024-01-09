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

	// 누적된 시간이 Duration을 넘으면
	if (m_vecFrm[m_CurFrmIdx].Duration < m_AccTime)
	{
		// Frame Index를 넘으면 다시 0으로 초기화
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
	// 현재 프레임 정보를 상수버퍼로 옮기고 b2 레지스터로 바인딩
	static HYConstBuffer* pCB = HYDevice::GetInst()->GetConstBuffer(CB_TYPE::ANIM2D_DATA);
	tAnimData2D data = {};
	data.UseAnim2D = 1;
	data.vLeftTop = m_vecFrm[m_CurFrmIdx].vLeftTop;
	data.vOffset = m_vecFrm[m_CurFrmIdx].vOffset;
	data.vSlizeSize = m_vecFrm[m_CurFrmIdx].vSlice;
	data.vBackGround = m_vecFrm[m_CurFrmIdx].vBackground;

	pCB->SetData(&data);
	pCB->UpdateData();

	// 아틀라스 텍스쳐 t10 에 바인딩
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

		// Pixel Unit -> UV Unit으로 변환

		frm.vSlice = Vec2(_vSliceSize.x / (float)_Atlas->GetWidth(), _vSliceSize.y / (float)_Atlas->GetHeight());

		frm.vLeftTop = Vec2(_vLeftTop.x / (float)_Atlas->GetWidth() + frm.vSlice.x * i, _vLeftTop.y / (float)_Atlas->GetHeight());

		frm.vOffset = Vec2(_vOffset.x / (float)_Atlas->GetWidth(), _vOffset.y / (float)_Atlas->GetHeight());
		// 초당 60 프레임 -> 한 프레임 당 유지 시간 60분의 1초
		frm.Duration = 1.f / _FPS;

		frm.vBackground = Vec2(_vBackground.x / (float)_Atlas->GetWidth(), _vBackground.y / (float)_Atlas->GetHeight());

		m_vecFrm.push_back(frm);
	}

	// 특정 이미지만 지정하여 오프셋 조정(임시)
	//m_vecFrm[1].vOffset.x = 5.0f / (float)_Atlas->GetWidth();
}
