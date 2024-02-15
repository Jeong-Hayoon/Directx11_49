#include "pch.h"
#include "ParamUI.h"

#include <Engine/HYAssetMgr.h>

#include "imgui.h"
#include "ListUI.h"

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
	
	// Vec4�� �迭�� �ٲٴ� Conversion�� �־ _Data �տ� *�� �ٿ��ָ� ��
	if (ImGui::InputFloat4(szID, *_Data))
	{
		return true;
	}
}

// ��ư�� ������ ListUI�� ���� true ��ȯ / Drag&Drop�� �ؽ�ó ���� ����
// Param_TEXTURE(�ش� ��ġ�� �̹� ���޵Ǿ� �ִ� �ؽ�ó, ���� ����, �Լ� ȣ���� ��ü, �Լ� ������)
bool ParamUI::Param_TEXTURE(Ptr<HYTexture>& _Texture, const string& _Desc, UI* _Inst, Delegate_1 _Func)
{
	// �Ķ������ Description ���� ���
	ImGui::Text(_Desc.c_str());
	ImGui::SameLine();

	// Texture �̸� ��� InputText �� ID �����
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

	// Texture Drop üũ
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

	// �ؽ��� �̹��� ���
	static bool use_text_color_for_tint = false;
	ImVec2 uv_min = ImVec2(0.0f, 0.0f);
	ImVec2 uv_max = ImVec2(1.0f, 1.0f);
	ImVec4 tint_col = use_text_color_for_tint ? ImGui::GetStyleColorVec4(ImGuiCol_Text) : ImVec4(1.0f, 1.0f, 1.0f, 1.0f); // No tint
	ImVec4 border_col = ImGui::GetStyleColorVec4(ImGuiCol_Border);
	ImGui::Image(texid, ImVec2(150, 150), uv_min, uv_max, tint_col, border_col);

	// �Էµ� ��������Ʈ�� �ִٸ�
	if (_Inst && _Func)
	{
		ImGui::SameLine();
		sprintf_s(szID, "##TexBtn%d", g_ID++);
		if (ImGui::Button(szID, ImVec2(20, 20)))
		{
			// ����Ʈ UI
			ListUI* pListUI = (ListUI*)HYImGuiMgr::GetInst()->FindUI("##List");

			// AssetMgr�� ���� �ؽ�ó �̸� �޾ƿ���
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
