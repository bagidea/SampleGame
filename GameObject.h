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
	void AddClip(SDL_Rect* clip);
	void LoadClip(string path);
	void SetFrame(int index);

	void Render();
	void ClearClip();
private:
	int index;
	vector<SDL_Rect> clipList;

	SDL_Renderer* renderer;
	GameTexture* tex;
};

#endif