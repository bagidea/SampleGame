#include "GameTexture.h"

//GameTexture Class
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
	if(tex != NULL)
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
		cerr << "IMG load Error [" << path << "]: " << IMG_GetError() << endl;
	}else{
		width = img->w;
		height = img->h;
		clip = SDL_Rect{0, 0, width, height};

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


void GameTexture::SetClip(int x, int y, int width, int height)
{
	clip = SDL_Rect{x, y, width, height};
}

void GameTexture::SetFlip(SDL_RendererFlip flip)
{
	this->flip = flip;
}

void GameTexture::SetTexture(SDL_Texture* tex)
{
	this->tex = tex;
}

int GameTexture::GetX(){return x;}
int GameTexture::GetY(){return y;}
int GameTexture::GetWidth(){return width;}
int GameTexture::GetHeight(){return height;}
SDL_Texture* GameTexture::GetTexture(){return tex;}

void GameTexture::Render()
{
	SDL_Rect quad = {x, y, width, height};
	SDL_RenderCopyEx(renderer, tex, &clip, &quad, rotation, &center, flip);
}

//GameSurface Class
GameSurface::GameSurface()
{
	surface = NULL;
}

GameSurface::~GameSurface()
{
	if(surface != NULL)
		SDL_FreeSurface(surface);

	surface = NULL;
}

bool GameSurface::Load(string path)
{
	surface = IMG_Load(path.c_str());
	if(surface == NULL)
	{
		cerr << "IMG load Error [" << path << "]: " << IMG_GetError() << endl;
		return false;
	}

	return true;
}

SDL_Surface* GameSurface::GetSurface(){return surface;}