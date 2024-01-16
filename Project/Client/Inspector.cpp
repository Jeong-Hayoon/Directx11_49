#include "pch.h"
#include "Inspector.h"

#include <Engine/HYTransform.h>

#include "TransformUI.h"

Inspector::Inspector()
	: UI("Inspector", "##Inspector")
	, m_TargetObject(nullptr)
{
	// �ڽ� UI ����
	m_TransformUI = new TransformUI;
	// �ڽ� ���
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

	// �Է����� ���� �̸� ���(2byte wstring -> 1byte string���� �ٲ���)
	string strName = string(m_TargetObject->GetName().begin(), m_TargetObject->GetName().end());
	ImGui::Text(strName.c_str());
}

// TargetObject ���
void Inspector::SetTargetObject(HYGameObject* _Object)
{
	m_TargetObject = _Object;

	// TransformUI���׵� Target Object�� �˷���
	m_TransformUI->SetTargetObject(_Object);
}
// TargetAsset ���
void Inspector::SetTargetAsset(Ptr<HYAsset> _Asset)
{
	m_TargetAsset = _Asset;
}
