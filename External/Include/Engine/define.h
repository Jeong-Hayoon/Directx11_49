#pragma once

// friend�� ���س����� �ڽ� Ŭ�������� �����ڿ� �Ҹ��ڸ� ���ܳ��� ������ ��ü ������ �ȵ�
#define SINGLE(classtype)  	private:\
								classtype();\
								~classtype();\
							friend class HYSingleton<classtype>;

// ���Ǹ� ���� define 
#define DEVICE HYDevice::GetInst()->GetDevice()
#define CONTEXT HYDevice::GetInst()->GetContext()