#include "pch.h"
#include "AssetUI.h"

AssetUI::AssetUI(const string& _strName, const string& _ID, ASSET_TYPE _Type)
	: UI(_strName, _ID)
	, m_Type(_Type)
{
	Deactivate();

}

AssetUI::~AssetUI()
{
}

void AssetUI::render_update()
{
	ImGui::PushID(0);
	ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(0.6f, 0.6f, 0.6f));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(0.6f, 0.6f, 0.6f));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(0.6f, 0.6f, 0.6f));

	// Title명은 Asset 타입으로
	ImGui::Button(ASSET_TYPE_STRING[(UINT)m_Type]);

	ImGui::PopStyleColor(3);
	ImGui::PopID();
}



void AssetUI::SetAsset(Ptr<HYAsset> _Asset)
{
	m_Asset = _Asset;

	// 입력된 Asset이 nullptr이면 해당 AssetUI를 비활성화한다.
	if (nullptr == m_Asset)
	{
		Deactivate();
	}

	else
	{
		// 정상적인 Asset 주소가 들어온 경우, 이 AssetUI 의 타입과 실제 Asset의 타입이 일치하면
		// 해당 AssetUI 를 활성화 시키고 아니면 비활성화시킨다.
		if (_Asset->GetType() == m_Type)
		{
			Activate();
		}
		else
		{
			Deactivate();
		}
	}
}
