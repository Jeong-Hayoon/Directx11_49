#include "pch.h"
#include "HYPathMgr.h"

wchar_t HYPathMgr::g_szContent[255] = {};


void HYPathMgr::init()
{
	// �������� ���(�۾� ���͸�)
	GetCurrentDirectory(255, g_szContent);

	size_t Len = wcslen(g_szContent);

	for(size_t i = Len - 1; i >= 0; --i)
	{
		if ('\\' == g_szContent[i])
		{
			g_szContent[i + 1] = '\0';
			break;
		}
	}

	wcscat_s(g_szContent, 255, L"content\\");
}

// ������ο��� ����θ� �������ִ� �Լ�
wstring HYPathMgr::GetRelativePath(const wstring& _FullPath)
{
	wstring strContentPath = GetContentPath();

	// �Էµ� ���(�������)��, content ��ΰ� ������ ����(-1�̸� ����)
	if (-1 == _FullPath.find(strContentPath, 0))
		return nullptr;

	// Content ��� ������ ���ڿ� �������� �߶� ��ȯ(substr - ������ �����ϰ� �ִ� ���ڿ��� �߶󳻴� �Լ�(�߶� ��ġ, �߶� ����))
	wstring strRelativePath = _FullPath.substr(strContentPath.length(), _FullPath.length());

	return strRelativePath;
}