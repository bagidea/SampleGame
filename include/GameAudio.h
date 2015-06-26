#ifndef GAMEAUDIO_INCLUDE_H
#define GAMEAUDIO_INCLUDE_H

#include <iostream>
#include <string>
#include <SDL.h>
#include <SDL_mixer.h>

using namespace std;

//AudioBackground
class AudioBackground
{
public:
	AudioBackground();
	~AudioBackground();

	bool Load(string path);

	void Play();
	void Pause();
	void Stop();
private:
	Mix_Music* audio;
};

//AudioClip
class AudioClip
{
public:
	AudioClip();
	~AudioClip();
	
	bool Load(string path);

	void Play();
private:
	Mix_Chunk* audio;
};

#endif