#include "pch.h"
#include "HYTraceState.h"

#include <Engine/HYGameObject.h>
#include <Engine/HYStateMachine.h>
#include <Engine/HYTransform.h>
				 
#include <Engine/HYTimeMgr.h>

HYTraceState::HYTraceState()
{
}

HYTraceState::~HYTraceState()
{
}

void HYTraceState::finaltick()
{
	// �÷��̾ ��ó�� ������, Trace ���·� �����Ѵ�.	
	float Speed = *((float*)GetBlackboardData(L"Speed"));
	HYGameObject* pTarget = ((HYGameObject*)GetBlackboardData(L"TargetObject"));
	HYGameObject* pSelf = GetFSM()->GetStateMachine()->GetOwner();

	// ������ ���� ����
	Vec3 vDir = pTarget->Transform()->GetWorldPos() - pSelf->Transform()->GetWorldPos();
	vDir.Normalize();

	Vec3 vPos = pSelf->Transform()->GetWorldPos() + vDir * DT * Speed;
	pSelf->Transform()->SetRelativePos(vPos);
}

void HYTraceState::Enter()
{
}

void HYTraceState::Exit()
{
}