#include "pch.h"
#include "HYSound.h"

// CallBack �Լ�(���� �Լ� O, ��� �Լ� X) ���漱��
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

// _iRoopCount : 0 (���ѹݺ�),  _fVolume : 0 ~ 1(Volume), _bOverlap : �Ҹ� ��ø ���ɿ���
int HYSound::Play(int _iRoopCount, float _fVolume, bool _bOverlap)
{
	if (_iRoopCount <= -1)
	{
		assert(nullptr);
	}

	// ����ǰ� �ִ� ä���� �̹� �ִµ�, �ߺ������ ������� �ʾҴ� -> ��� ����
	if (!_bOverlap && !m_listChannel.empty())
	{
		return E_FAIL;
	}

	// ���� FMOD�� -1�� �־���� ���� �����ε� ���������� �ʾƼ� 0�� ���� ����, 1�� 1�� ������� �ϱ� ���� -1
	_iRoopCount -= 1;

	FMOD::Channel* pChannel = nullptr;
	// pChannel�� ���� �����ͷ� ���� ���� : ��� ä�ο��� ����Ǵ��� �˷���� �ϱ� ����
	g_pFMOD->playSound(m_pSound, nullptr, false, &pChannel);

	// ��� ����(E_FAIL : ����(0���� ������ ����))
	if (nullptr == pChannel)
		return E_FAIL;

	pChannel->setVolume(_fVolume);

	pChannel->setCallback(CHANNEL_CALLBACK);
	pChannel->setUserData(this);

	pChannel->setMode(FMOD_LOOP_NORMAL);
	pChannel->setLoopCount(_iRoopCount);

	m_listChannel.push_back(pChannel);

	// �� �� ä�ο��� ����Ǵ��� �ε������� return
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

// ���� ����� ���� �� ä�� ����Ʈ ��Ͽ��� �����ϴ� �Լ�
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

// �ش� ä�� �ε����� �����ؼ� ���� ����
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
// Call Back - ���尡 ����� �� �ϰ� ���� ���� ������ �� �Լ��� �ּҸ� �˷���
// =============================================================================
FMOD_RESULT CHANNEL_CALLBACK(FMOD_CHANNELCONTROL* channelcontrol, FMOD_CHANNELCONTROL_TYPE controltype
	, FMOD_CHANNELCONTROL_CALLBACK_TYPE callbacktype
	, void* commanddata1, void* commanddata2)
{
	FMOD::Channel* cppchannel = (FMOD::Channel*)channelcontrol;
	HYSound* pSound = nullptr;

	switch (controltype)
	{
		// ä�ο��� ��� ���� ���尡 ����Ǵ� ����
	case FMOD_CHANNELCONTROL_CALLBACK_END:
	{
		// ���� ������ �޾ƿͼ� ä�ο��� remove
		cppchannel->getUserData((void**)&pSound);
		pSound->RemoveChannel(cppchannel);
	}
	break;
	}

	return FMOD_OK;
}