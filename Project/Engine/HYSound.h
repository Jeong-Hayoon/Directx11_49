#pragma once
#include "HYAsset.h"

#include <FMOD/fmod.h>
#include <FMOD/fmod.hpp>
#include <FMOD/fmod_codec.h>

#ifdef _DEBUG
#pragma comment(lib, "FMOD/fmodL64_vc.lib")
#else
#pragma comment(lib, "FMOD/fmod64_vc.lib")
#endif


class HYSound :
	public HYAsset
{
public:
	static FMOD::System*		g_pFMOD;		// FMOD 관리자 클래스

public:
	FMOD::Sound*				m_pSound;       // 로딩된 사운드 객체(소리 하나를 저장)
	list<FMOD::Channel*>		m_listChannel;  // 사운드가 재생되고 있는 채널(동시에 여러 개의 사운드가 송출될 때), 어느 채널에서 본인이 재생되고 있는지 기록

public:
	int Play(int _iRoopCount, float _fVolume = 1.f, bool _bOverlap = false);
	void Stop();

	// 0 ~ 1
	void SetVolume(float _f, int _iChannelIdx);

private:
	void RemoveChannel(FMOD::Channel* _pTargetChannel);

	friend FMOD_RESULT CHANNEL_CALLBACK(FMOD_CHANNELCONTROL* channelcontrol, FMOD_CHANNELCONTROL_TYPE controltype
		, FMOD_CHANNELCONTROL_CALLBACK_TYPE callbacktype
		, void* commanddata1, void* commanddata2);

public:
	virtual void UpdateData() {}

	// 파일로 저장
	virtual int Save(const wstring&) override { return S_OK; }

	// 파일로부터 로딩
	virtual int Load(const wstring& _strFilePath) override;

public:
	CLONE_DISABLE(HYSound);

	HYSound(bool _Engine = false);
	~HYSound();
};


