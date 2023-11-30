#pragma once
// �̱��� ������ ���� ��ü�� �ƴ϶� Template�� ����ؼ� ����
// �θ����忡�� ��ӹ޾ư� �ڽ��� �� �� �ֵ��� 
// �ڱ� �ڽ��� ����Ű�� �����͸� ����� ����
// �ڽ� Ŭ������ ������ �����ڿ� �Ҹ��ڴ� private���� ���ܾ� ��
template<typename T>
class HYSingleton
{
private:
	static T* m_This;					// �ڱ� �ڽ�

public:
	static T* GetInst()
	{
		if (nullptr == m_This)			
		{
			m_This = new T;
		}

		return m_This;
	}

	static void Destroy()
	{
		if (nullptr != m_This)
		{
			delete m_This;
			m_This = nullptr;
		}
	}

public:
	HYSingleton()
	{
		// atexit�� ��ȯŸ�� void�� ���ڰ� void Ÿ���� �䱸
		typedef void(*FUNC_TYPE)(void);
		// atexit : ���� �Լ� ����� �� ȣ���� �Լ��� ��Ͻ�Ű�� �Լ� 
		// ��Ͻ��ѳ��� �Լ� �����Ͱ� ������ ���� �Լ� ����Ǹ� ȣ�������
		// Destroy�� �Ű澵 �ʿ䰡 ������
		// Destroy�� static�̱� ������ �Ϲ� �����Լ� Ÿ������ ĳ����
		// �Լ� ������ ���� ���� & �ٿ���� �ϰ� ()�� ���� ��
		atexit((FUNC_TYPE)(&HYSingleton::Destroy));
	}
	~HYSingleton()
	{

	}
};

template<typename T>
T* HYSingleton<T>::m_This = nullptr;