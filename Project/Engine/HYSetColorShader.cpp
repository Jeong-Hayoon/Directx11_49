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

	// 상수 세팅
	m_Const.iArr[0] = m_TargetTex->GetWidth();
	m_Const.iArr[1] = m_TargetTex->GetHeight();
	m_Const.v4Arr[0] = m_Color;

	// 텍스쳐 U0 에 바인딩
	if (FAILED(m_TargetTex->UpdateData_CS_UAV(0)))
	{
		return E_FAIL;
	}

	return S_OK;
}

void HYSetColorShader::UpdateGroupCount()
{
	// 스레드 그룹 수 체크
	UINT width = m_TargetTex->GetWidth();
	UINT height = m_TargetTex->GetHeight();

	// 이렇게 계산하여 텍스처가 작으면 그룹 수를 더 작게 배정하여 Dispatch
	// +1은 32의 배수가 아닐 수도 있기 때문에
	SetGroupX(1 + width / m_ThreadX);
	SetGroupY(1 + height / m_ThreadY);
	SetGroupZ(1);
}

void HYSetColorShader::Clear()
{
	m_TargetTex->Clear_CS_UAV();
}
