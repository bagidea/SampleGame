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

#define FLIP_NONE SDL_FLIP_NONE
#define FLIP_HORIZONTAL SDL_FLIP_HORIZONTAL
#define FLIP_VERTICAL SDL_FLIP_VERTICAL

using namespace std;

//GameObject Class
class GameObject
{
public:
	int x, y, width, height, rotation;

	GameObject(SDL_Renderer* renderer = NULL);
	~GameObject();

	void SetRenderer(SDL_Renderer* renderer);
	void Load(string path);
	void SetTexture(GameTexture* tex);
	bool CreateGameTextureFromGameSurface(GameSurface* gameSurface);
	void AddClip(SDL_Rect* clip);
	void LoadClip(string path);
	void GenerateClip(int cols, int rows);
	void SetCenter(int x, int y);
	void SetFrame(int index);
	void SetAnimation(int start = 0, int end = 0);
	void SetFlip(SDL_RendererFlip flip = SDL_FLIP_NONE);
	bool HitTest(GameObject* hit);
	void SetTimeScale(float tmr);

	void SetClip(vector<SDL_Rect> clip);
	vector<SDL_Rect> GetClip();

	void Render();
	void ClearClip();

	GameTexture* GetTexture();
	float GetTimeScale();
	bool IsPlay();
	void Play();
	void Stop();
	int GetStart();
	int GetEnd();
	void SetLoop(bool loop);
private:
	float timeScale;
	int speedAnimation;
	bool isPlay;
	bool loop;
	int index;
	int start, end;
	vector<SDL_Rect> clipList;

	SDL_Renderer* renderer;
	GameTexture* tex;
};

#endif