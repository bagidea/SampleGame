#ifndef GAMEOBJECT_INCLUDE_H
#define GAMEOBJECT_INCLUDE_H

#include <iostream>
#include <string>
#include <SDL.h>
#include <SDL_image.h>

#include "GameTexture.h"

using namespace std;

class GameObject
{
public:
	GameObject(SDL_Renderer* renderer = NULL);
	~GameObject();

	void SetRenderer(SDL_Renderer* renderer);
	void Load(string path);

	void Render();
private:
	SDL_Renderer* renderer;
	GameTexture* tex;
};

#endif