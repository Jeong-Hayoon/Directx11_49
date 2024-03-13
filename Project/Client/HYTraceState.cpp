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
	// 플레이어가 근처에 있으면, Trace 상태로 변경한다.	
	float Speed = *((float*)GetBlackboardData(L"Speed"));
	HYGameObject* pTarget = ((HYGameObject*)GetBlackboardData(L"TargetObject"));
	HYGameObject* pSelf = GetFSM()->GetStateMachine()->GetOwner();

	// 상대방을 향한 방향
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