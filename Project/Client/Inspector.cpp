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
#include "TilemapUI.h"
#include "ParticleSystemUI.h"

#include "AssetUI.h"

Inspector::Inspector()
	: UI("Inspector", "##Inspector")
	, m_TargetObject(nullptr)
	, m_arrComUI{}
{
	// 자식 UI 생성 및 자식 등록
	CreateChildUI();
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

	// m_TargetObject가 nullptr이 아닌 경우에만 
	if (nullptr != m_TargetObject)
	{
		string strName = string(m_TargetObject->GetName().begin(), m_TargetObject->GetName().end());
		ImGui::Text(strName.c_str());
	}

	ImGui::Text("Target Object"); ImGui::SameLine();

	// 입력으로 들어온 이름 출력(2byte wstring -> 1byte string으로 바꿔줌)
	string strName = string(m_TargetObject->GetName().begin(), m_TargetObject->GetName().end());
	//ImGui::Text(strName.c_str());

	HYLevel* pCurLevel = HYLevelMgr::GetInst()->GetCurrentLevel();
	vector<wstring> pObjectList = pCurLevel->GetObjectName();
	vector<string> Name = {};

	// 현재 레벨에 들어있는 오브젝트 이름 리스트를 순회하면서 Name 벡터에 string 형태로 넣어줌
	for (int i = 0; i < pObjectList.size(); i++)
	{
		Name.push_back(string(pObjectList[i].begin(), pObjectList[i].end()));
	}
	
	static int current_idx = 0;
	const string preview = ToString(GetTargetObject()->GetName());

	if (ImGui::BeginCombo("##", preview.c_str(), ImGuiComboFlags_None))
	{
		for (int i = 0; i < pObjectList.size(); ++i)
		{
			const bool is_selected = (current_idx == i);
			// 특정 이름은 선택하면
			if (ImGui::Selectable(Name[i].c_str(), is_selected))
			{
				// 현재 인덱스를 바꿔주고
				current_idx = i;
				// 타겟 오브젝트로 지정
				SetTargetObject(pCurLevel->FindObjectByName(pObjectList[i]));
			}

			if (is_selected)
				// 하이라이트 넣어주는 함수
				ImGui::SetItemDefaultFocus();
		}
		ImGui::EndCombo();
	}
}

// TargetObject 등록
void Inspector::SetTargetObject(HYGameObject* _Object)
{
	// Target 오브젝트 설정
	m_TargetObject = _Object;

	// 해당 오브젝트가 보유하고 있는 컴포넌트에 대응하는 컴포넌트UI 활성화
	// ComponentUI한테도 Target Object를 알려줌
	for (UINT i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
	{
		if (nullptr != m_arrComUI[i])
		{
			m_arrComUI[i]->SetTargetObject(_Object);
		}
	}

	// AssetUI 비활성화
	for (UINT i = 0; i < (UINT)ASSET_TYPE::END; ++i)
	{
		m_arrAssetUI[i]->Deactivate();
	}
}

// TargetAsset 등록
void Inspector::SetTargetAsset(Ptr<HYAsset> _Asset)
{
	// SetTargetAsset이 호출되면 일단 TargetObject를 해제시켜서 비활성화시켜야 함
	SetTargetObject(nullptr);

	m_TargetAsset = _Asset;

	// 모든 AssetUI를 우선 다 비활성화시키고
	for (UINT i = 0; i < (UINT)ASSET_TYPE::END; ++i)
	{
		m_arrAssetUI[i]->Deactivate();
	}

	// 입력으로 들어온 AssetUI만 활성화
	if (nullptr != m_TargetAsset)
	{
		m_arrAssetUI[(UINT)m_TargetAsset->GetType()]->Activate();
		m_arrAssetUI[(UINT)m_TargetAsset->GetType()]->SetAsset(_Asset);
	}
}


