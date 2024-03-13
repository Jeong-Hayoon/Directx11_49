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
	// �÷��̾ ��ó�� ������, Trace ���·� �����Ѵ�.
	float DetectRange = *((float*)GetBlackboardData(L"DetectRange"));

	HYGameObject* pTarget = ((HYGameObject*)GetBlackboardData(L"TargetObject"));

	HYGameObject* pSelf = GetFSM()->GetStateMachine()->GetOwner();

	// Ÿ�ٰ� �� ������ �Ÿ�
	Vec3 vDist = pTarget->Transform()->GetWorldPos() - pSelf->Transform()->GetWorldPos();

	// Ž�� ���� DrawDebug
	GamePlayStatic::DrawDebugCircle(pSelf->Transform()->GetWorldPos(), DetectRange, Vec3(0.1f, 1.f, 0.1f), 0.f);

	// �� ������Ʈ ������ �Ÿ��� Ž�� �������� �۾�����
	if (vDist.Length() <= DetectRange)
	{
		// �������·� �����Ѵ�.
		ChangeState(L"TraceState");
	}
}

void HYIdleState::Enter()
{
}

void HYIdleState::Exit()
{
}