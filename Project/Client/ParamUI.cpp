#include "pch.h"
#include "ParamUI.h"

#include "imgui.h"

// ���� ��� �ʱ�ȭ
int ParamUI::g_ID = 0;

// Param_INT(��������ְ� ���� �������� ���� �ּ�, �������� ���� ����), ��ȯ�� - �� ���� ����
bool ParamUI::Param_INT(int* _Data, const string& _Desc)
{
	ImGui::Text(_Desc.c_str());
	ImGui::SameLine();

	char szID[256] = {};

	sprintf_s(szID, "##int%d", g_ID++);
	if (ImGui::InputInt(szID, _Data))
	{
		return true;
	}

	return false;
}
