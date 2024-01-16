#include "pch.h"
#include "Inspector.h"

#include <Engine/HYTransform.h>

#include "TransformUI.h"

Inspector::Inspector()
	: UI("Inspector", "##Inspector")
	, m_TargetObject(nullptr)
{
	// 자식 UI 생성
	m_TransformUI = new TransformUI;
	// 자식 등록
	AddChildUI(m_TransformUI);
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

	// 입력으로 들어온 이름 출력(2byte wstring -> 1byte string으로 바꿔줌)
	string strName = string(m_TargetObject->GetName().begin(), m_TargetObject->GetName().end());
	ImGui::Text(strName.c_str());
}

// TargetObject 등록
void Inspector::SetTargetObject(HYGameObject* _Object)
{
	m_TargetObject = _Object;

	// TransformUI한테도 Target Object를 알려줌
	m_TransformUI->SetTargetObject(_Object);
}
// TargetAsset 등록
void Inspector::SetTargetAsset(Ptr<HYAsset> _Asset)
{
	m_TargetAsset = _Asset;
}
