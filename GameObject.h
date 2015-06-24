#ifndef GAMEOBJECT_INCLUDE_H
#define GAMEOBJECT_INCLUDE_H

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <cstdio>
#include <SDL.h>
#include <SDL_image.h>

#include "GameTexture.h"

using namespace std;

class GameObject
{
public:
	int x, y, width, height;

	GameObject(SDL_Renderer* renderer = NULL);
	~GameObject();

	void SetRenderer(SDL_Renderer* renderer);
	void Load(string path);
	void SetTexture(GameTexture* tex);
	void AddClip(SDL_Rect* clip);
	void LoadClip(string path);
	void SetFrame(int index);
	void SetAnimation(int start = 0, int end = 0);
	void SetFlip(SDL_RendererFlip flip = SDL_FLIP_NONE);
	bool HitTest(GameObject* hit);
	void SetTimeScale(float tmr);

	void SetClip(vector<SDL_Rect> _clip);
	vector<SDL_Rect> GetClip();

	void Render();
	void ClearClip();

	GameTexture* GetTexture();
	float GetTimeScale();
	bool IsPlay();
	void Play();
	void Stop();
private:
	float timeScale;
	int speedAnimation;
	bool isPlay;
	int index;
	int start, end;
	vector<SDL_Rect> clipList;

	SDL_Renderer* renderer;
	GameTexture* tex;
};

#endif