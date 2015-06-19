#include "GameObject.h"

GameObject::GameObject(SDL_Renderer* renderer)
{
	index = 0;

	x = 0;
	y = 0;
	width = 0;
	height = 0;

	this->tex = new GameTexture();
	this->renderer = NULL;
	
	if(renderer != NULL)
	{
		this->renderer = renderer;
		this->tex->SetRenderer(renderer);
	}
}

GameObject::~GameObject()
{
	delete tex;
	tex = NULL;
}

void GameObject::SetRenderer(SDL_Renderer* renderer)
{
	this->renderer = renderer;
	tex->SetRenderer(renderer);
}

void GameObject::Load(string path)
{
	tex->Load(path);

	x = tex->GetX();
	y = tex->GetY();
	width = tex->GetWidth();
	height = tex->GetHeight();
}

void GameObject::AddClip(SDL_Rect* clip)
{
	clipList.push_back(clip);
}

void GameObject::Render()
{
	tex->SetX(x);
	tex->SetY(y);
	tex->SetWidth(width);
	tex->SetHeight(height);

	if(clipList.size() > 0)
	{
		tex->SetClip(clipList[index]->x, clipList[index]->y, clipList[index]->w, clipList[index]->h);
	}

	tex->Render();
}