#pragma once

// friend로 안해놓으면 자식 클래스들은 생성자와 소멸자를 숨겨놨기 때문에 객체 생성이 안됨
#define SINGLE(classtype)  	private:\
								classtype();\
								~classtype();\
							friend class HYSingleton<classtype>;

// 편의를 위해 define 
#define DEVICE HYDevice::GetInst()->GetDevice()
#define CONTEXT HYDevice::GetInst()->GetContext()