#include "pch.h"
#include "HYEntity.h"

UINT HYEntity::g_NextID = 0;

HYEntity::HYEntity()
	: m_ID(g_NextID++)
{
}

HYEntity::~HYEntity()
{
}
