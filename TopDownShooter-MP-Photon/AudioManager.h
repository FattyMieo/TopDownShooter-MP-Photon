#pragma once
#include <fmod.hpp>
#include "FMODExtension.h"
#include "AudioType.h"

#define SFX_COUNT 10
#define BGM_COUNT 1

class AudioManager
{
private:
	static char* pathToMedia;

	FMOD::System* m_system;

	FMOD::Sound* m_sound[SFX_COUNT];
	FMOD::Channel* m_soundChannel;

	FMOD::Sound* m_music[BGM_COUNT];
	FMOD::Channel* m_musicChannel;

public:
	//Singleton
	static AudioManager* GetInstance()
	{
		static AudioManager instance;
		return &instance;
	}

	AudioManager();
	virtual ~AudioManager();

	FMOD::System* GetSystem();
	bool CreateSFXStream(const char* fileName, AudioType_SFX slot);
	bool CreateBGMStream(const char* fileName, AudioType_BGM slot);
	bool PlaySFX(float volume, bool forceStart = false);
	bool PlaySFX(AudioType_SFX slot, float volume, bool forceStart = false);
	bool PlayBGM(float volume, bool forceStart = false);
	bool PlayBGM(AudioType_BGM slot, float volume, bool forceStart = false);
};
