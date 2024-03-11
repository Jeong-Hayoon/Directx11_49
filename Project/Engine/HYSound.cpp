#include "pch.h"
#include "HYSound.h"

// CallBack 함수(전역 함수 O, 멤버 함수 X) 전방선언
FMOD_RESULT CHANNEL_CALLBACK(FMOD_CHANNELCONTROL* channelcontrol, FMOD_CHANNELCONTROL_TYPE controltype
	, FMOD_CHANNELCONTROL_CALLBACK_TYPE callbacktype
	, void* commanddata1, void* commanddata2);

FMOD::System* HYSound::g_pFMOD = nullptr;

HYSound::HYSound(bool _Engine)
	: HYAsset(ASSET_TYPE::SOUND, _Engine)
	, m_pSound(nullptr)
{
}

HYSound::~HYSound()
{
	if (nullptr != m_pSound)
	{
		m_pSound->release();
		m_pSound = nullptr;
	}
}

// _iRoopCount : 0 (무한반복),  _fVolume : 0 ~ 1(Volume), _bOverlap : 소리 중첩 가능여부
int HYSound::Play(int _iRoopCount, float _fVolume, bool _bOverlap)
{
	if (_iRoopCount <= -1)
	{
		assert(nullptr);
	}

	// 재생되고 있는 채널이 이미 있는데, 중복재생을 허용하지 않았다 -> 재생 안함
	if (!_bOverlap && !m_listChannel.empty())
	{
		return E_FAIL;
	}

	// 실제 FMOD는 -1을 넣어줘야 무한 루프인데 직관적이지 않아서 0을 무한 루프, 1을 1번 재생으로 하기 위해 -1
	_iRoopCount -= 1;

	FMOD::Channel* pChannel = nullptr;
	// pChannel를 이중 포인터로 받은 이유 : 어느 채널에서 재생되는지 알려줘야 하기 때문
	g_pFMOD->playSound(m_pSound, nullptr, false, &pChannel);

	// 재생 실패(E_FAIL : 음수(0보다 작으면 실패))
	if (nullptr == pChannel)
		return E_FAIL;

	pChannel->setVolume(_fVolume);

	pChannel->setCallback(CHANNEL_CALLBACK);
	pChannel->setUserData(this);

	pChannel->setMode(FMOD_LOOP_NORMAL);
	pChannel->setLoopCount(_iRoopCount);

	m_listChannel.push_back(pChannel);

	// 몇 번 채널에서 재생되는지 인덱스값을 return
	int iIdx = -1;
	pChannel->getIndex(&iIdx);

	return iIdx;
}

void HYSound::Stop()
{
	list<FMOD::Channel*>::iterator iter;

	while (!m_listChannel.empty())
	{
		iter = m_listChannel.begin();
		(*iter)->stop();
	}
}

// 사운드 재생이 끝난 후 채널 리스트 목록에서 제거하는 함수
void HYSound::RemoveChannel(FMOD::Channel* _pTargetChannel)
{
	list<FMOD::Channel*>::iterator iter = m_listChannel.begin();
	for (; iter != m_listChannel.end(); ++iter)
	{
		if (*iter == _pTargetChannel)
		{
			m_listChannel.erase(iter);
			return;
		}
	}
}

// 해당 채널 인덱스에 접근해서 볼륨 설정
void HYSound::SetVolume(float _f, int _iChannelIdx)
{
	list<FMOD::Channel*>::iterator iter = m_listChannel.begin();

	int iIdx = -1;
	for (; iter != m_listChannel.end(); ++iter)
	{
		(*iter)->getIndex(&iIdx);
		if (_iChannelIdx == iIdx)
		{
			(*iter)->setVolume(_f);
			return;
		}
	}
}

int HYSound::Load(const wstring& _strFilePath)
{
	string path(_strFilePath.begin(), _strFilePath.end());

	if (FMOD_OK != g_pFMOD->createSound(path.c_str(), FMOD_DEFAULT, nullptr, &m_pSound))
	{
		assert(nullptr);
	}

	return S_OK;
}

// =============================================================================
// Call Back - 사운드가 재생될 때 하고 싶은 일이 있으면 그 함수의 주소를 알려줌
// =============================================================================
FMOD_RESULT CHANNEL_CALLBACK(FMOD_CHANNELCONTROL* channelcontrol, FMOD_CHANNELCONTROL_TYPE controltype
	, FMOD_CHANNELCONTROL_CALLBACK_TYPE callbacktype
	, void* commanddata1, void* commanddata2)
{
	FMOD::Channel* cppchannel = (FMOD::Channel*)channelcontrol;
	HYSound* pSound = nullptr;

	switch (controltype)
	{
		// 채널에서 재생 중인 사운드가 종료되는 시점
	case FMOD_CHANNELCONTROL_CALLBACK_END:
	{
		// 사운드 정보를 받아와서 채널에서 remove
		cppchannel->getUserData((void**)&pSound);
		pSound->RemoveChannel(cppchannel);
	}
	break;
	}

	return FMOD_OK;
}