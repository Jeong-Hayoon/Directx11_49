#include "pch.h"
#include "HYBlackboard.h"

HYBlackboard::HYBlackboard()
{
}

HYBlackboard::HYBlackboard(const HYBlackboard& _board)
	: HYEntity(_board)
	, m_mapBBData()
{
}

HYBlackboard::~HYBlackboard()
{
}

// BlackBoard에 데이터 추가하는 함수
void HYBlackboard::AddBlackboardData(const wstring& _strKey, BB_DATA _Type, void* _Data)
{
	map<wstring, tBlackboardData>::iterator iter = m_mapBBData.find(_strKey);

	assert(iter == m_mapBBData.end());

	m_mapBBData.insert(make_pair(_strKey, tBlackboardData{ _Type , _Data }));
}

void* HYBlackboard::GetBlackboardData(const wstring& _strKey)
{
	map<wstring, tBlackboardData>::iterator iter = m_mapBBData.find(_strKey);

	if (iter == m_mapBBData.end())
		return nullptr;

	return iter->second.pData;
}