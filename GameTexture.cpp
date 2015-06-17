#include "GameTexture.h"

GameTexture::GameTexture()
{
	tex = NULL;
	x = 0;
	y = 0;
	width = 0;
	height = 0;
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
		width = img->w;
		height = img->h;

		tex = SDL_CreateTextureFromSurface(renderer, img);
		if(tex == NULL)
		{
			success = false;
			cerr << "SDL Create Texture Error : " << SDL_GetError() << endl;
		}
	}

	return success;
}

void GameTexture::SetX(int value){x = value;}
void GameTexture::SetY(int value){y = value;}
void GameTexture::SetWidth(int value){width = value;}
void GameTexture::SetHeight(int value){height = value;}

int GameTexture::GetX(){return x;}
int GameTexture::GetY(){return y;}
int GameTexture::GetWidth(){return width;}
int GameTexture::GetHeight(){return height;}

void GameTexture::Render()
{
	SDL_Rect quad = {x, y, width, height};
	SDL_RenderCopy(renderer, tex, NULL, &quad);
}