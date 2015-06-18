#include "GameTexture.h"

GameTexture::GameTexture()
{
	tex = NULL;
	x = 0;
	y = 0;
	width = 0;
	height = 0;
	clip = SDL_Rect{0, 0, 0, 0};
	rotation = 0;
	center = SDL_Point{0, 0};
	flip = SDL_FLIP_NONE;
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
		clip = SDL_Rect{0, 0, width, height};;

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

void GameTexture::SetCenter(int x, int y)
{
	center = SDL_Point{x, y};
}

void GameTexture::SetRotate(int angle)
{
	rotation = angle;
}

int GameTexture::GetX(){return x;}
int GameTexture::GetY(){return y;}
int GameTexture::GetWidth(){return width;}
int GameTexture::GetHeight(){return height;}

void GameTexture::Render()
{
	SDL_Rect quad = {x, y, width, height};
	SDL_RenderCopyEx(renderer, tex, &clip, &quad, rotation, &center, flip);
}