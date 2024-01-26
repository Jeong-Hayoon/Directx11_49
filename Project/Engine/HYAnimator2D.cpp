#include "pch.h"
#include "HYAnimator2D.h"

#include "HYAnim.h"

HYAnimator2D::HYAnimator2D()
	: HYComponent(COMPONENT_TYPE::ANIMATOR2D)
{
}

HYAnimator2D::~HYAnimator2D()
{
	Delete_Map(m_mapAnim);
}

void HYAnimator2D::finaltick()
{
	if (nullptr == m_CurAnim)
		return;

	if (m_CurAnim->IsFinish() && m_bRepeat)
	{
		m_CurAnim->Reset();
	}

	m_CurAnim->finaltick();
}

void HYAnimator2D::UpdateData()
{
	if (nullptr == m_CurAnim)
		return;

	m_CurAnim->UpdateData();
}

void HYAnimator2D::Clear()
{
	HYAnim::Clear();
}

void HYAnimator2D::Create(const wstring& _strKey, Ptr<HYTexture> _AltasTex, Vec2 _LeftTop
	, Vec2 _vSliceSize, Vec2 _OffsetSize, Vec2 _Background, int _FrmCount, float _FPS)
{
	HYAnim* pAnim = FindAnim(_strKey);
	
	assert(!pAnim);

	pAnim = new HYAnim;
	pAnim->Create(this, _AltasTex, _LeftTop, _vSliceSize, _OffsetSize, _Background, _FrmCount, _FPS);
	m_mapAnim.insert(make_pair(_strKey, pAnim));
}

HYAnim* HYAnimator2D::FindAnim(const wstring& _strKey)
{
	map<wstring, HYAnim*>::iterator iter = m_mapAnim.find(_strKey);

	if (iter == m_mapAnim.end())
		return nullptr;

	return iter->second;
}

void HYAnimator2D::Play(const wstring& _strAnimName, bool _bRepeat)
{
	HYAnim* pAnim = FindAnim(_strAnimName);
	if (nullptr == pAnim)
		return;

	m_bRepeat = _bRepeat;

	m_CurAnim = pAnim;
	m_CurAnimName = _strAnimName;
	m_CurAnim->Reset();
}

void HYAnimator2D::GetAnimationName(vector<string>& _Out)
{
	map<wstring, HYAnim*>::iterator iter = m_mapAnim.begin();

	for (auto iter = m_mapAnim.begin();  iter!= m_mapAnim.end(); ++iter)
	{
		// first가 key값을 string으로 바꿔서 들어온 벡터에 담아줌
		_Out.push_back(ToString(iter->first));
	}
}