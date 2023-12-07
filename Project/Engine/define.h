#pragma once

// friend�� ���س����� �ڽ� Ŭ�������� �����ڿ� �Ҹ��ڸ� ���ܳ��� ������ ��ü ������ �ȵ�
#define SINGLE(classtype)  	private:\
								classtype();\
								~classtype();\
							friend class HYSingleton<classtype>;

// ���Ǹ� ���� define 
#define DEVICE HYDevice::GetInst()->GetDevice()
#define CONTEXT HYDevice::GetInst()->GetContext()

// Ű�Ŵ��� ��ũ��
#define KEY_CHECK(Key, State) HYKeyMgr::GetInst()->GetKeyState(Key) == State
#define DT	HYTimeMgr::GetInst()->GetDeltaTime()

#define KEY_TAP(Key) KEY_CHECK(Key, TAP)
#define KEY_PRESSED(Key) KEY_CHECK(Key, PRESSED)
#define KEY_RELEASED(Key) KEY_CHECK(Key, RELEASED)
#define KEY_NONE(Key) KEY_CHECK(Key, NONE)