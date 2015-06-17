#include "GameTexture.h"

GameTexture::GameTexture()
{
	tex = NULL;
}

GameTexture::~GameTexture()
{
	SDL_DestroyTexture(tex);
	tex = NULL;
}

void GameTexture::SetRenderer(SDL_Renderer* renderer)
{
	this->renderer = renderer;
}

bool GameTexture::Load(string path)
{
	bool success = true;

	SDL_Surface* img = IMG_Load(path.c_str());

	if(img == NULL)
	{
		success = false;
		cerr << "IMG load Error : " << IMG_GetError() << endl;
	}else{
		tex = SDL_CreateTextureFromSurface(renderer, img);
		if(tex == NULL)
		{
			success = false;
			cerr << "SDL Create Texture Error : " << SDL_GetError() << endl;
		}
	}

	return success;
}

void GameTexture::Render()
{
	SDL_RenderCopy(renderer, tex, NULL, NULL);
}