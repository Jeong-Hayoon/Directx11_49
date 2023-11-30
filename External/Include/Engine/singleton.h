#pragma once
// 싱글톤 패턴을 전역 객체가 아니라 Template로 상속해서 만듦
// 부모입장에서 상속받아간 자식을 알 수 있도록 
// 자기 자신을 가리키는 포인터를 멤버로 가짐
// 자식 클래스는 본인의 생성자와 소멸자는 private으로 숨겨야 함
template<typename T>
class HYSingleton
{
private:
	static T* m_This;					// 자기 자신

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
		// atexit가 반환타입 void고 인자가 void 타입을 요구
		typedef void(*FUNC_TYPE)(void);
		// atexit : 메인 함수 종료될 때 호출할 함수를 등록시키는 함수 
		// 등록시켜놓은 함수 포인터가 있으면 메인 함수 종료되면 호출시켜줌
		// Destroy를 신경쓸 필요가 없어짐
		// Destroy가 static이기 때문에 일반 전역함수 타입으로 캐스팅
		// 함수 포인터 적을 때는 & 붙여줘야 하고 ()는 빼야 함
		atexit((FUNC_TYPE)(&HYSingleton::Destroy));
	}
	~HYSingleton()
	{

	}
};

template<typename T>
T* HYSingleton<T>::m_This = nullptr;