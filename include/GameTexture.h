#ifndef GAMETEXTURE_INCLUDE_H
#define GAMETEXTURE_INCLUDE_H

#include <iostream>
#include <string>
#include <SDL.h>
#include <SDL_image.h>

using namespace std;

//GameTexture Class
class GameTexture
{
public:
	GameTexture();
	~GameTexture();

	void SetRenderer(SDL_Renderer* renderer);
	bool Load(string path);

	void SetX(int value);
	void SetY(int value);
	void SetWidth(int value);
	void SetHeight(int value);
	void SetCenter(int x, int y);
	void SetRotate(int angle);
	void SetClip(int x, int y, int width, int height);
	void SetFlip(SDL_RendererFlip flip = SDL_FLIP_NONE);
	void SetTexture(SDL_Texture* tex);

	int GetX();
	int GetY();
	int GetWidth();
	int GetHeight();
	SDL_Texture* GetTexture();

	void Render();		
private:
	SDL_Renderer* renderer;
	SDL_Texture* tex;

	int x, y, width, height;
	SDL_Rect clip;
	double rotation;
	SDL_Point center;
	SDL_RendererFlip flip;
};

//GameSurface Class
class GameSurface
{
public:
	GameSurface();
	~GameSurface();

	bool Load(string path);
	SDL_Surface* GetSurface();
private:
	SDL_Surface* surface;
};

#endif