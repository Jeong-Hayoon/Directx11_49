#include "pch.h"
#include "HYGC.h"

// 헤더 정보가 없으면 delete를 하려고 해도 소멸자 호출이 안됨
#include "HYEntity.h"

HYGC::HYGC()
{
}

HYGC::~HYGC()
{
	Delete_Vec(m_vecEntity);
}

void HYGC::tick()
{
	if (100 < m_vecEntity.size())
	{
		Delete_Vec(m_vecEntity);
	}
}
