#include "GameAudio.h"

//AudioBackground
AudioBackground::AudioBackground()
{
	audio = NULL;
}

AudioBackground::~AudioBackground()
{
	if(audio != NULL)
		Mix_FreeMusic(audio);
	audio = NULL;
}

bool AudioBackground::Load(string path)
{
	audio = Mix_LoadMUS(path.c_str());
	
	if(audio == NULL)
	{
		cerr << "Mix load Error [" << path << "]: " << Mix_GetError() << endl;
		return false;
	}

	return true;
}

void AudioBackground::Play()
{
	if(Mix_PlayingMusic() == 0)
		Mix_PlayMusic(audio, -1);
	else if(Mix_PausedMusic() == 1)
		Mix_ResumeMusic();
}

void AudioBackground::Pause()
{
	if(Mix_PlayingMusic() != 0 && Mix_PausedMusic() == 0)
		Mix_PauseMusic();
}

void AudioBackground::Stop()
{
	Mix_HaltMusic();
}

//AudioClip
AudioClip::AudioClip()
{
	audio = NULL;
}

AudioClip::~AudioClip()
{
	if(audio != NULL)
		Mix_FreeChunk(audio);
	audio = NULL;
}

bool AudioClip::Load(string path)
{
	audio = Mix_LoadWAV(path.c_str());

	if(audio == NULL)
	{
		cerr << "Mix load Error [" << path << "]: " << Mix_GetError() << endl;
		return false;
	}

	return true;
}

int channelSound = 0;

void AudioClip::Play()
{
	Mix_PlayChannel(channelSound++, audio, 0);

	if(channelSound >= 7)
		channelSound = 0;
}