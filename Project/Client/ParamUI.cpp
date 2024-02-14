#include "pch.h"
#include "ParamUI.h"

#include "imgui.h"

// 정적 멤버 초기화
int ParamUI::g_ID = 0;

// Param_INT(노출시켜주고 싶은 데이터의 시작 주소, 데이터의 설명 정보), 반환값 - 값 변경 여부
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
