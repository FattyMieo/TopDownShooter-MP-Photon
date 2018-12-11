#include "AudioManager.h"

char* AudioManager::pathToMedia = "../Media/";

AudioManager::AudioManager()
{
	FMODExtension::FMOD_Init(m_system);
}

AudioManager::~AudioManager() { }

FMOD::System * AudioManager::GetSystem()
{
	return m_system;
}

bool AudioManager::CreateSFXStream(const char * fileName, AudioType_SFX slot)
{
	if (slot >= SFX_COUNT)
		return false;

	std::string absolutePath = "";
	absolutePath += pathToMedia;
	absolutePath += fileName;
	FMOD_RESULT result = m_system->createStream(absolutePath.c_str(), FMOD_SOFTWARE, 0, &m_sound[slot]);
	FMODExtension::FMOD_ErrorCheck(result);

	return result == FMOD_OK;
}

bool AudioManager::CreateBGMStream(const char * fileName, AudioType_BGM slot)
{
	if (slot >= BGM_COUNT)
		return false;

	std::string absolutePath = "";
	absolutePath += pathToMedia;
	absolutePath += fileName;
	FMOD_RESULT result = m_system->createStream(absolutePath.c_str(), FMOD_SOFTWARE, 0, &m_music[slot]);
	FMODExtension::FMOD_ErrorCheck(result);

	return result == FMOD_OK;
}

bool AudioManager::PlaySFX(float volume, bool forceStart)
{
	return PlaySFX((AudioType_SFX)0, volume, forceStart);
}

bool AudioManager::PlaySFX(AudioType_SFX slot, float volume, bool forceStart)
{
	FMOD_RESULT result = m_system->playSound(FMOD_CHANNEL_REUSE, m_sound[slot], false, &m_soundChannel);
	if (!FMODExtension::FMOD_ErrorCheck(result)) return false;

	result = m_soundChannel->setVolume(volume);
	if (!FMODExtension::FMOD_ErrorCheck(result)) return false;

	return true;
}

bool AudioManager::PlayBGM(float volume, bool forceStart)
{
	return PlayBGM((AudioType_BGM)0, volume, forceStart);
}

bool AudioManager::PlayBGM(AudioType_BGM slot, float volume, bool forceStart)
{
	FMOD_RESULT result = m_system->playSound(FMOD_CHANNEL_REUSE, m_music[slot], false, &m_musicChannel);
	if (!FMODExtension::FMOD_ErrorCheck(result)) return false;

	result = m_musicChannel->setMode(FMOD_LOOP_NORMAL);
	if (!FMODExtension::FMOD_ErrorCheck(result)) return false;

	result = m_musicChannel->setVolume(volume);
	if (!FMODExtension::FMOD_ErrorCheck(result)) return false;

	return true;
}
