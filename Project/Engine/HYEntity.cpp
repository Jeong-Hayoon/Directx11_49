#include "pch.h"
#include "HYEntity.h"

UINT HYEntity::g_NextID = 0;

HYEntity::HYEntity()
	: m_ID(g_NextID++)
{
}

HYEntity::HYEntity(const HYEntity& _Origin)
	: m_ID(g_NextID++)
	, m_strName(_Origin.m_strName)
{
}

HYEntity::~HYEntity()
{
}
