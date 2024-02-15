#include "pch.h"
#include "ParamUI.h"

#include <Engine/HYAssetMgr.h>

#include "imgui.h"
#include "ListUI.h"

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

bool ParamUI::Param_FLOAT(float* _Data, const string& _Desc)
{
	ImGui::Text(_Desc.c_str());
	ImGui::SameLine();

	char szID[256] = {};
	sprintf_s(szID, "##float%d", g_ID++);
	if (ImGui::InputFloat(szID, _Data))
	{
		return true;
	}

	return false;
}

bool ParamUI::Param_VEC2(Vec2* _Data, const string& _Desc)
{
	ImGui::Text(_Desc.c_str());
	ImGui::SameLine();

	float arrFloat[2] = { _Data->x, _Data->y };
	char szID[256] = {};

	sprintf_s(szID, "##Vec2%d", g_ID++);

	if (ImGui::InputFloat2(szID, arrFloat))
	{
		_Data->x = arrFloat[0];
		_Data->y = arrFloat[1];
		return true;
	}
}

bool ParamUI::Param_VEC4(Vec4* _Data, const string& _Desc)
{
	ImGui::Text(_Desc.c_str());
	ImGui::SameLine();

	char szID[256] = {};
	sprintf_s(szID, "##Vec4%d", g_ID++);
	
	// Vec4는 배열로 바꾸는 Conversion이 있어서 _Data 앞에 *만 붙여주면 됨
	if (ImGui::InputFloat4(szID, *_Data))
	{
		return true;
	}
}

// 버튼을 눌러서 ListUI를 띄우면 true 반환 / Drag&Drop은 텍스처 원본 수정
// Param_TEXTURE(해당 위치에 이미 전달되어 있는 텍스처, 설명 정보, 함수 호출할 객체, 함수 포인터)
bool ParamUI::Param_TEXTURE(Ptr<HYTexture>& _Texture, const string& _Desc, UI* _Inst, Delegate_1 _Func)
{
	// 파라미터의 Description 정보 출력
	ImGui::Text(_Desc.c_str());
	ImGui::SameLine();

	// Texture 이름 출력 InputText 의 ID 만들기
	char szID[256] = {};
	sprintf_s(szID, "##Texture%d", g_ID++);

	ImTextureID texid = nullptr;
	string strTexKey;

	if (nullptr != _Texture)
	{
		texid = _Texture->GetSRV().Get();
		strTexKey = string(_Texture->GetKey().begin(), _Texture->GetKey().end());
	}

	ImGui::InputText(szID, (char*)strTexKey.c_str(), strTexKey.length(), ImGuiInputTextFlags_ReadOnly);

	// Texture Drop 체크
	if (ImGui::BeginDragDropTarget())
	{
		const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ContentTree");

		if (payload)
		{
			DWORD_PTR data = *((DWORD_PTR*)payload->Data);
			HYAsset* pAsset = (HYAsset*)data;
			if (ASSET_TYPE::TEXTURE == pAsset->GetType())
			{
				_Texture = (HYTexture*)pAsset;
			}
		}

		ImGui::EndDragDropTarget();
	}

	// 텍스쳐 이미지 출력
	static bool use_text_color_for_tint = false;
	ImVec2 uv_min = ImVec2(0.0f, 0.0f);
	ImVec2 uv_max = ImVec2(1.0f, 1.0f);
	ImVec4 tint_col = use_text_color_for_tint ? ImGui::GetStyleColorVec4(ImGuiCol_Text) : ImVec4(1.0f, 1.0f, 1.0f, 1.0f); // No tint
	ImVec4 border_col = ImGui::GetStyleColorVec4(ImGuiCol_Border);
	ImGui::Image(texid, ImVec2(150, 150), uv_min, uv_max, tint_col, border_col);

	// 입력된 델리게이트가 있다면
	if (_Inst && _Func)
	{
		ImGui::SameLine();
		sprintf_s(szID, "##TexBtn%d", g_ID++);
		if (ImGui::Button(szID, ImVec2(20, 20)))
		{
			// 리스트 UI
			ListUI* pListUI = (ListUI*)HYImGuiMgr::GetInst()->FindUI("##List");

			// AssetMgr를 통해 텍스처 이름 받아오기
			vector<string> vecTexName;
			HYAssetMgr::GetInst()->GetAssetName(ASSET_TYPE::TEXTURE, vecTexName);

			pListUI->AddString(vecTexName);
			pListUI->SetDbClickDelegate(_Inst, (Delegate_1)_Func);
			pListUI->Activate();

			return true;
		}
	}

	return false;
}
