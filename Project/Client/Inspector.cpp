#include "pch.h"
#include "Inspector.h"

#include <Engine/HYTransform.h>
#include <Engine/HYLevelMgr.h>
#include <Engine/HYLevel.h>
#include <Engine/HYGameObject.h>



#include "TransformUI.h"
#include "MeshRenderUI.h"
#include "Collider2DUI.h"
#include "Light2DUI.h"
#include "Animator2DUI.h"
#include "CameraUI.h"

Inspector::Inspector()
	: UI("Inspector", "##Inspector")
	, m_TargetObject(nullptr)\
	, m_arrComUI{}
{
	// 자식 UI 생성 및 자식 등록
	m_arrComUI[(UINT)COMPONENT_TYPE::TRANSFORM] = new TransformUI;
	AddChildUI(m_arrComUI[(UINT)COMPONENT_TYPE::TRANSFORM]);

	m_arrComUI[(UINT)COMPONENT_TYPE::MESHRENDER] = new MeshRenderUI;
	AddChildUI(m_arrComUI[(UINT)COMPONENT_TYPE::MESHRENDER]);

	m_arrComUI[(UINT)COMPONENT_TYPE::COLLIDER2D] = new Collider2DUI;
	AddChildUI(m_arrComUI[(UINT)COMPONENT_TYPE::COLLIDER2D]);

	m_arrComUI[(UINT)COMPONENT_TYPE::LIGHT2D] = new Light2DUI;
	AddChildUI(m_arrComUI[(UINT)COMPONENT_TYPE::LIGHT2D]);

	m_arrComUI[(UINT)COMPONENT_TYPE::ANIMATOR2D] = new Animator2DUI;
	AddChildUI(m_arrComUI[(UINT)COMPONENT_TYPE::ANIMATOR2D]);

	m_arrComUI[(UINT)COMPONENT_TYPE::CAMERA] = new CameraUI;
	AddChildUI(m_arrComUI[(UINT)COMPONENT_TYPE::CAMERA]);
}

Inspector::~Inspector()
{
}


void Inspector::tick()
{
}

void Inspector::render_update()
{
	if (nullptr == m_TargetObject)
		return;

	ImGui::Text("Target Object"); ImGui::SameLine();

	// 입력으로 들어온 이름 출력(2byte wstring -> 1byte string으로 바꿔줌)
	string strName = string(m_TargetObject->GetName().begin(), m_TargetObject->GetName().end());
	//ImGui::Text(strName.c_str());

	HYLevel* pCurLevel = HYLevelMgr::GetInst()->GetCurrentLevel();
	vector<wstring> pObjectList = pCurLevel->GetObjectName();
	vector<string> Name = {};

	for (int i = 0; i < pObjectList.size(); i++)
	{
		Name.push_back(string(pObjectList[i].begin(), pObjectList[i].end()));
	}

	static int current_idx = 0;
	const string preview = Name[current_idx];

	if (ImGui::BeginCombo("##", preview.c_str(), ImGuiComboFlags_None))
	{
		for (int i = 0; i < pObjectList.size(); ++i)
		{
			const bool is_selected = (current_idx == i);
			if (ImGui::Selectable(Name[i].c_str(), is_selected))
			{
				current_idx = i;
				SetTargetObject(pCurLevel->FindObjectByName(pObjectList[i]));
			}

			if (is_selected)
				ImGui::SetItemDefaultFocus();
		}
		ImGui::EndCombo();
	}
}

// TargetObject 등록
void Inspector::SetTargetObject(HYGameObject* _Object)
{
	m_TargetObject = _Object;

	// ComponentUI한테도 Target Object를 알려줌
	for (UINT i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
	{
		if (nullptr != m_arrComUI[i])
		{
			m_arrComUI[i]->SetTargetObject(_Object);
		}
	}
}
// TargetAsset 등록
void Inspector::SetTargetAsset(Ptr<HYAsset> _Asset)
{
	m_TargetAsset = _Asset;
}


