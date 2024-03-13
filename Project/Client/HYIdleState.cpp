#include "pch.h"
#include "HYIdleState.h"

#include <Engine/HYGameObject.h>
#include <Engine/HYStateMachine.h>
#include <Engine/HYTransform.h>

HYIdleState::HYIdleState()
{
}

HYIdleState::~HYIdleState()
{
}

void HYIdleState::finaltick()
{
	// 플레이어가 근처에 있으면, Trace 상태로 변경한다.
	float DetectRange = *((float*)GetBlackboardData(L"DetectRange"));

	HYGameObject* pTarget = ((HYGameObject*)GetBlackboardData(L"TargetObject"));

	HYGameObject* pSelf = GetFSM()->GetStateMachine()->GetOwner();

	// 타겟과 나 사이의 거리
	Vec3 vDist = pTarget->Transform()->GetWorldPos() - pSelf->Transform()->GetWorldPos();

	// 탐지 범위 DrawDebug
	GamePlayStatic::DrawDebugCircle(pSelf->Transform()->GetWorldPos(), DetectRange, Vec3(0.1f, 1.f, 0.1f), 0.f);

	// 두 오브젝트 사이의 거리가 탐지 범위보다 작아지면
	if (vDist.Length() <= DetectRange)
	{
		// 추적상태로 변경한다.
		ChangeState(L"TraceState");
	}
}

void HYIdleState::Enter()
{
}

void HYIdleState::Exit()
{
}