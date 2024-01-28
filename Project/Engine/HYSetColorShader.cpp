#include "pch.h"
#include "HYSetColorShader.h"


HYSetColorShader::HYSetColorShader()
	: HYComputeShader(32, 32, 1)
{
	Create(L"shader\\setcolor.fx", "CS_SetColor");
}

HYSetColorShader::~HYSetColorShader()
{
}

int HYSetColorShader::UpdateData()
{
	if (nullptr == m_TargetTex)
		return E_FAIL;

	// ��� ����
	m_Const.iArr[0] = m_TargetTex->GetWidth();
	m_Const.iArr[1] = m_TargetTex->GetHeight();
	m_Const.v4Arr[0] = m_Color;

	// �ؽ��� U0 �� ���ε�
	if (FAILED(m_TargetTex->UpdateData_CS_UAV(0)))
	{
		return E_FAIL;
	}

	return S_OK;
}

void HYSetColorShader::UpdateGroupCount()
{
	// ������ �׷� �� üũ
	UINT width = m_TargetTex->GetWidth();
	UINT height = m_TargetTex->GetHeight();

	// �̷��� ����Ͽ� �ؽ�ó�� ������ �׷� ���� �� �۰� �����Ͽ� Dispatch
	// +1�� 32�� ����� �ƴ� ���� �ֱ� ������
	SetGroupX(1 + width / m_ThreadX);
	SetGroupY(1 + height / m_ThreadY);
	SetGroupZ(1);
}

void HYSetColorShader::Clear()
{
	m_TargetTex->Clear_CS_UAV();
}
