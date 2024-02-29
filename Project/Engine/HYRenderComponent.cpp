#include "pch.h"
#include "HYRenderComponent.h"

#include "HYLevelMgr.h"
#include "HYLevel.h"

HYRenderComponent::HYRenderComponent(COMPONENT_TYPE _Type)
	: HYComponent(_Type)
{
}

HYRenderComponent::HYRenderComponent(const HYRenderComponent& _OriginRenderCom)
	: HYComponent(_OriginRenderCom)
	, m_Mesh(_OriginRenderCom.m_Mesh)
	, m_SharedMtrl(_OriginRenderCom.m_SharedMtrl)
{
	// 원본 오브젝트가 공유재질을 참조하고 있고, 현재 사용재질은 공유재질이 아닌경우
	if (_OriginRenderCom.m_SharedMtrl != _OriginRenderCom.m_CurMtrl)
	{
		assert(_OriginRenderCom.m_DynamicMtrl.Get());

		// 복사 렌더 컴포넌트도 별도의 동적재질을 생성한다.
		GetDynamicMaterial();

		// 원본 렌더컴포넌트의 동적재질 값을 현재 생성한 동적재질로 복사한다.(파라미터 대입)
		*m_DynamicMtrl.Get() = *_OriginRenderCom.m_DynamicMtrl.Get();
	}
	else
	{
		m_CurMtrl = m_SharedMtrl;
	}
}

HYRenderComponent::~HYRenderComponent()
{
	// 원래라면 AssetMgr에 등록되지 않는 동적 재질을 지워줘야 하지만 Ptr이기 때문에 신경쓰지 않아도 됨
}

// 아예 재질을 교체하는 함수 -> 동적 재질을 초기화 시켜야 함
void HYRenderComponent::SetMaterial(Ptr<HYMaterial> _Mtrl)
{
	// 재질이 변경되면 기존에 복사본 받아둔 DynamicMaterial 을 삭제한다.
	m_CurMtrl = m_SharedMtrl = _Mtrl;
	m_DynamicMtrl = nullptr;
}

// 동적 재질을 반환해주는 함수(동적 재질은 특정 오브젝트만의 재질)
Ptr<HYMaterial> HYRenderComponent::GetDynamicMaterial()
{
	// 레벨이 플레이 상태인 경우에만
	HYLevel* pCurLevel = HYLevelMgr::GetInst()->GetCurrentLevel();
	if (pCurLevel->GetState() != LEVEL_STATE::PLAY)
		return nullptr;

	// 이미 동적 재질을 보유하고 있으면 그걸 준다.
	if (nullptr != m_DynamicMtrl)
		return m_DynamicMtrl;

	// 동적 재질이 없고 공유 재질이 있다면
	if (nullptr != m_SharedMtrl)
	{
		// 공유 재질을 복사해서 동적재질을 만들고 그걸 현재 사용 재질로 설정한다.
		m_CurMtrl = m_DynamicMtrl = m_SharedMtrl->Clone();

		return m_DynamicMtrl;
	}

	return nullptr;
}

// 원래 재질로 돌아가도록 해주는 함수
void HYRenderComponent::RestoreMaterial()
{
	m_CurMtrl = m_SharedMtrl;
}

void HYRenderComponent::SaveToFile(FILE* _File)
{
	SaveAssetRef(m_Mesh, _File);
	SaveAssetRef(m_SharedMtrl, _File);
}

void HYRenderComponent::LoadFromFile(FILE* _File)
{
	LoadAssetRef(m_Mesh, _File);
	LoadAssetRef(m_SharedMtrl, _File);

	SetMaterial(m_SharedMtrl);
}

