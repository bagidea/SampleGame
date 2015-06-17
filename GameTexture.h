#ifndef GAMETEXTURE_INCLUDE_H
#define GAMETEXTURE_INCLUDE_H

#include <iostream>
#include <string>
#include <SDL.h>
#include <SDL_image.h>

using namespace std;

class GameTexture
{
public:
	GameTexture();
	~GameTexture();

	void SetRenderer(SDL_Renderer* renderer);
	bool Load(string path);

	void Render();		
private:
	SDL_Renderer* renderer;
	SDL_Texture* tex;
};

#endif