#include "GameObject.h"

GameObject::GameObject(SDL_Renderer* renderer)
{
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

void GameObject::Render()
{
	tex->SetX(x);
	tex->SetY(y);
	tex->SetWidth(width);
	tex->SetHeight(height);

	tex->Render();
}