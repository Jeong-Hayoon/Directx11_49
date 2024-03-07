#include "pch.h"
#include "HYPathMgr.h"

wchar_t HYPathMgr::g_szContent[255] = {};


void HYPathMgr::init()
{
	// 실행파일 경로(작업 디렉터리)
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

// 최종경로에서 상대경로를 추출해주는 함수
wstring HYPathMgr::GetRelativePath(const wstring& _FullPath)
{
	wstring strContentPath = GetContentPath();

	// 입력된 경로(최종경로)에, content 경로가 없으면 실패(-1이면 실패)
	if (-1 == _FullPath.find(strContentPath, 0))
		return nullptr;

	// Content 경로 끝에서 문자열 끝까지를 잘라서 반환(substr - 본인이 보유하고 있는 문자열을 잘라내는 함수(잘라낼 위치, 잘라낼 길이))
	wstring strRelativePath = _FullPath.substr(strContentPath.length(), _FullPath.length());

	return strRelativePath;
}