#include "pch.h"
#include "HYGC.h"

// ��� ������ ������ delete�� �Ϸ��� �ص� �Ҹ��� ȣ���� �ȵ�
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
