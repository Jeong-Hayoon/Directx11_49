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

void HYBlackboard::AddBlackboardData(const wstring& _strKey, BB_DATA _Type, void* _Data)
{
	map<wstring, tBlackboardData>::iterator iter = m_mapBBData.find(_strKey);

	assert(iter == m_mapBBData.end());

	m_mapBBData.insert(make_pair(_strKey, tBlackboardData{ _Type , _Data }));
}
