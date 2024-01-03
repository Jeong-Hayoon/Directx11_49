#pragma once

// ����Ʈ ������ Comptró�� ����� �� �ֵ��� �ϴ� Asset ���� ������
template<typename T>
class Ptr
{
private:
	T* Asset; 

public:
	T* Get() const { return Asset; }
	// ��� ������ �ּҸ� ����� ��
	T** GetAdressOf() const { return &Asset; }
	T* operator->() const { return Asset; }


public:
	void operator = (const Ptr& _ptr)
	{
		if (nullptr != Asset)
			Asset->Release();

		Asset = _ptr.Asset;

		if (nullptr != Asset)
			Asset->AddRef();
	}

	void operator = (T* _Asset)
	{
		if (nullptr != Asset)
			Asset->Release();

		Asset = _Asset;

		if (nullptr != Asset)
			Asset->AddRef();
	}

	bool operator ==(const Ptr<T>& _Other)
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

	bool operator != (const Ptr<T>& _Other)
	{
		return !(*this == _Other);
	}

	bool operator ==(const T* _Asset)
	{
		if (Asset == _Asset)
			return true;
		else
			return false;
	}

	bool operator != (const T* _Asset)
	{
		return !(*this == _Asset);
	}


public:
	Ptr()
		: Asset(nullptr)
	{}
	
	// ����Ʈ �����ͳ��� ���ԵǴ� ��Ȳ������ ������
	Ptr(const Ptr<T>& _Other)
		:Asset(_Other.Asset)
	{
		if (nullptr != Asset)
			Asset->AddRef();
	}


	// �ٸ� Asset�� ����Ű�鼭 ����
	Ptr(T* _Asset)
		: Asset(_Asset)
	{
		if (nullptr != Asset)
			Asset->AddRef();
	}

	// ����Ʈ ������ �Ҹ� �� ����Ű�� �ִ� Asset�� ���� ��� Release
	~Ptr()
	{
		if (Asset)
		{
			Asset->Release();
		}
	}
};

// nullptr�� ���� ���� ���� �� �ֵ��� �����Լ���
// �Ϲ� ����Լ��� ���۷����͸� �θ� nullptr�� ���� �� �� ����
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