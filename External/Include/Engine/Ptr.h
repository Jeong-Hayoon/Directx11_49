#pragma once

// 스마트 포인터 Comptr처럼 적용될 수 있도록 하는 Asset 전용 포인터
template<typename T>
class Ptr
{
private:
	T* Asset; 

public:
	T* Get() const { return Asset; }
	// 멤버 변수의 주소를 줘야할 때
	T** GetAdressOf() const { return &Asset; }
	T* operator->() const { return Asset; }

public:
	// 데이터를 받고 나면 자기 자신을 반환
	Ptr<T>& operator = (const Ptr& _ptr)
	{
		if (nullptr != Asset)
			Asset->Release();

		Asset = _ptr.Asset;

		if (nullptr != Asset)
			Asset->AddRef();

		return *this;
	}

	// 데이터를 받고 나면 자기 자신을 반환
	Ptr<T>& operator = (T* _Asset)
	{
		if (nullptr != Asset)
			Asset->Release();

		Asset = _Asset;

		if (nullptr != Asset)
			Asset->AddRef();

		return *this;
	}

	bool operator ==(const Ptr<T>& _Other) const
	{
		if (Asset == _Other.Asset)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	bool operator != (const Ptr<T>& _Other) const
	{
		return !(*this == _Other);
	}

	bool operator ==(const T* _Asset) const
	{
		if (Asset == _Asset)
			return true;
		else
			return false;
	}

	bool operator != (const T* _Asset) const
	{
		return !(*this == _Asset);
	}

public:
	Ptr()
		: Asset(nullptr)
	{}
	
	// 스마트 포인터끼리 대입되는 상황에서의 생성자
	Ptr(const Ptr<T>& _Other)
		:Asset(_Other.Asset)
	{
		if (nullptr != Asset)
			Asset->AddRef();
	}


	// 다른 Asset을 가리키면서 생성
	Ptr(T* _Asset)
		: Asset(_Asset)
	{
		if (nullptr != Asset)
			Asset->AddRef();
	}

	// 스마트 포인터 소멸 시 가리키고 있던 Asset이 있을 경우 Release
	~Ptr()
	{
		if (Asset)
		{
			Asset->Release();
		}
	}
};

// nullptr을 왼편에 놓고 비교할 수 있도록 전역함수로
// 일반 멤버함수로 오퍼레이터를 두면 nullptr을 왼편에 둘 수 없음
template<typename T>
bool operator == (const void* _Asset, const Ptr<T>& _Ptr)
{
	if (_Asset == _Ptr.Get())
		return true;
	else
		return false;
}

template<typename T>
bool operator != (const void* _Asset, const Ptr<T>& _Ptr)
{
	return !(_Asset == _Ptr);
}