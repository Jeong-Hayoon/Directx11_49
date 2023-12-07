#include "pch.h"
#include "HYPathMgr.h"

wchar_t HYPathMgr::g_szContent[255] = {};


void HYPathMgr::init()
{
	// 실행파일 경로(작업 디렉터리)
	GetCurrentDirectory(255, g_szContent);

	size_t Len = wcslen(g_szContent);

	for (size_t i = Len - 1; i >= 0; --i)
	{
		if ('\\' == g_szContent[i])
		{
			g_szContent[i + 1] = '\0';
			break;
		}
	}

	wcscat_s(g_szContent, 255, L"content\\");
}
