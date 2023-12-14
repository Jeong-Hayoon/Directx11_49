#include "pch.h"
#include "HYTransform.h"

#include "HYDevice.h"
#include "HYConstBuffer.h"

HYTransform::HYTransform()
	: HYComponent(COMPONENT_TYPE::TRANSFORM)
	, m_vRelativeScale(Vec3(1.f, 1.f, 1.f))
{
}

HYTransform::~HYTransform()
{
}

void HYTransform::finaltick()
{
}

// Transform에 저장되어 있던 위치정보를 
// 상수버퍼에 담아서 레지스터로 보냄
// (여기에서 세팅된 위치정보를 기반으로 쉐이더가 동작)
// Device가 관리하고 있는 여러 상수버퍼 중
// 위치값을 전달받을 수 있는 transform 전용 상수버퍼가 있고
// Device를 통해 얻어내서 위치데이터를 보내놓음
// 랜더링하기 전에 해당 상수버퍼를 b0 레지스터에 바인딩을 할 것임
void HYTransform::UpdateData()
{
	tTransform transform = {};
	transform.vWorldPos = m_vRelativePos;
	transform.vWorldScale = m_vRelativeScale;

	// 위치정보를 Transform 상수버퍼에 보내고, B0 레지스터에 바인딩 해둠
	HYConstBuffer* pCB = HYDevice::GetInst()->GetConstBuffer(CB_TYPE::TRANSFORM);
	
	// _ElementCount는 1개이긴 한데 굳이 입력 안해도 상수버퍼는 
	// Transform * 1개로 만들었기 때문에 Default 인자 0이 넘어가도
	// 알아서 1로 인식(상수버퍼 크기가 Transform 구조체 1개 크기이므로)
	pCB->SetData(&transform);
	pCB->UpdateData(0);
}