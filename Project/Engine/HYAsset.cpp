#include "pch.h"
#include "HYAsset.h"

HYAsset::HYAsset(ASSET_TYPE _Type, bool _bEngineAsset)
	: m_Type(_Type)
	, m_bEngineAsset(_bEngineAsset)
	, m_RefCount(0)
{
}

HYAsset::HYAsset(const HYAsset& _Origin)
	: HYEntity(_Origin)
	, m_Type(_Origin.m_Type)
	, m_bEngineAsset(_Origin.m_bEngineAsset)
	, m_RefCount(0)
{
}

HYAsset::~HYAsset()
{
}