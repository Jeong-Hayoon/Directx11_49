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
	// �ڽ� UI ���� �� �ڽ� ���
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

	// m_TargetObject�� nullptr�� �ƴ� ��쿡�� 
	if (nullptr != m_TargetObject)
	{
		string strName = string(m_TargetObject->GetName().begin(), m_TargetObject->GetName().end());
		ImGui::Text(strName.c_str());
	}

	ImGui::Text("Target Object"); ImGui::SameLine();

	// �Է����� ���� �̸� ���(2byte wstring -> 1byte string���� �ٲ���)
	string strName = string(m_TargetObject->GetName().begin(), m_TargetObject->GetName().end());
	//ImGui::Text(strName.c_str());

	HYLevel* pCurLevel = HYLevelMgr::GetInst()->GetCurrentLevel();
	vector<wstring> pObjectList = pCurLevel->GetObjectName();
	vector<string> Name = {};

	// ���� ������ ����ִ� ������Ʈ �̸� ����Ʈ�� ��ȸ�ϸ鼭 Name ���Ϳ� string ���·� �־���
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
			// Ư�� �̸��� �����ϸ�
			if (ImGui::Selectable(Name[i].c_str(), is_selected))
			{
				// ���� �ε����� �ٲ��ְ�
				current_idx = i;
				// Ÿ�� ������Ʈ�� ����
				SetTargetObject(pCurLevel->FindObjectByName(pObjectList[i]));
			}

			if (is_selected)
				// ���̶���Ʈ �־��ִ� �Լ�
				ImGui::SetItemDefaultFocus();
		}
		ImGui::EndCombo();
	}
}

// TargetObject ���
void Inspector::SetTargetObject(HYGameObject* _Object)
{
	// Target ������Ʈ ����
	m_TargetObject = _Object;

	// �ش� ������Ʈ�� �����ϰ� �ִ� ������Ʈ�� �����ϴ� ������ƮUI Ȱ��ȭ
	// ComponentUI���׵� Target Object�� �˷���
	for (UINT i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
	{
		if (nullptr != m_arrComUI[i])
		{
			m_arrComUI[i]->SetTargetObject(_Object);
		}
	}

	// AssetUI ��Ȱ��ȭ
	for (UINT i = 0; i < (UINT)ASSET_TYPE::END; ++i)
	{
		m_arrAssetUI[i]->Deactivate();
	}
}

// TargetAsset ���
void Inspector::SetTargetAsset(Ptr<HYAsset> _Asset)
{
	// SetTargetAsset�� ȣ��Ǹ� �ϴ� TargetObject�� �������Ѽ� ��Ȱ��ȭ���Ѿ� ��
	SetTargetObject(nullptr);

	m_TargetAsset = _Asset;

	// ��� AssetUI�� �켱 �� ��Ȱ��ȭ��Ű��
	for (UINT i = 0; i < (UINT)ASSET_TYPE::END; ++i)
	{
		m_arrAssetUI[i]->Deactivate();
	}

	// �Է����� ���� AssetUI�� Ȱ��ȭ
	if (nullptr != m_TargetAsset)
	{
		m_arrAssetUI[(UINT)m_TargetAsset->GetType()]->Activate();
		m_arrAssetUI[(UINT)m_TargetAsset->GetType()]->SetAsset(_Asset);
	}
}


