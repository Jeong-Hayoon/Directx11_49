#include "pch.h"
#include "Animator2DUI.h"
#include "AnimationEditor.h"

#include <Engine/HYAnimator2D.h>

#include "HYImGuiMgr.h"
#include "ListUI.h"
#include "Inspector.h"

Animator2DUI::Animator2DUI()
	: ComponentUI("Animator2D", "##Animator2D", COMPONENT_TYPE::ANIMATOR2D)
    , m_bActive(true)
{
	SetSize(ImVec2(0.f, 120.f));
	SetComopnentTitle("Animator2D");
}

Animator2DUI::~Animator2DUI()
{
}


void Animator2DUI::render_update()
{
	ComponentUI::render_update();

    bool Active = m_bActive;

	HYGameObject* pTarget = GetTargetObject();

	ImGui::Text("Current Animation"); ImGui::SameLine();

	vector<string> vecAnimationName;

	pTarget->Animator2D()->GetAnimationName(vecAnimationName);

	vector<string> Name = {};

	for (int i = 0; i < vecAnimationName.size(); i++)
	{
		Name.push_back(string(vecAnimationName[i].begin(), vecAnimationName[i].end()));
	}

	static int current_idx = 0;

	if (ImGui::BeginCombo("##AnimationCb", ToString(pTarget->Animator2D()->GetCurAnimName()).c_str(), ImGuiComboFlags_None))
	{
		for (int i = 0; i < vecAnimationName.size(); ++i)
		{
			const bool is_selected = (current_idx == i);
			// 특정 이름은 선택하면
			if (ImGui::Selectable(Name[i].c_str(), is_selected))
			{
				// 현재 인덱스를 바꿔주고
				current_idx = i;
				GetTargetObject()->Animator2D()->Play(ToWString(Name[i]));
			}

			if (is_selected)
				// 하이라이트 넣어주는 함수
				ImGui::SetItemDefaultFocus();
		}
		ImGui::EndCombo();
	}

	ImGui::Text("Repeat ON/OFF");	ImGui::SameLine();

	bool Repeat = pTarget->Animator2D()->IsRepeat();

	if (ImGui::Checkbox("##RepeatBtn", &Repeat))
	{
		if (Repeat)
		{
			pTarget->Animator2D()->SetRepeat(false);
		}
		else
		{
			pTarget->Animator2D()->SetRepeat(true);
		}
	}


	ImGui::Text("Animator Edito Open");	ImGui::SameLine();

	if (ImGui::Button("##Animator Editor", ImVec2(20, 20)))
	{
		AnimationEditor* pAnimationEditor = (AnimationEditor*)HYImGuiMgr::GetInst()->FindUI("##AnimationEditor");
		pAnimationEditor->Activate();
	}
	
	//Vec3 offset = (Vec3)(pTarget->Animator2D()->GetOffset());

	//ImGui::Text("Offset"); ImGui::SameLine();  ImGui::DragFloat2("##Animation Offset", offset);

	//if(ImGui::InputFloat2)
}


void Animator2DUI::AnimationSelect(DWORD_PTR _ptr)
{
	string strAnimation= (char*)_ptr;
	wstring strAnimationName= ToWString(strAnimation);

	GetTargetObject()->Animator2D()->Play(strAnimationName);
}